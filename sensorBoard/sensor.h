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
        const uint8_t PIN;
        const uint8_t ID;
    } SensorConstData;

    typedef enum
    {
        MeasurementOK,
        MeasurementValueUnexpected,
        MeasurementError
    } MeasurementResult;
}
#endif
