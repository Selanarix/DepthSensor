#include "depthSensor.h"  
#include "logger.h"
#include "testSeries.h"

namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------

    //-------------------- Private Function Prototypes -----------------------------

    TestSeries::TestSeriesCheckResult testTestSeries();
    Depth evaluateTestSeries();
    Sensor::AverageMeasurementTestResult testEvaluatedValue(const Depth depth);
    void readMPX5500(double* measurementOfSeries);

    void errorHandlingTestSeriesError(Sensor::TestSeriesTestResult);
    void errorNewAverageDepth(Sensor::AverageMeasurementTestResult);

    //------------------------- Private Data ---------------------------------------
    static Depth lastMeasurement = 0;
    //------------------------ Read only ------------------------------------------

    const TestSeries::TestSeriesControll depthMeasurementControll =
    {
        10,     // maximalMeasurementRetries;
        2000,   // delayForRetry_ms;
        10,      // delayBetweenMeasurements_ms;
        10      // usedMeasurmentsPerTestSeries <= TEST_SERIES_SIZE 
    };

    void (* const sensorReadFunctions[])(double*) = 
    {
       readMPX5500, // 0 
    };

    /*
    * Input Vout of DiffSensor
    */
    const int depthSensorPin = 0;

    //------------------------------- Public Functions -----------------------------
    
    Depth getLastMeasurement()
    {
          return lastMeasurement;
    }
    
    /**
     * Init Hw components of the project
     */
    void initDepthSensorHW()
    {
        pinMode(depthSensorPin, INPUT); //TODO
        Logger::log(Logger::INFO, "depth sensor initialized");
    }

    /**
    * Measures the depth by taking several measurments and calculates a
    * average one. 
    */
    Sensor::SensorMeasurmentResult measureDepth(DepthSensor sensor)
    {
        if(sensor > MPX5500DP)
            return Sensor::TestSeriesError;
        Logger::log(Logger::INFO, "---------------------------------------");
        Logger::log(Logger::INFO, "Start with test series for depth sensor");
        TestSeries::TestSeriesCheckResult res = TestSeries::measure(&depthMeasurementControll, testTestSeries, sensorReadFunctions[sensor]);
        if(res != TestSeries::TestSeriesOK)
        {
            Logger::log(Logger::ERROR,"Could not measure depth"); 
            Logger::log(Logger::INFO, "---------------------------------------");
            lastMeasurement = 0;
            return Sensor::TestSeriesError;
        }
	    //Process data out of test series
        Sensor::SensorMeasurmentResult result = Sensor::SensorValueOK;
        Depth avgDepthOfSeries = (Depth)TestSeries::getAverageMeanOfSeries(&depthMeasurementControll);
        Sensor::AverageMeasurementTestResult averageSensorTestResult = testEvaluatedValue(avgDepthOfSeries);
       //Keep sensor value but generate callback if not as acpected. 
        if(averageSensorTestResult != Sensor::AverageMeasurmentOK)
        {
             errorNewAverageDepth(averageSensorTestResult);
             result = Sensor::SensorValueUnexpected;
        }
        Logger::logInt(Logger::INFO, "Tiefe [cm]: ", avgDepthOfSeries);
	  
        Logger::log(Logger::INFO, "---------------------------------------");  
        lastMeasurement = avgDepthOfSeries;
        return result;
    }

    //------------------------------ Private Functions -----------------------------

    void readMPX5500(double* measurementOfSeries)
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

    TestSeries::TestSeriesCheckResult testTestSeries()
    { 
        Sensor::TestSeriesTestResult errorTypes = Sensor::TestSeriesOK;
	TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(&depthMeasurementControll);

	double meanVariation = res.max - res.min;
	if(res.min < 0.0001 && res.max < 0.0001)
	    errorTypes = Sensor::SensorOutOfFunction;
	else if(res.min < MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
	    errorTypes = Sensor::TestSeriesUnderMinRange;
	else if(res.max > MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
	    errorTypes = Sensor::TestSeriesAboveMaxRange;
	else if(meanVariation > ALLOWED_DEPTH_TEST_SERIES_VARIATION)
	    errorTypes = Sensor::TestSeriesMeanVariationToBig;

        errorHandlingTestSeriesError(errorTypes);
 
        if(errorTypes == Sensor::SensorOutOfFunction)
            return TestSeries::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != Sensor::TestSeriesOK)
            return TestSeries::TestSeriesInvalid;
        
	return TestSeries::TestSeriesOK;
    }

    Sensor::AverageMeasurementTestResult testEvaluatedValue(const Depth dep)
    {
	    static Depth depthHistory = 0;
	
	    Sensor::AverageMeasurementTestResult res = Sensor::AverageMeasurmentOK;
	    Depth depthDiff;
	    //Calc diff between new and last Depth
	    if(dep > depthHistory)
		    depthDiff = dep - depthHistory;
	    else	
		    depthDiff = depthHistory - dep; 

	    if(depthHistory != 0 && depthDiff > MAX_ALLOWED_AVERAGED_DEPTH_VALUE_CHANGE)
		    res = Sensor::AverageMeasurmentNotInRange;

	    depthHistory = dep;	
	    return res;
    }

    //-------------------------------------- E r r o r s ------------------------

    void errorHandlingTestSeriesError(Sensor::TestSeriesTestResult an)
    {
      switch(an)
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
