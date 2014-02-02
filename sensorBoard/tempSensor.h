
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
        Sensor::TestSeriesCheckResult (*takeTestSeries)(const TemperaturSensor* con);
        double (*getAverageMeanOfSeries)(const TemperaturSensor* con);
        Sensor::MinMax (*evaluateMinMaxOfTestSeries)(const TemperaturSensor* con);
        unsigned int (*getSize)(const TemperaturSensor*);
        unsigned char (*getID)(const TemperaturSensor*);
        unsigned char (*getPin)(const TemperaturSensor*);
        //Abstract methods
        void (*readSensorValue)(double*);
        Sensor::TestSeriesCheckResult(*checkTestSeries)(const TemperaturSensor*);
        void (*initSensorHW)(const TemperaturSensor*);
        //extend base class
        //Data
        Temperature lastTemperature;
        //Methods
        Temperature (*getTemperature)(const TemperaturSensor*); 
        Sensor::MeasurmentResult (*measureTemperature)(TemperaturSensor* con);
    };
    bool construct(TemperaturSensor* con, const Sensor::SensorConstData* constDa, const Sensor::SensorConstraints* constraint, unsigned int testSeriesSize, TemperaturSensorType t);
}

#endif /* DEPTHSENSOR_H_ */
