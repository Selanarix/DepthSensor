#include "tempSensor.h"
#include "logger.h"

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
        Logger::log(Logger::INFO, "temperatur sensor initialized");
    }

    temperatur measureTemperatur()
    {
        double tempvalue = (double)analogRead(tempSensorPin);
        Logger::logDouble(Logger::INFO, "Temp [C]: ",(5000.0 /1024.0 * tempvalue / 10.0));
        return  (temperatur)(5000.0 /1024.0 * tempvalue / 10.0);
    }
    
}
