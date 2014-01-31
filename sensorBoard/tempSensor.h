
#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_
#include "sensor.h"

namespace TemperatureSensor //extends Sensor
{
    typedef unsigned int Temperature;

    typedef enum
    {
        LM35, //TODO
    } TemperaturSensorType;

    typedef struct _TemperaturSensor TemperaturSensor;

    struct _TemperaturSensor
    {
        //base class
        //Data
        unsigned int sensorUsedSizeOfTestSeries;
        const Sensor::SensorConstraints* sensorConstraints; 
        const Sensor::SensorConstData* constData;
        //Methods
        Sensor::TestSeriesCheckResult (*takeTestSeries)(const Sensor::Sensor* con);
        double (*getAverageMeanOfSeries)(const Sensor::Sensor* con);
        Sensor::MinMax (*evaluateMinMaxOfTestSeries)(const Sensor::Sensor* con);
        unsigned int (*getSize)(const Sensor::Sensor*);
        unsigned char (*getID)(const Sensor::Sensor*);
        unsigned char (*getPin)(const Sensor::Sensor*);
        //Abstract methods
        void (*readSensorValue)(double*);
        Sensor::TestSeriesCheckResult(*checkTestSeries)(const Sensor::Sensor*);
        void (*initSensorHW)(const Sensor::Sensor* con);
        //extend base class
        //Data
        Temperature lastTemperature;
        //Methods
        Temperature (*getTemperatur)(const TemperaturSensor*); 
        Sensor::MeasurmentResult (*measureTemperature)(TemperaturSensor* con);
    };
    bool construct(TemperaturSensor* con, const Sensor::SensorConstData* constDa, const Sensor::SensorConstraints* constraint, unsigned int testSeriesSize, TemperaturSensorType t);
}

#endif /* DEPTHSENSOR_H_ */
