#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "sensor.h"
#include "testSeries.h"

namespace TemperatureSensor 
{
    typedef int32_t Temperature;

    typedef enum
    {
        LM35, //TODO
    } TemperatureSensorType;

    typedef struct _TemperatureSensor TemperatureSensor;

    struct _TemperatureSensor
    {
        Temperature lastTemperature;
        const Sensor::SensorConstraints* constrains;
        const Sensor::SensorConstData* constData;
        TestSeries::TestSeries series;
    };

    bool construct(TemperatureSensor*, 
                    const Sensor::SensorConstData*, 
                    const TestSeries::TestSeriesControll*,
                    const Sensor::SensorConstraints*, 
                    TemperatureSensorType , uint32_t );

    Sensor::MeasurementResult measureTemperature(TemperatureSensor* con);   
    Temperature getLastMeasurement(const TemperatureSensor* con);
    void initADC_PIN(const TemperatureSensor* con);
}

#endif /* DEPTHSENSOR_H_ */
