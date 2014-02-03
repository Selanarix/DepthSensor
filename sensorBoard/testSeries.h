#ifndef TEST_SERIES_H_
#define TEST_SERIES_H_

#include <Arduino.h>
#include "sensor.h"

#define TEST_SERIES_SIZE 10 

namespace TestSeries
{   
    typedef enum
    {
        TestSeriesOK,
        TestSeriesCancelMeasurement,
        TestSeriesInvalid
    } TestSeriesCheckResult;
  
    typedef struct
    {
        double min;
        double max;
    } MinMax;

    typedef struct
    {
        const uint32_t MAXIMAL_MEASUREMENT_RETRIES;
        const uint32_t DELAY_FOR_RETRY_ms;
        const uint32_t DELAY_BETWEEN_MEASUREMENTS_ms;
    } TestSeriesControll;

    typedef struct _TestSeries TestSeries;

    struct _TestSeries
    {
        uint32_t sensorUsedSizeOfTestSeries;
        const double* testSeries;
        const TestSeriesControll* seriesControll;
        void (*readSensorValue)(double*);
        TestSeriesCheckResult(*checkTestSeries)(const TestSeries* , const Sensor::SensorConstraints* );
    };

    bool construct(TestSeries* con, 
            const TestSeriesControll* controll, 
            void (*sensorRead)(double*), 
            TestSeriesCheckResult(*checkTestSeries)(const TestSeries*, const Sensor::SensorConstraints*), 
            uint32_t testSeriesSize);
    TestSeriesCheckResult takeTestSeries(const TestSeries* con, const Sensor::SensorConstraints*);
    double getAverageMeanOfSeries(const TestSeries* con);
    MinMax evaluateMinMaxOfTestSeries(const TestSeries* con);
}
#endif
