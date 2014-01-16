
#ifndef HARDWARE_ABSTRACTION_LAYER_H_
#ifndef HARDWARE_ABSTRACTION_LAYER_H_

namespace HAL
{ 
    typedef unsigned int PinID;
    
    typedef struct
    {
        PinID pin;
        PinType pType;
        PinPullup pPull;
    } Pin;

    typedef enum
    {
        OUTPUT,
        INPUT,
    } PinType;

    typedef enum
    {
        HEIGH,
        LOW,
    } PinState;

    typedef enum
    {
        NOPULL,
        PULLUP,
        PULLDOWN,
    } Pullup;

    void initPin(const Pin* p);
    unsigned int analogReadPin(const Pin* p);
    void digitalSetPin(const Pin* p, const PinState s);
    PinState digitalReadPin(const Pin* p);
}
#endif
