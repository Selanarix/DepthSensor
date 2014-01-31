
#ifndef SENSOR_H_
#define SENSOR_H_

#define TEST_SERIES_SIZE 10 

namespace Sensor
{   
    typedef enum
    {
        TestSeriesOK,
        TestSeriesCancelMeasurement,
        TestSeriesInvalid
    } TestSeriesCheckResult;
  
    typedef enum 
    {
        MeasurmentOK,
        MeasurmentError,
        MeasurmentValueUnexpected
    } MeasurmentResult;

    typedef struct
    {
        double min;
        double max;
    } MinMax;

    typedef struct
    {
        const double MINIMAL_EXPECTED_SENSOR_VALUE;
        const double MAXIMAL_EXPECTED_SENSOR_VALUE;
        const double ALLOWED_TEST_SERIES_VARIATION;
        const double MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
    } SensorConstraints;

    typedef struct
    {
        const unsigned int MAXIMAL_MEASUREMENT_RETRIES;
        const unsigned int DELAY_FOR_RETRY_ms;
        const unsigned int DELAY_BETWEEN_MEASUREMENTS_ms;
    } TestSeriesControll;

    typedef struct
    {
        const TestSeriesControll* const controll;
        const unsigned char PIN;
        const unsigned char ID;
    } SensorConstData;

    typedef struct _Sensor Sensor;

    struct _Sensor
    {
        //Data
        unsigned int sensorUsedSizeOfTestSeries;
        const SensorConstraints* sensorConstraints; 
        const SensorConstData* constData;
        //Methods
        TestSeriesCheckResult (*takeTestSeries)(const Sensor* con);
        double (*getAverageMeanOfSeries)(const Sensor* con);
        MinMax (*evaluateMinMaxOfTestSeries)(const Sensor* con);
        unsigned int (*getSize)(const Sensor*);
        unsigned char (*getID)(const Sensor*);
        unsigned char (*getPin)(const Sensor*);
        //Abstract methods
        void (*readSensorValue)(double*);
        TestSeriesCheckResult(*checkTestSeries)(const Sensor*);
        void (*initSensorHW)(const Sensor* con);
    };

    bool construct(Sensor* con, const SensorConstData* constDa, const SensorConstraints* constraint, unsigned int testSeriesSize);
}
#endif
