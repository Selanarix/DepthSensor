#include "led.h"  

namespace ProjectLED
{
    //-------------------------- Private Types -------------------------------------
     
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------

    //------------------------ Read only ------------------------------------------

    //------------------------------- Public Functions -----------------------------
    void initLedPins()
    {
        pinMode(LED0, OUTPUT);
    }
    void LED_On(LED led)
    {
        digitalWrite(led, HIGH);
    }
    void LED_Off(LED led)
    {
        digitalWrite(led, LOW);
    }
    void LED_Toggle(LED led)
    {
       digitalWrite(led, !digitalRead(led)); 
    }

    //------------------------------ Private Functions -----------------------------

}
