#include "depthSensor.h"  
#include "logger.h"
#include "sensorErrorTypes.h"


namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------

    static Sensor::TestSeriesCheckResult testDepthSeriesF(const DepthSensor* con);
    static void readMPX5500SensorF(double* measurementOfSeries);
    static void initDepthSensorHW_F(const DepthSensor* con); 
    static Sensor::MeasurmentResult measureDepthF(DepthSensor* con);   
    static Depth getLastDepthF(const DepthSensor* con);

    static SensorError::AverageMeasurementError testEvaluatedValue(const DepthSensor* con, const Depth dep);
    static void logDepthTestSeriesError(SensorError::TestSeriesError an);
    static void logAverageDepthErrors(SensorError::AverageMeasurementError b);

    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    //------------------------ Public Functions ------------------------------------
    bool construct(DepthSensor* con, const Sensor::SensorConstData* constDa, const Sensor::SensorConstraints* constraint, unsigned int s, DepthSensorType t)
    {
        if(!Sensor::construct((Sensor::Sensor*) con, constDa, constraint,s))
            return false;
        //specify abstract object       
        switch(t)
        {
            case MPX5500:
                con->readSensorValue = readMPX5500SensorF;
                break;
            default:
                return false;
        }
        con->checkTestSeries = testDepthSeriesF;
        con->initSensorHW = initDepthSensorHW_F;
        
        //extend object
        con->lastDepth = 0.0;
        con->getDepth = getLastDepthF;
        con->measureDepth = measureDepthF;
        return true;
    }    

    static Depth getLastDepthF(const DepthSensor* con)
    {
          if(con == NULL)
              return 0;
          return con->lastDepth;
    }
    
    static void initDepthSensorHW_F(const DepthSensor* con)
    {
        //pinMode(con->getPin((Sensor::Sensor*)con),INPUT);
        Logger::log(Logger::INFO, "depth sensor initialized");
    }

    static Sensor::MeasurmentResult measureDepthF(DepthSensor* con)
    {
        if(con == NULL)
        {
            Logger::log(Logger::ERROR,"Can not measure for null instance of depth sensor");
            return Sensor::MeasurmentError;
        }    
            
        Logger::log(Logger::INFO, "---------------------------------------");
        Logger::logInt(Logger::INFO, "Start with test series for depth sensor with id: ",(uint32_t)con->getID(con));
        Sensor::TestSeriesCheckResult res = con->takeTestSeries(con);

        if(res != Sensor::TestSeriesOK)
        {
            Logger::log(Logger::ERROR,"Could not measure depth"); 
            Logger::log(Logger::INFO, "---------------------------------------");
            con->lastDepth = 0;
            return Sensor::MeasurmentError;
        }
	    //Process data out of test series
        Sensor::MeasurmentResult result = Sensor::MeasurmentOK;
        Depth avgDepthOfSeries = con->getAverageMeanOfSeries(con);
        SensorError::AverageMeasurementError averageSensorTestResult = testEvaluatedValue(con,avgDepthOfSeries);
       //Keep sensor value but generate callback if not as acpected. 
        if(averageSensorTestResult != SensorError::AverageMeasurmentOK)
        {
             logAverageDepthErrors(averageSensorTestResult);
             result = Sensor::MeasurmentValueUnexpected;
        }
        Logger::logInt(Logger::INFO, "Tiefe [cm]: ", avgDepthOfSeries);
        Logger::log(Logger::INFO, "---------------------------------------");  
        con->lastDepth = avgDepthOfSeries;
        return result;
    }

    //------------------------------ Private Functions -----------------------------

    void readMPX5500SensorF(double* measurementOfSeries)
    {
        //read value from sensor and assign it to measurementOfSeries
        double pressure = 0.0;
        double adcvalue = 0.0;
        double sensorVoltageADC = 0.0;
        const double offset = 0.19;
        
         static int ax = 0;
        *measurementOfSeries = ax++; 
       /*
        adcvalue = (double)analogRead(depthSensorPin);
        //      Serial.print("Rohwert [ADC]: ");
        //      Serial.println(adcvalue);
        
        //Formel aus Datenblatt (in kPa): Vout = Vs*(0,0018*p+0,04)        
        //Umgestellt nach p (in hPa!!): p=(Vout-0,2)/(9*10^-4)

        sensorVoltageADC = (adcvalue)*(5.0/1024.0);
        //      Serial.print("Rohwert [V]: ");
        //      Serial.println(sensorVoltageADC);

        //        pressure = (sensorVoltageADC*10.0-(offset*10))*1000/9;
        pressure = (sensorVoltageADC-offset)*1111;
        //      Serial.print("Druck [hPa]: ");
        //      Serial.println(pressure);
      */
	//    *measurementOfSeries = pressure;
    }

    static Sensor::TestSeriesCheckResult testDepthSeriesF(const DepthSensor* sen)
    { 
        if(sen == NULL)
            return Sensor::TestSeriesCancelMeasurement;

        using namespace SensorError;

        const Sensor::SensorConstraints* con = sen->sensorConstraints;
        TestSeriesError errorTypes = TestSeriesOK;
	    Sensor::MinMax res = sen->evaluateMinMaxOfTestSeries(sen);

	    double meanVariation = res.max - res.min;
	    if(res.min < 0.0001 && res.max < 0.0001)
	        errorTypes = SensorOutOfFunction;
	    else if(res.min < con->MINIMAL_EXPECTED_SENSOR_VALUE)
	        errorTypes = TestSeriesUnderMinRange;
	    else if(res.max > con->MAXIMAL_EXPECTED_SENSOR_VALUE)
	        errorTypes = TestSeriesAboveMaxRange;
	    else if(meanVariation > con->ALLOWED_TEST_SERIES_VARIATION)
	        errorTypes = TestSeriesMeanVariationToBig;

        logDepthTestSeriesError(errorTypes);
 
        if(errorTypes == SensorOutOfFunction)
            return Sensor::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != TestSeriesOK)
            return Sensor::TestSeriesInvalid;
            
	    return Sensor::TestSeriesOK;
    }

    static SensorError::AverageMeasurementError testEvaluatedValue(const DepthSensor* sen, const Depth dep)
    {
        static Depth depthHistory = 0;
        const Sensor::SensorConstraints* con = sen->sensorConstraints;
        if(con == NULL)
        {
            Logger::log(Logger::WARNING,"Could not test new depth because no constraint defined");        
            return SensorError::AverageMeasurmentOK;
        }
        SensorError::AverageMeasurementError res = SensorError::AverageMeasurmentOK;
        Depth depthDiff;
        //Calc diff between new and last Depth
        if(dep > depthHistory)
            depthDiff = dep - depthHistory;
	else	
     depthDiff = depthHistory - dep; 

        if(sen->lastDepth != 0 && depthDiff > con->MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
            res = SensorError::AverageMeasurmentNotInRange;
       
        return res;
    }

    //-------------------------------------- E r r o r s ------------------------

    void logDepthTestSeriesError(SensorError::TestSeriesError an)
    {
      using namespace SensorError;
      switch(an)
      {
          case SensorOutOfFunction:
              Logger::log(Logger::ERROR, "Sensor seams to be out of function. Only 0 measures");
          break;
          case TestSeriesUnderMinRange:
              Logger::log(Logger::ERROR, "On of the sensor's values is out of minimal range");
          break;
          case TestSeriesAboveMaxRange:
              Logger::log(Logger::ERROR, "On of the sensor's values is out of maximal range");
          break;
          case TestSeriesMeanVariationToBig:
              Logger::log(Logger::ERROR, "Mean variaion of sensor's values is out of range");
          break;
          case TestSeriesOK:
          break;
          default:
              Logger::log(Logger::ERROR, "General error in test series of depth sensor");
      }
    }

    void logAverageDepthErrors(SensorError::AverageMeasurementError b)
    {
        using namespace SensorError;
        switch(b)
        {
            case AverageMeasurmentNotInRange:
                Logger::log(Logger::ERROR, "Average measurement was out of defined range");
            break;
            default:
                Logger::log(Logger::ERROR, "General error when testing average depth measurement.");
        }
    }
}
