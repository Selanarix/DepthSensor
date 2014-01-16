#include "tempSensor.h"
#include "myUtil.h"

namespace TemperaturSensor 
{
    //-------------------------- Private Types -------------------------------------
      

    //-------------------- Private Function Prototypes -----------------------------


    //------------------------- Private Data ---------------------------------------

    
   

    //------------------------ Read only ------------------------------------------

    const int tempSensorPin = 1;

    //------------------------------- Public Functions -----------------------------
 
    void initTemperaturSensorHW()
    {
        pinMode(tempSensorPin, INPUT);
    }

    temperatur measureTemperatur()
    {
        double tempvalue = (double)analogRead(tempSensorPin);
        Serial.print("Temp [C]: ");
        Serial.println(5000.0 /1024.0 * tempvalue / 10.0);
        return  (temperatur)(5000.0 /1024.0 * tempvalue / 10.0);
    }
    
}