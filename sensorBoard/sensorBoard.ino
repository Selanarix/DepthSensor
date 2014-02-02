#include "led.h"
#include "tempSensor.h"
#include "depthSensor.h"
//#include "network.h"
#include "logger.h"
//#include "hal.h"
#include "sensor.h"

// Set up temperatur sensor objects and their data
//  ----------------------------------------------------------------------
Sensor::SensorConstraints temperatureConstrain = 
{
        -10.0,       // MINIMAL_EXPECTED_SENSOR_VALUE;
        60.0,        // MAXIMAL_EXPECTED_SENSOR_VALUE;
        2.0,         // ALLOWED_TEST_SERIES_VARIATION;
        10.0         // MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
};

Sensor::TestSeriesControll tempControll = 
{
        5,         // MAXIMAL_MEASUREMENT_RETRIES
        1000,      // DELAY_FOR_RETRY_IF_SERIES_ERROR
        500        // DELAY_BETWEEN_MEASUREMENTS_ms
};

Sensor::SensorConstData temperatureConst = 
{
        &tempControll, //Test series controll struct
        0,             //PIN
        5,             //ID
};
// Set up depth sensor objects and their data
//  ----------------------------------------------------------------------
Sensor::SensorConstraints depthConstrain = 
{
        0.0,             // MINIMAL_EXPECTED_SENSOR_VALUE;
        1000.0,          // MAXIMAL_EXPECTED_SENSOR_VALUE;
        2.0,             // ALLOWED_TEST_SERIES_VARIATION;
        20.0             // MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
};

Sensor::TestSeriesControll depthControll = 
{
        10,            // MAXIMAL_MEASUREMENT_RETRIES
        5000,          // DELAY_FOR_RETRY_IF_SERIES_ERROR
        1000           // DELAY_BETWEEN_MEASUREMENTS_ms
};

Sensor::SensorConstData depthSensorConst = 
{
        &depthControll, //Test series controll struct
        1,              //PIN
        0,              //ID
};

TemperatureSensor::TemperaturSensor temperatureSensor1;
DepthSensor::DepthSensor depthSensor1;

void setup()
{    
    Logger::initLogger();
   // HAL::initBaseHW();
   // ProjectLED::initLedPins();
    if(!TemperatureSensor::construct(&temperatureSensor1, &temperatureConst, &temperatureConstrain, 5, TemperatureSensor::LM35))
    //    Logger::log(Logger::ERROR,"Could not set up temperatur sensor");
    //temperatureSensor1.initSensorHW((Sensor::Sensor*)(&temperatureSensor1));

   // DepthSensor::construct(&depthSensor1, &depthSensorConst, &depthConstrain, 5, DepthSensor::MPX5500);
  //  depthSensor1.initSensorHW((Sensor::Sensor*)(&depthSensor1));


  //   Network::initNetworkStack();

    Logger::log(Logger::INFO,"System initialized");
}

void flashLED_1s()
{
    ProjectLED::LED_On(ProjectLED::LED0);
    delay(500);
    ProjectLED::LED_Off(ProjectLED::LED0);
    delay(500);
}

void loop()
{    
  
   // DepthSensor::Depth dep = DepthSensor::measureDepth();
 //   Sensor::MeasurmentResult tempRes = temperatureSensor1.measureTemperature(&temperatureSensor1);
 //   Sensor::MeasurmentResult depthRes = depthSensor1.measureDepth(&depthSensor1);
    
  //  if(depthRes == Sensor::MeasurmentOK )//&& depthRes == Sensor::MeasurmentOK)
  //  {
    //    TemperatureSensor::Temperature tmp1 = temperatureSensor1.getTemperature(&temperatureSensor1);
     //   DepthSensor::Depth dep1 = depthSensor1.getDepth(&depthSensor1);
        //Send depth sensor
       // Network::http_GET_Request(temperatureSensor1.getID((Sensor::Sensor*)&temperatureSensor1), tmp1);
        //Send temperatur sensor
      //  Network::http_GET_Request(depthSensor1.getID((Sensor::Sensor*)&depthSensor1), dep1); 
  //  }
    //flashLED_1s();
    Logger::log(Logger::INFO,"Test");
    delay(2000);
}
