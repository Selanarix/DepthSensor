#include "led.h"
#include "tempSensor.h"
#include "depthSensor.h"
#include "network.h"
#include "logger.h"
#include "hal.h"
#include "sensor.h"
#include "realTimeClock.h"
#include "displayControl.h"

static bool initialized = false;

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
        10, //ID
};
// Set up depth sensor objects and their data
// ----------------------------------------------------------------------
const Sensor::SensorConstraints depthConstrain =
{
        -2.0, // MINIMAL_EXPECTED_SENSOR_VALUE;
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
        1, //PIN
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
    
    DisplayControl::setUpDisplay();
    
    Logger::initLogger();
    Logger::changeOutputLogLevel(Logger::DEBUG);
        
    if(!HAL::initBaseHW())
    {
        Logger::log(Logger::ERROR,F("Could not set up HAL successfully"));
        return;
    }
    if(!TemperatureSensor::construct(&analogTemperature, &analogTemperatureConstData, &tempControll, &temperatureConstrain, TemperatureSensor::LM35, 5))
    {
        Logger::log(Logger::ERROR,F("Could not set up temperatur sensor"));
        return;
    }
    //Set up Sensors
    if(!TMP102::construct(&tmp102,TMP102::GND))
    {
        Logger::log(Logger::ERROR,F("Could not set up tmp102"));  
        return;
    } 
    if(!TemperatureSensor::constructDigital(&digitalTemperature, &digitalTemperatureConstData, &tempControll, &temperatureConstrain, TemperatureSensor::TMP102, 5, &tmp102))
    {
        Logger::log(Logger::ERROR,F("Could not set up digital temperature sensor"));
        return;
    }  
    if(!DepthSensor::construct(&depthSensor1, &depthConst, &depthControll, &depthConstrain, DepthSensor::MPX5100, 10))
    {
        Logger::log(Logger::ERROR,F("Could not set up depth sensor"));
        return;
    }    
    
   
    setUpRealTimeClock();
    //Network::initNetworkStack();
    Logger::log(Logger::INFO, F("System initialized"));
    ProjectLED::LED_Off(ProjectLED::LED0); //Indicate end of init on board
    initialized = true;
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
      DisplayControl::showIndicator(DisplayControl::MEASUREMENT);
      Sensor::MeasurementResult tempRes = TemperatureSensor::measureTemperature(&analogTemperature);
      Sensor::MeasurementResult analogtempRes = TemperatureSensor::measureTemperature(&digitalTemperature);
      Sensor::MeasurementResult depthRes = DepthSensor::measureDepth(&depthSensor1);
      DisplayControl::hideIndicator(DisplayControl::MEASUREMENT);
      
      TemperatureSensor::Temperature digiTemp= 0;
      TemperatureSensor::Temperature analogTemp= 0;
      DepthSensor::Depth depth = 0;
      
      if(tempRes == Sensor::MeasurementOK)
          digiTemp = TemperatureSensor::getLastTemperature(&digitalTemperature);
      
      if(analogtempRes == Sensor::MeasurementOK)
          analogTemp = TemperatureSensor::getLastTemperature(&analogTemperature);
      
      if(depthRes == Sensor::MeasurementOK)
          depth = DepthSensor::getLastDepth(&depthSensor1);
     
     Sensor::SensorStringInformation depthInfo = DepthSensor::sensorGetStringInfo();
     Sensor::SensorStringInformation tempInfo = TemperatureSensor::sensorGetStringInfo();
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space0, &depthInfo, depth);
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space1, &tempInfo, digiTemp);
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space2, &tempInfo, analogTemp);
    
     DisplayControl::showIndicator(DisplayControl::NETWORK);
     delay(1000);
     DisplayControl::hideIndicator(DisplayControl::NETWORK);
/*    if(depthRes == Sensor::MeasurementOK && depthRes == Sensor::MeasurementOK)
    {
       TemperatureSensor::Temperature tmp1 = TemperatureSensor::getLastTemperature(&analogTemperature);
       DepthSensor::Depth dep1 = DepthSensor::getLastDepth(&depthSensor1);
        //Send depth sensor
       // Network::http_GET_Request(analogTemperature.getID((Sensor::Sensor*)&analogTemperature), tmp1);
        //Send temperatur sensor
      // Network::http_GET_Request(depthSensor1.getID((Sensor::Sensor*)&depthSensor1), dep1);
    }
    */
    flashLED_1s(); 
}

void loop()
{
    if(!initialized)
    {
        Logger::log(Logger::ERROR, F("System error! Going to sleep for eternity"));
        delay(100);
        enterSleep(); //Sleep will never wake up here!
        return;
    }
    
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
