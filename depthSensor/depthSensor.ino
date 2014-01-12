#include <SPI.h>
#include <Ethernet.h>

namespace sensor 
{
  #include "depthSensor.c"
}

void setup()
{
    Serial.begin(9600);  
    sensor::initHW();
}

void loop()
{
    sensor::handleLAN();
    sensor::processData(sensor::measureDepth());
    Serial.println("Ende Messung...............");

            
  delay(1000);
}
