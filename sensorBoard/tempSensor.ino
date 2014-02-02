#include "tempSensor.h"
#include "logger.h"
#include "sensorErrorTypes.h"

namespace TemperatureSensor 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------

    static Sensor::TestSeriesCheckResult testTestSeriesF(const TemperaturSensor* con);
    static void readLM35_F(double* measurementOfSeries);
    static void initADC_PIN(const TemperaturSensor* con); 
    static Sensor::MeasurmentResult measureTemperatureF(TemperaturSensor* con);   
    static Temperature getLastMeasurementF(const TemperaturSensor* con);

    static SensorError::AverageMeasurementError testEvaluatedValue(const TemperaturSensor* con, const Temperature dep);
    static void logTemperaturTestSeriesError(SensorError::TestSeriesError ab);
    static void logAverageTemperaturErrors(SensorError::AverageMeasurementError b);

    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    //------------------------ Public Functions ------------------------------------
    bool construct(TemperaturSensor* con, const Sensor::SensorConstData* constDa, const Sensor::SensorConstraints* constraint, unsigned int s, TemperaturSensorType t)
    {
        if(!Sensor::construct((Sensor::Sensor*) con, constDa, constraint,s))
            return false;
        //specify abstract object       
        switch(t)
        {
            case LM35:
                con->readSensorValue = readLM35_F;
                con->initSensorHW = initADC_PIN;
            break;
            default:
                return false;        
        }
        con->checkTestSeries = testTestSeriesF;
        
        //extend object
        con->lastTemperature = 0.0;
        con->getTemperature = getLastMeasurementF;
        con->measureTemperature = measureTemperatureF;
        return true;
    }

    static Temperature getLastMeasurementF(const TemperaturSensor* con)
    {
         if(con == NULL)
              return 0;
         return con->lastTemperature;
    }
 
    static void initADC_PIN(const TemperaturSensor* con)
    {
       // HAL::(con->getPin((Sensor::Sensor*)con),INPUT);
        Logger::log(Logger::INFO, "temperatur sensor initialized");
    }

    Sensor::MeasurmentResult measureTemperatureF(TemperaturSensor* con)
    {
        if(con == NULL)
        {
            Logger::log(Logger::ERROR,"Can not measure for null instance of temperatur sensor");
            return Sensor::MeasurmentError;
        }    
        Logger::log(Logger::INFO, "---------------------------------------");
        Logger::logInt(Logger::INFO, "Start with test series for temperature sensor with id: ",(uint32_t)con->getID(con));
        Sensor::TestSeriesCheckResult res = con->takeTestSeries(con);
        
        if(res != Sensor::TestSeriesOK)
        {
           Logger::log(Logger::ERROR,"Could not measure temperature");
           Logger::log(Logger::INFO, "---------------------------------------");
           con->lastTemperature = 0;
           return Sensor::MeasurmentError;
        }
        Sensor::MeasurmentResult result = Sensor::MeasurmentOK;
        Temperature avgTemperaturOfSeries = con->getAverageMeanOfSeries(con);
        SensorError::AverageMeasurementError averageSensorTestResult = testEvaluatedValue(con,avgTemperaturOfSeries);
        //Keep sensor value but generate callback if not as acpected. 
        if(averageSensorTestResult != SensorError::AverageMeasurmentOK)
        {
            logAverageTemperaturErrors(averageSensorTestResult);
            result = Sensor::MeasurmentValueUnexpected;
        }
        Logger::logInt(Logger::INFO, "Temperatur [C]: ", avgTemperaturOfSeries);
        Logger::log(Logger::INFO, "---------------------------------------");
        con->lastTemperature = avgTemperaturOfSeries;
        return result;
    }
    
    static void readLM35_F(double* mes)
    {
        static int a = 0;
        *mes = a++;
     /*   double tempvalue = (double)analogRead(tempSensorPin);
        Logger::logDouble(Logger::INFO, "Temp [C]: ",(5000.0 /1024.0 * tempvalue / 10.0));
        return  (temperatur)(5000.0 /1024.0 * tempvalue / 10.0);
     */
    }  
    
    static Sensor::TestSeriesCheckResult testTestSeriesF(const TemperaturSensor* sen)
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

        logTemperaturTestSeriesError(errorTypes); //log errors

        if(errorTypes == SensorOutOfFunction)
            return Sensor::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != TestSeriesOK)
            return Sensor::TestSeriesInvalid;
            
        return Sensor::TestSeriesOK;
    }
    
    static SensorError::AverageMeasurementError testEvaluatedValue(const TemperaturSensor* sen,const Temperature dep)
    {
        static Temperature depthHistory = 0;
	
        const Sensor::SensorConstraints* con = sen->sensorConstraints;
        if(con == NULL)
        {
            Logger::log(Logger::WARNING,"Could not test new temperature because no constraint defined");        
            return SensorError::AverageMeasurmentOK;
        }
        SensorError::AverageMeasurementError res = SensorError::AverageMeasurmentOK;
        Temperature depthDiff;

        //Calc diff between new and last Depth
        if(dep > depthHistory)
	        depthDiff = dep - depthHistory;
        else	
	        depthDiff = depthHistory - dep; 
        
        if(sen->lastTemperature != 0 && depthDiff >  con->MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
            res = SensorError::AverageMeasurmentNotInRange;
       
        return res;
    }

    //-------------------------------------- E r r o r s ------------------------
    
    static void logTemperaturTestSeriesError(SensorError::TestSeriesError ab)
    {
      using namespace SensorError;
      switch(ab)
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

    static void logAverageTemperaturErrors(SensorError::AverageMeasurementError b)
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
