#ifndef SENSOR_ERROR_H
#define SENSOR_ERROR_H

namespace SensorError
{

    typedef enum
    {
        AverageMeasurmentOK,
        AverageMeasurmentNotInRange
    } AverageMeasurementError;

    typedef enum
    {
        TestSeriesOK,
        SensorOutOfFunction,
        TestSeriesUnderMinRange,
        TestSeriesAboveMaxRange,
        TestSeriesMeanVariationToBig
    } TestSeriesError;

}

#endif
