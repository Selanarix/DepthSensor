#include "led.h"
#include "depthSensor.h"
#include "tempSensor.h"
#include "network.h"

void setup()
{
    Serial.begin(9600); 
  /*  ProjectLED::initLedPins();
    Network::initNetworkStack();
    DepthSensor::initDepthSensorHW();
    TemperaturSensor::initTemperaturSensorHW();
    */
}

void flashLED_1s()
{
    delay(300);
    ProjectLED::LED_On(ProjectLED::LED0);
    delay(400);
    ProjectLED::LED_Off(ProjectLED::LED0);
    delay(300);
}

void loop()
{    
  /*  ProjectLED::LED_On(ProjectLED::LED0);
    DepthSensor::depth dep = DepthSensor::measureDepth();
    TemperaturSensor::temperatur temp = TemperaturSensor::measureTemperatur();   
    ProjectLED::LED_Off(ProjectLED::LED0);
    
    flashLED_1s();
 
    ProjectLED::LED_On(ProjectLED::LED0);
    //Send depth sensor
    Network::http_GET_Request(0, dep);
    //Send temperatur sensor
    Network::http_GET_Request(5,temp);
    ProjectLED::LED_Off(ProjectLED::LED0);
    
    flashLED_1s();
    flashLED_1s();
    */
    pinMode(2, INPUT);
    
    int pressure = 0;
    pressure = analogRead(2);
    Serial.print("ADC: ");
    Serial.println(pressure);
    Serial.print("Sensor [mV]: ");
    Serial.println(5000.0/1024.0 * pressure / 125);


    delay(500);
}
