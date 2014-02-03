#ifndef SENSOR_H_
#define SENSOR_H_

namespace Sensor
{
    typedef struct
    {
        const double MINIMAL_EXPECTED_SENSOR_VALUE;
        const double MAXIMAL_EXPECTED_SENSOR_VALUE;
        const double ALLOWED_TEST_SERIES_VARIATION;
        const double MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
    } SensorConstraints;

    typedef struct
    {
        const unsigned char PIN;
        const unsigned char ID;
    } SensorConstData;

    typedef enum
    {
        MeasurementOK,
        MeasurementValueUnexpected,
        MeasurementError
    } MeasurementResult;
}
#endif
