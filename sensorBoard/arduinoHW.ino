#include "hal.h"
#include <Arduino.h>

namespace HAL
{ 
    //-------------------------- Private Types -------------------------------------
      
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------

    //------------------------ Read only ------------------------------------------
    
    const uint8_t maxPinNumber = 20;
    
    const uint8_t pinTypeMapping[] = 
    {
       OUTPUT,// PIN_OUTPUT = 0,
       INPUT_PULLUP,// PIN_INPUT_PULLUP = 1,
       0xff,// PIN_INPUT_PULLDOWN = 2,
       INPUT// PIN_INPUT = 3
    };
    
    /*
    void pinMode(uint8_t, uint8_t);
    void digitalWrite(uint8_t, uint8_t);
    int digitalRead(uint8_t);
    int analogRead(uint8_t);
    void analogReference(uint8_t mode);
    void analogWrite(uint8_t, int);
    
    #define HIGH 0x1
    #define LOW  0x0
    #define INPUT 0x0
    #define OUTPUT 0x1


    */
    //------------------------------- Public Functions -----------------------------
    void initPin(PinID pId, PinType pType)
    {
        if(pType == PIN_INPUT_PULLDOWN || pType > 3 || pId > maxPinNumber)
            return;
        pinMode(pId, pType);
    }

    int analogReadPin(PinID p)
    {
        if(p > maxPinNumber)
            return 0;
        return analogRead(p);
    }

    void digitalSetPin(PinID p, PinState s)
    {
        if(s > 2 || p > maxPinNumber)
              return;
        digitalWrite(p, s);
    }
    
    PinState digitalReadPin(PinID p)
    {
        if(p > maxPinNumber)
            return PIN_LOW;
        return (PinState)digitalRead(p);
    }
    //------------------------------ Private Functions -----------------------------

}
