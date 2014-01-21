#include "depthSensor.h"  
#include "logger.h"
#include "testSeries.h"

namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------
      
    typedef enum
    {
	    AverageMeasurmentOK,
	    AverageMeasurmentNotInRange
    } AverageMeasuementTestResult;

   typedef enum
    {
         TestSeriesOK,
         SensorOutOfFunction,
         TestSeriesUnderMinRange,
         TestSeriesAboveMaxRange,
         TestSeriesMeanVariationToBig
    } TestSeriesTestResult;

    //-------------------- Private Function Prototypes -----------------------------

    TestSeries::TestSeriesCheckResult testTestSeries();
    Depth evaluateTestSeries();
    AverageMeasuementTestResult testEvaluatedValue(const Depth depth);
    void readDepthSensor(double* measurementOfSeries);

    void errorHandlingTestSeriesError(TestSeriesTestResult);
    void errorNewAverageDepth(AverageMeasuementTestResult);

    //------------------------- Private Data ---------------------------------------

    //------------------------ Read only ------------------------------------------

    const TestSeries::TestSeriesControll depthMeasurementControll =
    {
        10,     // maximalMeasurementRetries;
        2000,   // delayForRetry_ms;
        10,      // delayBetweenMeasurements_ms;
        10      // usedMeasurmentsPerTestSeries <= TEST_SERIES_SIZE 
    };

    /*
    * Input Vout of DiffSensor
    */
    const int depthSensorPin = 0;

    //------------------------------- Public Functions -----------------------------
    /**
     * Init Hw components of the project
     */
    void initDepthSensorHW()
    {
        pinMode(depthSensorPin, INPUT);
        Logger::log(Logger::INFO, "depth sensor initialized");
    }

    /**
    * Measures the depth by taking several measurments and calculates a
    * average one. 
    */
    Depth measureDepth()
    {
	    TestSeries::measure(&depthMeasurementControll, testTestSeries, readDepthSensor);

	    //Process data out of test series
	    Depth avgDepthOfSeries = (Depth)TestSeries::getAverageMeanOfSeries(&depthMeasurementControll);
	    AverageMeasuementTestResult averageSensorTestResult = testEvaluatedValue(avgDepthOfSeries);
	    //Keep sensor value but generate callback if not as acpected. 
	    if(averageSensorTestResult != AverageMeasurmentOK)
			    errorNewAverageDepth(averageSensorTestResult);
        Logger::logInt(Logger::INFO, "Tiefe [cm]: ", avgDepthOfSeries);
	    return avgDepthOfSeries;
    }

    //------------------------------ Private Functions -----------------------------

    void readDepthSensor(double* measurementOfSeries)
    {
        //read value from sensor and assign it to measurementOfSeries
        double pressure = 0.0;
        double adcvalue = 0.0;
        double sensorVoltageADC = 0.0;
        const double offset = 0.19;

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
      
	    *measurementOfSeries = pressure;
    }

    TestSeries::TestSeriesCheckResult testTestSeries()
    {
        TestSeriesTestResult errorTypes = TestSeriesOK;
	TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(&depthMeasurementControll);

	Depth meanVariation = res.max - res.min;
	if(res.min < 0.0001 && res.max < 0.0001)
	    errorTypes = SensorOutOfFunction;
	else if(res.min < MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
	    errorTypes = TestSeriesUnderMinRange;
	else if(res.max > MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
	    errorTypes = TestSeriesAboveMaxRange;
	else if(meanVariation > ALLOWED_DEPTH_TEST_SERIES_VARIATION)
	    errorTypes = TestSeriesMeanVariationToBig;

        errorHandlingTestSeriesError(errorTypes);
 
        if(errorTypes == SensorOutOfFunction)
            return TestSeries::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != TestSeriesOK)
            return TestSeries::TestSeriesInvalid;
        
	    return TestSeries::TestSeriesOK;
    }

    AverageMeasuementTestResult testEvaluatedValue(const Depth dep)
    {
	    static Depth depthHistory = 0;
	
	    AverageMeasuementTestResult res = AverageMeasurmentOK;
	    Depth depthDiff;

	    //Calc diff between new and last Depth
	    if(dep > depthHistory)
		    depthDiff = dep - depthHistory;
	    else	
		    depthDiff = depthHistory - dep; 

	    if(depthHistory != 0 && depthDiff > MAX_ALLOWED_AVERAGED_DEPTH_VALUE_CHANGE)
		    res = AverageMeasurmentNotInRange;

	    depthHistory = dep;	
	    return res;
    }

    //-------------------------------------- E r r o r s ------------------------

    void errorHandlingTestSeriesError(TestSeriesTestResult a)
    {
      switch(a)
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
          default:
              Logger::log(Logger::ERROR, "General error in test series of depth sensor");
      }
    }

    void errorNewAverageDepth(AverageMeasuementTestResult b)
    {
        switch(b)
        {
            case AverageMeasurmentNotInRange:
            break;
                Logger::log(Logger::ERROR, "Average measurement was out of defined range");
            default:
                Logger::log(Logger::ERROR, "General error when testing average depth measurement.");
        }
    }
}
