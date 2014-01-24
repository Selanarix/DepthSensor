#include "led.h"
#include "depthSensor.h"
#include "tempSensor.h"
//#include "network.h"
#include "logger.h"
//#include "ADC_tlc243.h"

void setup()
{    
   Logger::initLogger();
   ProjectLED::initLedPins();
 //   Network::initNetworkStack();
   DepthSensor::initDepthSensorHW();
   TemperatureSensor::initTemperatureSensorHW();
 
    Logger::log(Logger::INFO,"System initialized");
  delay(10);
    
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
    DepthSensor::Depth dep = DepthSensor::measureDepth();
   TemperatureSensor::Temperature temp = TemperatureSensor::measureTemperature();   
    
     flashLED_1s();
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
