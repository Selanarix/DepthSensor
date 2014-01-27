#include "led.h"  
#include "hal.h"

namespace ProjectLED
{
    //-------------------------- Private Types -------------------------------------
     
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------

    //------------------------ Read only ------------------------------------------

    //------------------------------- Public Functions -----------------------------
    void initLedPins()
    {
        HAL::initPin(LED0, HAL::PIN_OUTPUT);
    }
    void LED_On(LED led)
    {
        HAL::digitalSetPinHeigh(led);
    }
    void LED_Off(LED led)
    {
        HAL::digitalSetPinLow(led);
    }
    void LED_Toggle(LED led)
    {
        HAL::digitalTogglePin(led);
    }

    //------------------------------ Private Functions -----------------------------

}
