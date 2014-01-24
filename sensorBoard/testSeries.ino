#include "testSeries.h"  
#include "logger.h"

namespace TestSeries 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    void takeTestSeries(const TestSeriesControll* con, ReadValueCallback readSensorF);
    //------------------------- Private Data ---------------------------------------
    static double testSeries[TEST_SERIES_SIZE] = {0};
    //------------------------ Read only ------------------------------------------

    //------------------------------- Public Functions -----------------------------
    TestSeriesCheckResult measure(const TestSeriesControll* con, CheckTestSeriesCallback testTestSeries, ReadValueCallback readSensor)
    {
        if(con == NULL || testSeries == NULL || readSensor == NULL)
            return TestSeriesCancelMeasurement;

        TestSeriesCheckResult seriesTestResult = TestSeriesInvalid;

        for(uint32_t seriesRetries = 0; seriesTestResult != TestSeriesOK &&
                                   seriesRetries < con->maximalMeasurementRetries;
	seriesRetries++)
        {
            takeTestSeries(con, readSensor);
            seriesTestResult = testTestSeries();
            if(seriesTestResult == TestSeriesCancelMeasurement)
            {
                  Logger::log(Logger::WARNING,"Taking test series where canceled by sensor");
                  break;
            }
            else if(seriesTestResult != TestSeriesOK)
            {
                Logger::log(Logger::WARNING,"Error in series wait and try again"); 
                delay(con->delayForRetry_ms);
            }
        }
        return seriesTestResult;
    }


    double getAverageMeanOfSeries(const TestSeriesControll* con)
    {
        //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, "Could not calculate average of test series. Nullpointer given");
            return 0.0;        
        }

	    double avg = 0;
	    int t = 1;

	    for(uint32_t measurement = 0; measurement < con->usedMeasurmentsPerTestSeries && measurement < TEST_SERIES_SIZE; measurement ++)
	    {
		    double x = testSeries[measurement];
		    avg += (x - avg) / t;
		    t++;
	    }
	    return avg;
    }
    
    MinMax evaluateMinMaxOfTestSeries(const TestSeriesControll* con)
    {
	    MinMax res;
	    res.min = 1000000.0;
            res.max = 0.0;
            
	    for(uint32_t i=0; i < con->usedMeasurmentsPerTestSeries && i < TEST_SERIES_SIZE; i++)
	    {
	        if(testSeries[i] < res.min)
		    res.min = testSeries[i];
                else if(testSeries[i] > res.max)
		    res.max = testSeries[i];
	    }
	    return res;
    }

    //------------------------------ Private Functions -----------------------------
    void takeTestSeries(const TestSeriesControll* con, ReadValueCallback readSensor)
    {
	    uint32_t measurement;
        //Test parameters
        if(readSensor == NULL || con == NULL)
        {
            Logger::log(Logger::ERROR, "Could not fill test series with values because of nullpointer");
            return;        
        }
        
	for(measurement = 0; measurement < con->usedMeasurmentsPerTestSeries && measurement < TEST_SERIES_SIZE; measurement ++)
	{
	   readSensor(&(testSeries[measurement]));
	   delay(con->delayBetweenMeasurements_ms);	
	}		
    }
}

