
#ifndef HARDWARE_ABSTRACTION_LAYER_H_
#define HARDWARE_ABSTRACTION_LAYER_H_

namespace HAL
{ 
    typedef unsigned int PinID;

    typedef enum
    {
        PIN_OUTPUT = 0,
        PIN_INPUT_PULLUP = 1,
        PIN_INPUT_PULLDOWN = 2,
        PIN_INPUT = 3
    } PinType;

    typedef enum
    {
        PIN_LOW = 0,
        PIN_HIGH = 1  
    } PinState;

    bool initBaseHW();
    void initPin(PinID, PinType);
    int analogReadPin(PinID p);
    void digitalSetPinHeigh(PinID p);
    void digitalSetPinLow(PinID p);
    void digitalTogglePin(PinID p);
    PinState digitalReadPin( PinID p);
}
#endif
