#include "led.h"
#include "tempSensor.h"
//#include "network.h"
#include "logger.h"
#include "hal.h"
#include "sensor.h"

Sensor::SensorConstraints senConstraint = 
{
        20,       // MINIMAL_EXPECTED_SENSOR_VALUE;
        30,   // MAXIMAL_EXPECTED_SENSOR_VALUE;
        10,   // ALLOWED_TEST_SERIES_VARIATION;
        3    // MAX_ALLOWED_AVERAGED_VALUE_CHANGE;
};

Sensor::TestSeriesControll controll = 
{
        2,// MAXIMAL_MEASUREMENT_RETRIES
        5000,// DELAY_FOR_RETRY_ms
        300// DELAY_BETWEEN_MEASUREMENTS_ms
};

Sensor::SensorConstData constDa = 
{
        &controll, //Test series controll struct
        0, //PIN
        5, //ID
};

TemperatureSensor::TemperaturSensor sen1;

void setup()
{    
    Logger::initLogger();
    HAL::initBaseHW();
    ProjectLED::initLedPins();

    TemperatureSensor::construct(&sen1, &constDa, &senConstraint,5, TemperatureSensor::LM35);
    sen1.initSensorHW((Sensor::Sensor*)&sen1);

  //   Network::initNetworkStack();
  // DepthSensor::initDepthSensorHW();
  // TemperatureSensor::initTemperatureSensorHW();
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
    using namespace TemperatureSensor;
   // DepthSensor::Depth dep = DepthSensor::measureDepth();
    Sensor::MeasurmentResult res = sen1.measureTemperature(&sen1);
    if(res == Sensor::MeasurmentOK)
         Temperature a = sen1.getTemperatur(&sen1);
    else
         Logger::log(Logger::ERROR,"Could not measure");
    
  //   flashLED_1s();
  
    
  /*
    //Send depth sensor
    Network::http_GET_Request(0, dep);
    //Send temperatur sensor
    Network::http_GET_Request(5,temp);
    
    flashLED_1s();
    flashLED_1s();
  if(ADC_TLC_243::testTLC243(ADC_TLC_243::V_Diff) == ADC_TLC_243::ADCOutOfFunction)
    Logger::log(Logger::ERROR,"adc not working");
  else
    Logger::log(Logger::INFO,"workds :-)");
    
 */
//   Serial.println(ADC_TLC_243::analogReadPrepareNext(ADC_TLC_243::Channel0));
  
   /* if(ADC_TLC_243::testTLC243(ADC_TLC_243::V_Plus) == ADC_TLC_243::ADCOutOfFunction)
    Logger::log(Logger::ERROR,"adc not working");
  else
    Logger::log(Logger::INFO,"workds :-)");
*/
    delay(2000);
}
