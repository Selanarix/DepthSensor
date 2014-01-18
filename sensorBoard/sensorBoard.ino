#include "led.h"
//#include "depthSensor.h"
//#include "tempSensor.h"
//#include "network.h"
#include "logger.h"
#include <SPI.h>

void setup()
{
    Logger::initLogger();
  /*  ProjectLED::initLedPins();
    Network::initNetworkStack();
    DepthSensor::initDepthSensorHW();
    TemperaturSensor::initTemperaturSensorHW();
    */
    pinMode(9,OUTPUT);
    
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    digitalWrite(9,LOW);
    SPI.transfer(0x02);
    digitalWrite(9,HIGH);
    
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
/*    
    DepthSensor::depth dep = DepthSensor::measureDepth();
    TemperaturSensor::temperatur temp = TemperaturSensor::measureTemperatur();   
    
    flashLED_1s();
 
    //Send depth sensor
    Network::http_GET_Request(0, dep);
    //Send temperatur sensor
    Network::http_GET_Request(5,temp);
    
    flashLED_1s();
    flashLED_1s();
 */   
 /*
    pinMode(2, INPUT);
    
    int pressure = 0;
    pressure = analogRead(2);
    Serial.print("ADC: ");
    Serial.println(pressure);
    Serial.print("Sensor [mV]: ");
    Serial.println(5000.0/1024.0 * pressure / 125);
*/

  uint32_t result=0;
  
  digitalWrite(9,LOW);
  result=SPI.transfer(0xD4);
  digitalWrite(9,HIGH);
  Serial.println(result);
    delay(10);
}
