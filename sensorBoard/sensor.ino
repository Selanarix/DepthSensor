#include "sensor.h"  
#include "logger.h"

namespace Sensor 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    static TestSeriesCheckResult takeTestSeriesF(const Sensor* con);
    static double getAverageMeanOfSeriesF(const Sensor* con);
    static MinMax evaluateMinMaxOfTestSeriesF(const Sensor* con);
    static unsigned int getSizeF(const Sensor* con);
    static unsigned char getIDF(const Sensor* con);
    static unsigned char getPinF(const Sensor* con);

    static void takeTestSeries(const Sensor* con);
    //------------------------- Private Global Data ---------------------------------------
    static double testSeries[TEST_SERIES_SIZE] = {0};
    //------------------------ Read only ------------------------------------------

    //------------------------------- Public Functions -----------------------------

    bool construct(Sensor* con, const SensorConstData* constDa, const SensorConstraints * constraint, unsigned int testSeriesSize)
    {
        if(con == NULL || constDa == NULL)
            return false;

        //init data
        if(testSeriesSize > TEST_SERIES_SIZE)
            con->sensorUsedSizeOfTestSeries = TEST_SERIES_SIZE;
        else        
            con->sensorUsedSizeOfTestSeries = testSeriesSize;
        con->sensorConstraints = constraint; 
        con->constData = constDa;

        //init public functions
        con->takeTestSeries = takeTestSeriesF;
        con->getSize = getSizeF;
        con->getAverageMeanOfSeries = getAverageMeanOfSeriesF;
        con->evaluateMinMaxOfTestSeries = evaluateMinMaxOfTestSeriesF;
        con->getID = getIDF;
        con->getPin = getPinF;

        //init abstract methods
        con->readSensorValue = NULL;
        con->checkTestSeries = NULL;
        con->initSensorHW = NULL;
        return true;
    }


    static TestSeriesCheckResult takeTestSeriesF(const Sensor* con)
    {
        //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, "Could not calculate average of test series. Nullpointer given");
            return TestSeriesCancelMeasurement;
        }

        if(con->checkTestSeries == NULL)
        {
            Logger::log(Logger::ERROR, "Can not measure for abstract sensor");
            return TestSeriesCancelMeasurement; 
        }   

        TestSeriesCheckResult seriesTestResult = TestSeriesInvalid;

        for(uint32_t seriesRetries = 0; seriesTestResult != TestSeriesOK &&
                                   seriesRetries < con->constData->controll->MAXIMAL_MEASUREMENT_RETRIES;
	    seriesRetries++)
        {
            takeTestSeries(con);
            if(con->sensorConstraints != NULL)
            {            
                seriesTestResult = con->checkTestSeries(con);
                if(seriesTestResult == TestSeriesCancelMeasurement)
                {
                    Logger::log(Logger::WARNING,"Taking test series where canceled by sensor");
                    break;
                }
                else if(seriesTestResult != TestSeriesOK)
                {
                    Logger::log(Logger::WARNING,"Error in series wait and try again"); 
                    delay(con->constData->controll->DELAY_FOR_RETRY_ms);
                }            
            }
            else
                Logger::log(Logger::WARNING,"No constraint for sensor defined. No check of series!");
        }
        return seriesTestResult;
    }

    static unsigned int getSizeF(const Sensor* con)
    {
         return con->sensorUsedSizeOfTestSeries;
    }

    static unsigned char getIDF(const Sensor* con)
    {
        return con->constData->ID;
    }

    static unsigned char getPinF(const Sensor* con)
    {
        return con->constData->PIN;
    }

    static double getAverageMeanOfSeriesF(const Sensor* con)
    {
        //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, "Could not calculate average of test series. Nullpointer given");
            return 0.0;        
        }

	    double avg = 0;
	    int t = 1;

	    for(uint32_t measurement = 0; measurement < con->sensorUsedSizeOfTestSeries; measurement ++)
	    {
		    double x = testSeries[measurement];
		    avg += (x - avg) / t;
		    t++;
	    }
	    return avg;
    }
    
    static MinMax evaluateMinMaxOfTestSeriesF(const Sensor* con)
    {
	    MinMax res;
	    res.min = 1000000.0;
        res.max = 0.0;
            
	    for(uint32_t i=0; i < con->sensorUsedSizeOfTestSeries; i++)
	    {
	        if(testSeries[i] < res.min)
		    res.min = testSeries[i];
                else if(testSeries[i] > res.max)
		    res.max = testSeries[i];
	    }
	    return res;
    }

    //------------------------------ Private Functions -----------------------------
    void takeTestSeries(const Sensor* con)
    {
	    //Test parameters
        if(con == NULL)
        {
            Logger::log(Logger::ERROR, "Could not fill test series with values because of nullpointer");
            return;        
        }
        if(con->readSensorValue == NULL)
        {
            Logger::log(Logger::ERROR, "Can not take test series for abstract sensor");
            return;        
        }
        uint32_t measurement;
        
	    for(measurement = 0; measurement < con->sensorUsedSizeOfTestSeries; measurement ++)
	    {
	       con->readSensorValue(&(testSeries[measurement]));
	       delay(con->constData->controll->DELAY_BETWEEN_MEASUREMENTS_ms);	
	    }		
    }
}

