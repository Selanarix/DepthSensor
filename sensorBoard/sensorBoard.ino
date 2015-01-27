#include "led.h"
#include "tempSensor.h"
#include "depthSensor.h"
//#include "network.h"
#include "logger.h"
#include "hal.h"
#include "sensor.h"
#include "realTimeClock.h"
#include "displayControl.h"

#define ONBOARD_DISPLAY 1

static boolean initialized = false;

// Set up temperatur sensor objects and their data
// ----------------------------------------------------------------------
/*const Sensor::SensorConstraints temperatureConstrain =
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
        200 // DELAY_BETWEEN_MEASUREMENTS_ms
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
};*/

TemperatureSensor::TemperatureSensor analogTemperature;
TemperatureSensor::TemperatureSensor digitalTemperature;
DepthSensor::DepthSensor depthSensor1;
TMP102::TMP102 tmp102;

void setup()
{
    ProjectLED::initLedPins();
    ProjectLED::LED_On(ProjectLED::LED0); //Indicate init procedure on board
    
#if ONBOARD_DISPLAY == 1
    DisplayControl::setUpDisplay();
#endif

    Logger::initLogger();
    Logger::changeOutputLogLevel(Logger::DEBUG);
        
 /*   if(!HAL::initBaseHW())
    {
        Logger::log(Logger::ERROR,F("Could not set up HAL successfully"));
        return;
    }
    //Set up Sensors
    if(!TemperatureSensor::construct(&analogTemperature, &analogTemperatureConstData, &tempControll, &temperatureConstrain, TemperatureSensor::LM35, 5))
    {
        Logger::log(Logger::ERROR,F("Could not set up temperatur sensor"));
        return;
    }
    
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
    }    */
    setUpRealTimeClock();
    //Network::initNetworkStack();
    Logger::log(Logger::INFO, F("System initialized"));
    ProjectLED::LED_Off(ProjectLED::LED0); //Indicate end of init on board
    initialized = true;
}

void showMeasurementDisplayView(Sensor::MeasurementResult tempRes, Sensor::MeasurementResult analogtempRes, Sensor::MeasurementResult depthRes,
                                TemperatureSensor::Temperature digiTemp, TemperatureSensor::Temperature analogTemp, DepthSensor::Depth depth)
{     
     Sensor::SensorStringInformation depthInfo = DepthSensor::sensorGetStringInfo();
     Sensor::SensorStringInformation tempInfo = TemperatureSensor::sensorGetStringInfo();
     
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space0, &depthInfo, depth);
     if(depthRes != Sensor::MeasurementOK)
         DisplayControl::setUpMeasurementWarning(DisplayControl::Space0);
     else
         DisplayControl::clearMeasurmentIcon(DisplayControl::Space0); 
     
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space1, &tempInfo, digiTemp);
     if(tempRes != Sensor::MeasurementOK)
         DisplayControl::setUpMeasurementWarning(DisplayControl::Space1);
     else
         DisplayControl::clearMeasurmentIcon(DisplayControl::Space1);
         
     DisplayControl::updatedepthMeasurementOutputSpace(DisplayControl::Space2, &tempInfo, analogTemp);
     if(analogtempRes != Sensor::MeasurementOK)
         DisplayControl::setUpMeasurementWarning(DisplayControl::Space2);
     else
         DisplayControl::clearMeasurmentIcon(DisplayControl::Space2);
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
    //Only update system if waiting period is over already
    if(!isTimePeriodOver(32))
        return;
 
#if ONBOARD_DISPLAY == 1 
      DisplayControl::showIndicator(DisplayControl::MEASUREMENT);
#endif
    ProjectLED::LED_On(ProjectLED::LED0); //Measure LED
    //Measure sensors
//    Sensor::MeasurementResult analogtempRes = TemperatureSensor::measureTemperature(&analogTemperature);
//    Sensor::MeasurementResult digitaltempRes = TemperatureSensor::measureTemperature(&digitalTemperature);
//    Sensor::MeasurementResult depthRes = DepthSensor::measureDepth(&depthSensor1);
    ProjectLED::LED_Off(ProjectLED::LED0); //Measure LED
#if ONBOARD_DISPLAY == 1       
      DisplayControl::hideIndicator(DisplayControl::MEASUREMENT);
#endif


    //Update sensor measurement results     
    TemperatureSensor::Temperature digiTemp= 0;
    TemperatureSensor::Temperature analogTemp= 0;
    DepthSensor::Depth depth = 0;
    
/*    if(digitaltempRes == Sensor::MeasurementOK)
        digiTemp = TemperatureSensor::getLastTemperature(&digitalTemperature);
    
    if(analogtempRes == Sensor::MeasurementOK)
        analogTemp = TemperatureSensor::getLastTemperature(&analogTemperature);
    
    if(depthRes == Sensor::MeasurementOK)
        depth = DepthSensor::getLastDepth(&depthSensor1); */
    
     //Update display
#if ONBOARD_DISPLAY == 1
    //showMeasurementDisplayView(digitaltempRes, analogtempRes, depthRes, digiTemp, analogTemp, depth);
    if(DisplayControl::isNewLogInfo())
    {
         DisplayControl::showLogMessageScreen();
         delay(4000);
         DisplayControl::prepareStaticDisplay();
        // showMeasurementDisplayView(digitaltempRes, analogtempRes, depthRes, digiTemp, analogTemp, depth);
    }
#endif

#if ONBOARD_DISPLAY == 1 
     DisplayControl::showIndicator(DisplayControl::NETWORK);
     delay(1000);
     // Network::http_GET_Request(DepthSensor::getSensorID(&depthSensor1), depth);    
     DisplayControl::hideIndicator(DisplayControl::NETWORK);
#endif
   
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
