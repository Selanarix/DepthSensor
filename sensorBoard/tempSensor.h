#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "sensor.h"
#include "testSeries.h"
#include "TMP_102.h"

namespace TemperatureSensor 
{
    typedef int32_t Temperature;

    typedef enum
    {
        LM35, 
        TMP102,
    } TemperatureSensorType;

    typedef struct _TemperatureSensor TemperatureSensor;

    struct _TemperatureSensor
    {
        Temperature lastTemperature;
        const Sensor::SensorConstraints* constrains;
        const Sensor::SensorConstData* constData;
        TestSeries::TestSeries series;
        TMP102::TMP102* digitalSensor;
    };

    bool construct(TemperatureSensor*, 
                    const Sensor::SensorConstData*, 
                    const TestSeries::TestSeriesControll*,
                    const Sensor::SensorConstraints*, 
                    TemperatureSensorType , uint32_t );
                    
    bool constructDigital(TemperatureSensor* tSen, 
                    const Sensor::SensorConstData* constDa, 
                    const TestSeries::TestSeriesControll* controll,
                    const Sensor::SensorConstraints* cons, 
                    TemperatureSensorType t, uint32_t size, TMP102::TMP102* sen);
                    
    Sensor::MeasurementResult measureTemperature(TemperatureSensor* con);   
    Temperature getLastTemperature(const TemperatureSensor* con);
    void initTemperatureSensorHW(const TemperatureSensor* con);
}

#endif /* DEPTHSENSOR_H_ */
