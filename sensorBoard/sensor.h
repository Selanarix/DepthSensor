
#ifndef SENSOR_H_
#define SENSOR_H_


namespace Sensor 
{
    typedef enum 
    {
        SensorValueOK,
        TestSeriesError,
        SensorValueUnexpected
    } SensorMeasurmentResult;

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
    } AverageMeasurementTestResult;
}

#endif
