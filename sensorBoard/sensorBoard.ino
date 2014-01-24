#include "led.h"
//#include "depthSensor.h"
//#include "tempSensor.h"
//#include "network.h"
#include "logger.h"
#include "ADC_tlc243.h"

void setup()
{    
  Logger::initLogger();
  //Serial.println(ADC_TLC_243::initAndPreorderConversion(ADC_TLC_243::Channel0, ADC_TLC_243::ADC_MSB, ADC_TLC_243::Unipolar, ADC_TLC_243::Bit16));
  //  ProjectLED::initLedPins();
 //   Network::initNetworkStack();
  //  DepthSensor::initDepthSensorHW();
  //  TemperaturSensor::initTemperaturSensorHW();
 
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
   // DepthSensor::depth dep = DepthSensor::measureDepth();
 /*   TemperaturSensor::temperatur temp = TemperaturSensor::measureTemperatur();   
    
    flashLED_1s();
 
    //Send depth sensor
    Network::http_GET_Request(0, dep);
    //Send temperatur sensor
    Network::http_GET_Request(5,temp);
    
    flashLED_1s();
    flashLED_1s();
    
 */
   
  if(ADC_TLC_243::testTLC243(ADC_TLC_243::V_Plus) == ADC_TLC_243::ADCOutOfFunction)
    Logger::log(Logger::ERROR,"adc not working");
  else
    Logger::log(Logger::INFO,"workds :-)");
  

    delay(10);
}
