#include "led.h"
#include "tempSensor.h"
#include "testSeries.h"
#include "depthSensor.h"
#include "network.h"
#include "logger.h"
#include "hal.h"
#include "sensor.h"
#include "realTimeClock.h"


// Set up temperatur sensor objects and their data
// ----------------------------------------------------------------------
const Sensor::SensorConstraints temperatureConstrain =
{
        -10.0, // MINIMAL_EXPECTED_SENSOR_VALUE;
        60.0, // MAXIMAL_EXPECTED_SENSOR_VALUE;
        2.0, // ALLOWED_TEST_SERIES_VARIATION;
        10.0 // MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
};

const TestSeries::TestSeriesControll tempControll =
{
        5, // MAXIMAL_MEASUREMENT_RETRIES
        1000, // DELAY_FOR_RETRY_IF_SERIES_ERROR
        500 // DELAY_BETWEEN_MEASUREMENTS_ms
};

const Sensor::SensorConstData analogTemperatureConstData =
{
        0, //PIN
        5, //ID
};
const Sensor::SensorConstData digitalTemperatureConstData =
{
        0, //PIN
        5, //ID
};
// Set up depth sensor objects and their data
// ----------------------------------------------------------------------
const Sensor::SensorConstraints depthConstrain =
{
        0.0, // MINIMAL_EXPECTED_SENSOR_VALUE;
        1000.0, // MAXIMAL_EXPECTED_SENSOR_VALUE;
        2.0, // ALLOWED_TEST_SERIES_VARIATION;
        20.0 // MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
};

const TestSeries::TestSeriesControll depthControll =
{
        10, // MAXIMAL_MEASUREMENT_RETRIES
        5000, // DELAY_FOR_RETRY_IF_SERIES_ERROR
        1000 // DELAY_BETWEEN_MEASUREMENTS_ms
};

const Sensor::SensorConstData depthConst =
{
        2, //PIN
        0, //ID
};

TemperatureSensor::TemperatureSensor analogTemperature;
TemperatureSensor::TemperatureSensor digitalTemperature;
DepthSensor::DepthSensor depthSensor1;
TMP102::TMP102 tmp102;

void setup()
{
    ProjectLED::initLedPins();
    ProjectLED::LED_On(ProjectLED::LED0); //Indicate init procedure on board
    
    Logger::initLogger();
    Logger::changeOutputLogLevel(Logger::DEBUG);
    
    HAL::initBaseHW();
    if(!TemperatureSensor::construct(&analogTemperature, &analogTemperatureConstData, &tempControll, &temperatureConstrain, TemperatureSensor::LM35, 5))
       Logger::log(Logger::ERROR,F("Could not set up temperatur sensor"));
       
    //Set upt digital Sensor
    if(!TMP102::construct(&tmp102,TMP102::GND))
       Logger::log(Logger::ERROR,F("Could not set up tmp102"));  
   
    if(!TemperatureSensor::constructDigital(&digitalTemperature, &digitalTemperatureConstData, &tempControll, &temperatureConstrain, TemperatureSensor::TMP102, 5, &tmp102))
       Logger::log(Logger::ERROR,F("Could not set up digital temperature sensor"));
    
    if(!DepthSensor::construct(&depthSensor1, &depthConst, &depthControll, &depthConstrain, DepthSensor::MPX5500, 10))
      Logger::log(Logger::ERROR,F("Could not set up depth sensor"));
       
    setUpRealTimeClock();
    //Network::initNetworkStack();
    Logger::log(Logger::INFO, F("System initialized"));
    ProjectLED::LED_Off(ProjectLED::LED0); //Indicate end of init on board
}

void flashLED_1s()
{
    ProjectLED::LED_On(ProjectLED::LED0);
    delay(500);
    ProjectLED::LED_Off(ProjectLED::LED0);
    delay(500);
}

void cycleTask()
{
   
    Sensor::MeasurementResult tempRes = TemperatureSensor::measureTemperature(&analogTemperature);
    Sensor::MeasurementResult analogtempRes = TemperatureSensor::measureTemperature(&digitalTemperature);
    Sensor::MeasurementResult depthRes = DepthSensor::measureDepth(&depthSensor1);
    
    if(depthRes == Sensor::MeasurementOK && depthRes == Sensor::MeasurementOK)
    {
       TemperatureSensor::Temperature tmp1 = TemperatureSensor::getLastTemperature(&analogTemperature);
       DepthSensor::Depth dep1 = DepthSensor::getLastDepth(&depthSensor1);
        //Send depth sensor
       // Network::http_GET_Request(analogTemperature.getID((Sensor::Sensor*)&analogTemperature), tmp1);
        //Send temperatur sensor
      // Network::http_GET_Request(depthSensor1.getID((Sensor::Sensor*)&depthSensor1), dep1);
    }
    flashLED_1s();
}

void loop()
{
    if(isRTC_FlagAndClear())
    {
        cycleTask();
        delay(100); //give HW some time to process last commands from task

        enterSleep();
    }
    else
    {
        Logger::log(Logger::ERROR,F("No power saving mode entered"));
    }
}
