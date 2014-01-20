#include "led.h"
//#include "depthSensor.h"
//#include "tempSensor.h"
#include "network.h"
#include "logger.h"
#include <SPI.h>

void setup()
{
    Logger::initLogger();
    initADC();
  //  ProjectLED::initLedPins();
 //   Network::initNetworkStack();
  //  DepthSensor::initDepthSensorHW();
  //  TemperaturSensor::initTemperaturSensorHW();
 
    
    Logger::log(Logger::INFO,"System initialized");
    
    
}

void initADC()
{
/* Init Pin an SPI Bus*/
    pinMode(9,OUTPUT);
    
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.begin();

/*Make first Conversion to kick first malformed value*/
    digitalWrite(9,LOW);
    SPI.transfer(0x0C);
    SPI.transfer(0x00);
    digitalWrite(9,HIGH);
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

  uint32_t resultMsb=0, resultLsb=0;
  uint32_t result=0;
  
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  digitalWrite(9,LOW);

  resultMsb=SPI.transfer(0x0C);
  resultLsb=SPI.transfer(0x00);
  digitalWrite(9,HIGH);
  result=( resultMsb<<8 | resultLsb );
  result=result>>4;
  Serial.println(result);
  Serial.println(5000.0 /4096.0 * result / 10.0);
    delay(10);
}
