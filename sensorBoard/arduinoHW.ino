#include "hal.h"

namespace HAL
{ 
    //-------------------------- Private Types -------------------------------------
      
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------

    //------------------------ Read only ------------------------------------------

    //------------------------------- Public Functions -----------------------------
    void initPin(const Pin* p)  
    {
        pinMode(p->pin, p->PinType);
    }

    unsigned int analogReadPin(const Pin* p)
    {
        return analogRead(p->pin);
    }

    void digitalSetPin(const Pin* p, const PinState s)
    {
        digitalWrite(p->pin, HIGH);
    }
    
    PinState digitalReadPin(const Pin* p)
    {
        return digitalRead(p->ping);
    }
    //------------------------------ Private Functions -----------------------------

}
