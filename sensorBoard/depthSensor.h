
#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

#include "sensor.h"
#include "testSeries.h"

namespace DepthSensor 
{
    typedef uint32_t Depth;

    typedef enum
    {
        MPX5500,
    } DepthSensorType;
    
    typedef struct _DepthSensor DepthSensor;

    struct _DepthSensor
    {
        Depth lastDepth;
        const Sensor::SensorConstraints* constrains;
        const Sensor::SensorConstData* constData;
        TestSeries::TestSeries series;
    };
    
    bool construct(DepthSensor*, 
                    const Sensor::SensorConstData*, 
                    const TestSeries::TestSeriesControll*,
                    const Sensor::SensorConstraints*, 
                    DepthSensorType , uint32_t );

    Sensor::MeasurementResult measureDepth(DepthSensor* con);   
    Depth getLastDepth(const DepthSensor* con);
    void initSensorHW(const DepthSensor* con);
}
#endif /* DEPTHSENSOR_H_ */
