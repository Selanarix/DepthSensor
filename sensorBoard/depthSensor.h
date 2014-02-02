
#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_
#include "sensor.h"

namespace DepthSensor 
{
    typedef unsigned int Depth;

    typedef enum
    {
        MPX5500,
    } DepthSensorType;
    
    typedef struct _DepthSensor DepthSensor;

    struct _DepthSensor
    {
        //base class
        //Data
        unsigned int sensorUsedSizeOfTestSeries;
        const Sensor::SensorConstraints* sensorConstraints; 
        const Sensor::SensorConstData* constData;
        //Methods
        Sensor::TestSeriesCheckResult (*takeTestSeries)(const DepthSensor* con);
        double (*getAverageMeanOfSeries)(const DepthSensor* con);
        Sensor::MinMax (*evaluateMinMaxOfTestSeries)(const DepthSensor* con);
        unsigned int (*getSize)(const DepthSensor*);
        unsigned char (*getID)(const DepthSensor*);
        unsigned char (*getPin)(const DepthSensor*);
        //Abstract methods
        void (*readSensorValue)(double*);
        Sensor::TestSeriesCheckResult(*checkTestSeries)(const Sensor::Sensor*);
        void (*initSensorHW)(const Sensor::Sensor* con);
        //extend base class
        //Data
        Depth lastDepth;
        //Methods
        Depth (*getDepth)(const DepthSensor*); 
        Sensor::MeasurmentResult (*measureDepth)(DepthSensor* con);
    };
    bool construct(DepthSensor* con, const Sensor::SensorConstData* constDa, const Sensor::SensorConstraints* constraint, unsigned int testSeriesSize,  DepthSensorType t);
}
#endif /* DEPTHSENSOR_H_ */
