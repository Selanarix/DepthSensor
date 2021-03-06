#include "depthSensor.h"  
#include "logger.h"
#include "sensorErrorTypes.h"

namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------

    void readMPX5500Sensor(double* measurementOfSeries, const void* b);
    void readMPX5100Sensor(double* measurementOfSeries, const void* c);
    static inline Depth roundToDepth(double);
      

    TestSeries::TestSeriesCheckResult testTestSeries(const TestSeries::TestSeries* s, const Sensor::SensorConstraints* con);
    bool testEvaluatedValue(const DepthSensor* con, const Depth dep);

    static void logDepthTestSeriesError(SensorError::TestSeriesError an);
    static void logAverageDepthErrors(SensorError::AverageMeasurementError b);

    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    static const char* sensorNames[] = 
    {
      "MPX5500",
      "MPX5100"
    };
    
    //------------------------ Public Functions ------------------------------------
    bool construct(DepthSensor* tSen, 
                    const Sensor::SensorConstData* constDa, 
                    const TestSeries::TestSeriesControll* controll,
                    const Sensor::SensorConstraints* cons, 
                    DepthSensorType t, uint32_t size)
    {
        if(tSen == NULL || constDa == NULL || controll == NULL || cons == NULL)
            return false;
 
        tSen->lastDepth = 0.0;
        tSen->constrains = cons;
        tSen->constData = constDa;
        tSen->type = t;
  
        switch(t)
        {
            case MPX5500:
                return TestSeries::construct(&(tSen->series),  controll, readMPX5500Sensor, testTestSeries, size); 
            case MPX5100:
                return TestSeries::construct(&(tSen->series),  controll, readMPX5100Sensor, testTestSeries, size); 
        }
  
        return false;
    }    

    uint8_t getSensorID(const DepthSensor* con)
    {
          if(con == NULL)
              return 0;
          return con->constData->ID;
    }

    Depth getLastDepth(const DepthSensor* con)
    {
          if(con == NULL)
              return 0;
          return con->lastDepth;
    }
    
    const Sensor::SensorStringInformation sensorGetStringInfo()
    {
       Sensor::SensorStringInformation res = 
       {
          F("Depth: "),
          UNIT_cm
       };
       return res;
    }
    
    void initDepthSensorHW(const DepthSensor* con)
    {
        //pinMode(con->getPin((Sensor::Sensor*)con),INPUT);
        Logger::log(Logger::INFO, F("depth sensor initialized"));
    }

    Sensor::MeasurementResult measureDepth(DepthSensor* pSen)
    {
        if(pSen == NULL)
        {
            Logger::log(Logger::ERROR,F("Can not measure for null instance of depth sensor"));
            return Sensor::MeasurementError;
        }    
            
        Logger::printSeperator();//------------------------------------------
        Logger::logString(Logger::INFO, F("Measure depth sensor: "), sensorNames[pSen->type]);
        Logger::logInt(Logger::DEBUG, F("Sensor id: "),(uint32_t)pSen->constData->ID);
        TestSeries::TestSeriesCheckResult res = TestSeries::takeTestSeries(&(pSen->series), pSen->constrains, pSen);

        if(res != TestSeries::TestSeriesOK)
        {
            Logger::log(Logger::ERROR,F("Could not measure depth"));
            Logger::printSeperator();//------------------------------------------
            pSen->lastDepth = 0.0;
            return Sensor::MeasurementError;
        }
	    //Process data out of test series
        Sensor::MeasurementResult result = Sensor::MeasurementOK;
        Depth avgDepthOfSeries = roundToDepth(TestSeries::getAverageMeanOfSeries(&(pSen->series)));
      
        //Keep sensor value but generate callback if not as acpected. 
        if(!testEvaluatedValue(pSen,avgDepthOfSeries))
            result = Sensor::MeasurementValueUnexpected;

        Logger::logInt(Logger::INFO, F("Tiefe [cm]: "), avgDepthOfSeries);
        Logger::printSeperator();//------------------------------------------
        pSen->lastDepth = avgDepthOfSeries;
        return result;
    }

    //------------------------------ Private Functions -----------------------------
    void readMPX5100Sensor(double* measurementOfSeries, const void* ob)
    {
        //read value from sensor and assign it to measurementOfSeries
        double sensor_mV = 0.0;
        const double offset = 190.0;
        
        if(ob == NULL)
            return;
            
        const DepthSensor* thi = (DepthSensor*)ob;
          
        //Formel aus Datenblatt (in kPa): Vout = Vs*(0,009*p+0,04)        
        //Umgestellt nach p (in hPa!!): p=(Vout-200)/4.5
     sensor_mV = (double)HAL::analogReadVoltage(thi->constData->PIN);
        *measurementOfSeries = (sensor_mV-offset)/3.88;

    }

    void readMPX5500Sensor(double* measurementOfSeries, const void* ob)
    {
        //read value from sensor and assign it to measurementOfSeries
        double pressure = 0.0;
        double adcvalue = 0.0;
        double sensorVoltageADC = 0.0;
        const double offset = 0.180;
        
        if(ob == NULL)
            return;
        
        const DepthSensor* thi = (DepthSensor*)ob;
        
        adcvalue = (double)HAL::analogReadPin(thi->constData->PIN);
        sensorVoltageADC = (adcvalue)*(5.0/4096.0);
        pressure = (sensorVoltageADC-offset)*1111;
        
	*measurementOfSeries = pressure;
    }

    TestSeries::TestSeriesCheckResult testTestSeries(const TestSeries::TestSeries* s, const Sensor::SensorConstraints* con)
    { 
        if(s == NULL || con == NULL)
            return TestSeries::TestSeriesCancelMeasurement;

        using namespace SensorError;

        TestSeriesError errorTypes = TestSeriesOK;
        TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(s);

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
            return TestSeries::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != TestSeriesOK)
            return TestSeries::TestSeriesInvalid;
            
	    return TestSeries::TestSeriesOK;
    }

    bool testEvaluatedValue(const DepthSensor* sen, const Depth dep)
    {
        if(sen == NULL)
        {
            Logger::log(Logger::WARNING,F("Could not test new depth because no constraint defined"));        
            return false;
        }   
        const Sensor::SensorConstraints* con = sen->constrains;
      
        bool res = true;
        SensorError::AverageMeasurementError errorCode = SensorError::AverageMeasurmentOK;
        Depth depthDiff;
        //Calc diff between new and last Depth
        if(dep > sen->lastDepth)
            depthDiff = dep - sen->lastDepth;
	else	
            depthDiff = sen->lastDepth - dep; 

        if(sen->lastDepth != 0 && depthDiff > con->MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
        {
            errorCode = SensorError::AverageMeasurmentNotInRange;
            res = false;        
        }
        logAverageDepthErrors(errorCode); //log errors
       
        return res;
    }
    
    static inline Depth roundToDepth(double roundedDepth)
    {
      Depth result = (Depth)roundedDepth;
      if (roundedDepth - result > 0.5)
          return result+1;
      
      return result;
    }

    //-------------------------------------- E r r o r s ------------------------

    void logDepthTestSeriesError(SensorError::TestSeriesError an)
    {
      using namespace SensorError;
      switch(an)
      {
          case SensorOutOfFunction:
              Logger::log(Logger::ERROR, F("Sensor seams to be out of function. Only 0 measures"));
          break;
          case TestSeriesUnderMinRange:
              Logger::log(Logger::ERROR, F("On of the sensor's values is out of minimal range"));
          break;
          case TestSeriesAboveMaxRange:
              Logger::log(Logger::ERROR, F("On of the sensor's values is out of maximal range"));
          break;
          case TestSeriesMeanVariationToBig:
              Logger::log(Logger::ERROR, F("Mean variaion of sensor's values is out of range"));
          break;
          case TestSeriesOK:
          break;
          default:
              Logger::log(Logger::ERROR, F("General error in test series of depth sensor"));
      }
    }

    void logAverageDepthErrors(SensorError::AverageMeasurementError b)
    {
        using namespace SensorError;
        switch(b)
        {
            case AverageMeasurmentOK:
                return;
            case AverageMeasurmentNotInRange:
                Logger::log(Logger::ERROR, F("Average measurement was out of defined range"));
            break;
            default:
                Logger::log(Logger::ERROR, F("General error when testing average depth measurement."));
        }
    }
}
