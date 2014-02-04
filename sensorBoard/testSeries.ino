#include "testSeries.h"  
#include "logger.h"

namespace TestSeries
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    void measureSeries(const TestSeries* con, const void*);
    //------------------------- Private Global Data ---------------------------------------
    static double testSeriesData[TEST_SERIES_SIZE] = {0};
    //------------------------ Read only ------------------------------------------
    
    //------------------------------- Public Functions -----------------------------
    bool construct(TestSeries* con, 
            const TestSeriesControll* controll, 
            void (*sensorRead)(double*, const void* ), 
            TestSeriesCheckResult(*checkTestSeries)(const TestSeries*, const Sensor::SensorConstraints*), 
            uint32_t testSeriesSize)
    {
        if(con == NULL || controll == NULL || checkTestSeries == NULL || sensorRead == NULL)
            return false;
    
        //init data
        if(testSeriesSize > TEST_SERIES_SIZE)
            con->sensorUsedSizeOfTestSeries = TEST_SERIES_SIZE;
        else        
            con->sensorUsedSizeOfTestSeries = testSeriesSize;
    
        con->testSeries = testSeriesData;
        con->seriesControll = controll;
        con->readSensorValue = sensorRead;
        con->checkTestSeries = checkTestSeries;
    
        return true;
    }
    
    
    TestSeriesCheckResult takeTestSeries(const TestSeries* con, const Sensor::SensorConstraints* constraints, const void* b)
    {
        //Test parameters
        if(con == NULL || constraints == NULL)
        {
            Logger::log(Logger::ERROR, F("Can not take test series. Nullpointer as data given"));
            return TestSeriesCancelMeasurement;
        }
    
        if(con->checkTestSeries == NULL || con->readSensorValue == NULL)
        {
            Logger::log(Logger::ERROR, F("Can not measure without callback functions"));
            return TestSeriesCancelMeasurement; 
        }   
    
        TestSeriesCheckResult seriesTestResult = TestSeriesInvalid;
    
        for(uint32_t seriesRetries = 0; seriesTestResult != TestSeriesOK &&
                                   seriesRetries < con->seriesControll->MAXIMAL_MEASUREMENT_RETRIES;
        seriesRetries++)
        {
            measureSeries(con, b);
            seriesTestResult = con->checkTestSeries(con,constraints);
            if(seriesTestResult == TestSeriesCancelMeasurement)
            {
                Logger::log(Logger::WARNING,F("Taking test series where canceled by sensor"));
                break;
            }
            else if(seriesTestResult != TestSeriesOK)
            {
                Logger::log(Logger::WARNING,F("Error in series wait and try again")); 
                delay(con->seriesControll->DELAY_FOR_RETRY_ms);
            }            
        }
        return seriesTestResult;
    }
    
    double getAverageMeanOfSeries(const TestSeries* con)
    {
        //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, F("No average for nullpointer"));
            return 0.0;        
        }
    
        double avg = 0;
        int t = 1;
    
        for(uint32_t measurement = 0; measurement < con->sensorUsedSizeOfTestSeries; measurement ++)
        {
    	    double x = testSeriesData[measurement];
    	    avg += (x - avg) / t;
    	    t++;
        }
        return avg;
    }
    
    MinMax evaluateMinMaxOfTestSeries(const TestSeries* con)
    {
    
        MinMax res;
        res.min = 0.0;
        res.max = 0.0;
        //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, F("No min or max for nullpointer"));
            return res;        
        }
        res.min = 1000000.0;
            
        for(uint32_t i=0; i < con->sensorUsedSizeOfTestSeries; i++)
        {
            if(testSeriesData[i] < res.min)
    	    res.min = testSeriesData[i];
                else if(testSeriesData[i] > res.max)
    	    res.max = testSeriesData[i];
        }
        return res;
    }
    
    //------------------------------ Private Functions -----------------------------
    void measureSeries(const TestSeries* con, const void* obj)
    {
        uint32_t measurement;
        
        for(measurement = 0; measurement < con->sensorUsedSizeOfTestSeries; measurement ++)
        {
           con->readSensorValue(&(testSeriesData[measurement]),obj);
           delay(con->seriesControll->DELAY_BETWEEN_MEASUREMENTS_ms);	
        }		
    }
}
