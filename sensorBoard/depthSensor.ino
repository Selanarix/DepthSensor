#include "depthSensor.h"  
#include "myUtil.h"
#include "logger.h"

namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------
      
    typedef enum
    {
	    TestSeriesOK,
	    SensorOutOfFunction,
	    TestSeriesUnderMinRange,
	    TestSeriesAboveMaxRange,
	    TestSeriesMeanVariationToBig
    } TestSeriesTestResult;

    typedef enum
    {
	    AverageMeasurmentOK,
	    AverageMeasurmentNotInRange
    } AverageMeasuementTestResult;

    //-------------------- Private Function Prototypes -----------------------------

    void takeTestSeries();
    TestSeriesTestResult testTestSeries();
    depth evaluateTestSeries();
    AverageMeasuementTestResult testEvaluatedValue(const depth depth);
    void readDepthSensor(double* measurementOfSeries);

    void callback1(TestSeriesTestResult);
    void callback2(AverageMeasuementTestResult);

    //------------------------- Private Data ---------------------------------------

    static void (*testSeriesErrorCallback)(TestSeriesTestResult) = callback1;
    static void (*SensorValuesOutOfCourseCallback)(AverageMeasuementTestResult) = callback2;

    static double testSeries[AMOUNT_OF_DEPTH_SPOT_TESTS] = {0};

    //------------------------ Read only ------------------------------------------

    const uint32_t TEST_SERIES_RETRIES_IF_ERROR = 10;
    const uint32_t MEASUREMENT_RETRIE_DELAY_IN_MSEC = 2000;
    const uint32_t TEST_SERIES_MEASURMENT_DELAX_IN_MSEC = 10;

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

    depth measureDepth()
    {
	    uint32_t seriesRetries;
	    depth avgDepthOfSeries = 0;


	    TestSeriesTestResult seriesTestResult = SensorOutOfFunction;

	    //Try to get a valid test serie of depth measurments
	    for(seriesRetries = 0; seriesTestResult != TestSeriesOK &&
			           seriesRetries < TEST_SERIES_RETRIES_IF_ERROR;
	    seriesRetries++)
	    {
		    takeTestSeries();
		    seriesTestResult = testTestSeries();
            //Serial.println(seriesTestResult);
		    if(seriesTestResult != TestSeriesOK)
			    delay(MEASUREMENT_RETRIE_DELAY_IN_MSEC);
	    }
	    //Could not build a valid test series
	    if(seriesTestResult != TestSeriesOK)
	    {
		    if(testSeriesErrorCallback != 0)
			    testSeriesErrorCallback(seriesTestResult);
		    return 0;
	    }

	    //Process data out of test series
	    avgDepthOfSeries = evaluateTestSeries();
	    AverageMeasuementTestResult averageSensorTestResult = testEvaluatedValue(avgDepthOfSeries);
	    //Keep sensor value but generate callback if not as acpected. 
	    if(averageSensorTestResult != AverageMeasurmentOK)
	    {
		    if(SensorValuesOutOfCourseCallback != 0)
			    SensorValuesOutOfCourseCallback(averageSensorTestResult);
	    }
            Logger::logInt(Logger::INFO, "Tiefe [cm]: ", avgDepthOfSeries);
	    return avgDepthOfSeries;
    }

    //------------------------------ Private Functions -----------------------------

    void takeTestSeries()
    {
	    uint32_t measurement;
	    for(measurement = 0; measurement < AMOUNT_OF_DEPTH_SPOT_TESTS; measurement ++)
	    {
		    readDepthSensor(&(testSeries[measurement]));
		    delay(TEST_SERIES_MEASURMENT_DELAX_IN_MSEC);	
	    }		
    }

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

    TestSeriesTestResult testTestSeries()
    {
	    Utils::MinMax res = Utils::evaluateMinMax(testSeries,AMOUNT_OF_DEPTH_SPOT_TESTS);

	    depth meanVariation = res.max - res.min;
	    if(res.min < 0.0001 && res.max < 0.0001)
		    return SensorOutOfFunction;
	    if(res.min < MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
		    return TestSeriesUnderMinRange;
	    if(res.max > MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE)
		    return TestSeriesAboveMaxRange;
	    if(meanVariation > ALLOWED_DEPTH_TEST_SERIES_VARIATION)
		    return TestSeriesMeanVariationToBig;

	    return TestSeriesOK;
    }

    depth evaluateTestSeries()
    {
	    double avg = 0;
	    int t = 1;
	    uint32_t measurement;

	    for(measurement = 0; measurement < AMOUNT_OF_DEPTH_SPOT_TESTS; measurement ++)
	    {
		    double x = testSeries[measurement];
		    avg += (x - avg) / t;
		    t++;
	    }
	    return (depth)avg;
    }

    AverageMeasuementTestResult testEvaluatedValue(const depth dep)
    {
	    static depth depthHistory = 0;
	
	    AverageMeasuementTestResult res = AverageMeasurmentOK;
	    depth depthDiff;

	    //Calc diff between new and last depth
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

    void callback1(TestSeriesTestResult a)
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

    void callback2(AverageMeasuementTestResult b)
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
