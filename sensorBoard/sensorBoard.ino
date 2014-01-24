#include "led.h"
//#include "depthSensor.h"
//#include "tempSensor.h"
//#include "network.h"
#include "logger.h"
#include "ADC_tlc243.h"
#include <SPI.h>

void setup()
{
<<<<<<< HEAD
    Logger::initLogger();
    ProjectLED::initLedPins();
   // Network::initNetworkStack();
    DepthSensor::initDepthSensorHW();
    TemperaturSensor::initTemperaturSensorHW();
   
=======
    
  Logger::initLogger();
  //Serial.println(ADC_TLC_243::initAndPreorderConversion(ADC_TLC_243::Channel0, ADC_TLC_243::ADC_MSB, ADC_TLC_243::Unipolar, ADC_TLC_243::Bit16));
  //  ProjectLED::initLedPins();
 //   Network::initNetworkStack();
  //  DepthSensor::initDepthSensorHW();
  //  TemperaturSensor::initTemperaturSensorHW();
 
    
>>>>>>> e59e697801434cdfe8f3f596e351ecb83873d4e5
    Logger::log(Logger::INFO,"System initialized");
delay(10);
    
}
<<<<<<< HEAD
/*
void initADC()
{
    pinMode(9,OUTPUT);
    
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.begin();

    digitalWrite(9,LOW);
    SPI.transfer(0x0C);
    SPI.transfer(0x00);
    digitalWrite(9,HIGH);
}*/
=======

>>>>>>> e59e697801434cdfe8f3f596e351ecb83873d4e5

void flashLED_1s()
{
    ProjectLED::LED_On(ProjectLED::LED0);
    delay(500);
    ProjectLED::LED_Off(ProjectLED::LED0);
    delay(500);
}

void loop()
{    
    DepthSensor::depth dep = DepthSensor::measureDepth();
 /*   TemperaturSensor::temperatur temp = TemperaturSensor::measureTemperatur();   
    
    flashLED_1s();
 
    //Send depth sensor
    Network::http_GET_Request(0, dep);
    //Send temperatur sensor
    Network::http_GET_Request(5,temp);
    
    flashLED_1s();
    flashLED_1s();
    
 /*
    pinMode(2, INPUT);
    
    int pressure = 0;
    pressure = analogRead(2);
    Serial.print("ADC: ");
    Serial.println(pressure);
    Serial.print("Sensor [mV]: ");
    Serial.println(5000.0/1024.0 * pressure / 125);

<<<<<<< HEAD
/*
  uint32_t resultMsb=0, resultLsb=0;
=======
>>>>>>> e59e697801434cdfe8f3f596e351ecb83873d4e5
  uint32_t result=0;

  if(ADC_TLC_243::testTLC243(ADC_TLC_243::V_Plus) == ADC_TLC_243::ADCOutOfFunction)
    Logger::log(Logger::ERROR,"adc not working");
  else
    Logger::log(Logger::INFO,"workds :-)");
  

<<<<<<< HEAD
  resultMsb=SPI.transfer(0x0C);
  resultLsb=SPI.transfer(0x00);
  digitalWrite(9,HIGH);
  result=( resultMsb<<8 | resultLsb );
  result=result>>4;
  Serial.println(result);
  Serial.println(5000.0 /4096.0 * result / 10.0);
    delay(10); */
=======
//  result = ADC_TLC_243::analogReadPrepareNext(ADC_TLC_243::Channel1);
  //MSerial.println(result);
    delay(10);
>>>>>>> e59e697801434cdfe8f3f596e351ecb83873d4e5
}
