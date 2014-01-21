
#ifndef TEST_SERIES_H_
#define TEST_SERIES_H_

#define TEST_SERIES_SIZE 10 

namespace TestSeries 
{ 
    typedef struct
    {
        unsigned int maximalMeasurementRetries;
        unsigned int delayForRetry_ms;
        unsigned int delayBetweenMeasurements_ms;
        unsigned int usedMeasurmentsPerTestSeries;
    } TestSeriesControll;

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
  
    typedef void (*ReadValueCallback)(double*);
    typedef TestSeriesCheckResult(*CheckTestSeriesCallback)(void);

    void measure(const TestSeriesControll* con, CheckTestSeriesCallback testSeries, ReadValueCallback readSensor);
    double getAverageMeanOfSeries(const TestSeriesControll* con);
    MinMax evaluateMinMaxOfTestSeries(const TestSeriesControll* con);
}
#endif /* DEPTHSENSOR_H_ */
