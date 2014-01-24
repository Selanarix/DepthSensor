#include "tempSensor.h"
#include "logger.h"
#include "testSeries.h"

namespace TemperatureSensor 
{
    //-------------------------- Private Types -------------------------------------

    //-------------------- Private Function Prototypes -----------------------------

    TestSeries::TestSeriesCheckResult testTestSeries();
    void readTempSensor(double* measurementOfSeries);
    
    Sensor::AverageMeasurementTestResult testEvaluatedValue(const Temperature dep);
    void errorHandlingTestSeriesError(Sensor::TestSeriesTestResult ab);
    void errorNewAverageDepth(Sensor::AverageMeasurementTestResult b);

    //------------------------- Private Data ---------------------------------------

    Temperature lastMeasurement = 0;
   

    //------------------------ Read only ------------------------------------------

    const TestSeries::TestSeriesControll tempteraturMeasurementControll =
    {
        10,     // maximalMeasurementRetries;
        1000,   // delayForRetry_ms;
        10,      // delayBetweenMeasurements_ms;
        5      // usedMeasurmentsPerTestSeries <= TEST_SERIES_SIZE 
    };

    const int tempSensorPin = 1;
    
    //------------------------------- Public Functions -----------------------------
    
    Temperature getLastMeasurement()
    {
         return lastMeasurement;
    }
 
    void initTemperatureSensorHW()
    {
        pinMode(tempSensorPin, INPUT);
        Logger::log(Logger::INFO, "temperatur sensor initialized");
    }


    Sensor::SensorMeasurmentResult measureTemperature()
    {
        Logger::log(Logger::INFO, "---------------------------------------");
        Logger::log(Logger::INFO, "Start with test series for temperature sensor");
        TestSeries::TestSeriesCheckResult res = TestSeries::measure(&tempteraturMeasurementControll, testTestSeries, readTempSensor);
        
        if(res != TestSeries::TestSeriesOK)
        {
           Logger::log(Logger::ERROR,"Could not measure temperature");
           Logger::log(Logger::INFO, "---------------------------------------"); 
           lastMeasurement = 0;
           return Sensor::TestSeriesError;
        }
        Sensor::SensorMeasurmentResult result = Sensor::SensorValueOK;
        Temperature avgTemperaturOfSeries = (Temperature)TestSeries::getAverageMeanOfSeries(&tempteraturMeasurementControll);
        Sensor::AverageMeasurementTestResult averageSensorTestResult = testEvaluatedValue(avgTemperaturOfSeries);
        //Keep sensor value but generate callback if not as acpected. 
        if(averageSensorTestResult != Sensor::AverageMeasurmentOK)
        {
            errorNewAverageDepth(averageSensorTestResult);
            result = Sensor::SensorValueUnexpected;
        }
        Logger::logInt(Logger::INFO, "Temperatur [C]: ", avgTemperaturOfSeries);
        Logger::log(Logger::INFO, "---------------------------------------");
        lastMeasurement = avgTemperaturOfSeries;
        return result;
    }
    
    void readTempSensor(double* mes)
    {
        *mes = 1;
     /*   double tempvalue = (double)analogRead(tempSensorPin);
        Logger::logDouble(Logger::INFO, "Temp [C]: ",(5000.0 /1024.0 * tempvalue / 10.0));
        return  (temperatur)(5000.0 /1024.0 * tempvalue / 10.0);
     */
    }  
    
    TestSeries::TestSeriesCheckResult testTestSeries()
    { 
        Sensor::TestSeriesTestResult errorTypes = Sensor::TestSeriesOK;
	TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(&tempteraturMeasurementControll);

	double meanVariation = res.max - res.min;
	if(res.min < 0.0001 && res.max < 0.0001)
	    errorTypes = Sensor::SensorOutOfFunction;
	else if(res.min < MINIMAL_EXPECTED_TEMPERATURE_SENSOR_VALUE)
	    errorTypes = Sensor::TestSeriesUnderMinRange;
	else if(res.max > MAXIMAL_EXPECTED_TEMPERATURE_SENSOR_VALUE)
	    errorTypes = Sensor::TestSeriesAboveMaxRange;
	else if(meanVariation > ALLOWED_TEMPERATURE_TEST_SERIES_VARIATION)
	    errorTypes = Sensor::TestSeriesMeanVariationToBig;

        errorHandlingTestSeriesError(errorTypes);
 
        if(errorTypes == Sensor::SensorOutOfFunction)
            return TestSeries::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != Sensor::TestSeriesOK)
            return TestSeries::TestSeriesInvalid;
        
	return TestSeries::TestSeriesOK;
    }
    
    Sensor::AverageMeasurementTestResult testEvaluatedValue(const Temperature dep)
    {
        static Temperature depthHistory = 0;
	
        Sensor::AverageMeasurementTestResult res = Sensor::AverageMeasurmentOK;
        Temperature depthDiff;

        //Calc diff between new and last Depth
        if(dep > depthHistory)
	    depthDiff = dep - depthHistory;
        else	
	    depthDiff = depthHistory - dep; 
        
        if(depthHistory != 0 && depthDiff > MAX_ALLOWED_AVERAGED_TEMPERATURE_VALUE_CHANGE)
            res = Sensor::AverageMeasurmentNotInRange;
        
        depthHistory = dep;	
        return res;
    }

    
    void errorHandlingTestSeriesError(Sensor::TestSeriesTestResult ab)
    {
      switch(ab)
      {
          case Sensor::SensorOutOfFunction:
              Logger::log(Logger::ERROR, "Sensor seams to be out of function. Only 0 measures");
          break;
          case Sensor::TestSeriesUnderMinRange:
              Logger::log(Logger::ERROR, "On of the sensor's values is out of minimal range");
          break;
          case Sensor::TestSeriesAboveMaxRange:
              Logger::log(Logger::ERROR, "On of the sensor's values is out of maximal range");
          break;
          case Sensor::TestSeriesMeanVariationToBig:
              Logger::log(Logger::ERROR, "Mean variaion of sensor's values is out of range");
          break;
          case Sensor::TestSeriesOK:
          break;
          default:
              Logger::log(Logger::ERROR, "General error in test series of depth sensor");
      }
    }

    void errorNewAverageDepth(Sensor::AverageMeasurementTestResult b)
    {
        switch(b)
        {
            case Sensor::AverageMeasurmentNotInRange:
                Logger::log(Logger::ERROR, "Average measurement was out of defined range");
            break;
            default:
                Logger::log(Logger::ERROR, "General error when testing average depth measurement.");
       
        }
    }
}
