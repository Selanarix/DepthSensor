
#ifndef HARDWARE_ABSTRACTION_LAYER_H_
#define HARDWARE_ABSTRACTION_LAYER_H_

namespace HAL
{ 
    typedef unsigned int PinID;

    typedef enum
    {
        PIN_OUTPUT,
        PIN_INPUT
    } PinType;

    typedef enum
    {
        PIN_HEIGH,
        PIN_LOW
    } PinState;

    typedef enum
    {
        NOPULL,
        PULLUP,
        PULLDOWN
    } PinPullup;
    
       typedef struct
    {
        PinID pin;
        PinType pType;
        PinPullup pPull;
    } Pin;

    void initPin(const Pin* p);
    unsigned int analogReadPin(const Pin* p);
    void digitalSetPin(const Pin* p, const PinState s);
    PinState digitalReadPin(const Pin* p);
    
    
}
#endif
