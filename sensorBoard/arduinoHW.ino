#include "hal.h"
#include <Arduino.h>
#include "atmega328_registers.h"
#include "ADC_tlc243.h"
#include "logger.h"

namespace HAL
{ 
    //-------------------------- Private Types -------------------------------------
    
    typedef enum
    {
        InternalADC,
        ExternalADC
    } ADC_Source;
    
    //-------------------- Private Function Prototypes -----------------------------
    static inline int readInternalADC(PinID pin);
    static inline int readExternalADC(PinID pin);
    static inline bool testPin(PinID id);
    //------------------------- Private Data ---------------------------------------
    static bool externalInit = false;
    //------------------------ Read only ------------------------------------------
    static const uint32_t INTERNAL_ADC_MAX_VALUE = 1024.0;
    static const uint32_t INTERNAL_ADC_MAX_VOLTAGE_mV = 5000.0;
    static const double INTERNAL_ADC_mV_PER_TICK = (double)INTERNAL_ADC_MAX_VOLTAGE_mV/(double)INTERNAL_ADC_MAX_VALUE;
    static const uint8_t maxPinNumber = 13;
    static const ADC_Source usedSource = ExternalADC;
    
    const uint8_t pinTypeMapping[] = 
    {
       OUTPUT,// PIN_OUTPUT = 0,
       INPUT_PULLUP,// PIN_INPUT_PULLUP = 1,
       0xff,// PIN_INPUT_PULLDOWN = 2,
       INPUT// PIN_INPUT = 3
    };
   
    //------------------------------- Public Functions -----------------------------
    bool initBaseHW()
    {  
        
        //Use internal ADC no further setup necessary
        if(usedSource == InternalADC)
          return true;
        
        //already initalized
        if(externalInit == true)
          return true; 
          
        using namespace ADC_TLC_243;

        //Test DAC
        if(testTLC243(ADC_TLC_243::V_Diff) == ADCOutOfFunction)
        {
            Logger::log(Logger::ERROR, F("Could not find a proper working external ADC"));
            return false;
        }
        
        if(initAndPreorderConversion(Channel0, ADC_MSB, Unipolar, Bit16) != Initialized)
        {
            Logger::log(Logger::ERROR, F("Was not able to init external ADC"));
            return false;
        }
        
        Logger::log(Logger::DEBUG, F("Set up external ADC successfully"));
        externalInit = true;
        return true;
    }

    void initPin(PinID pId, PinType pType)
    {
        if(pType == PIN_INPUT_PULLDOWN || pType > PIN_INPUT || !testPin(pId))
            return;
        pinMode(pId, pinTypeMapping[pType]);
    }

    int analogReadPin(PinID p)
    {
        if(usedSource == InternalADC)
            return readInternalADC(p);
        if(externalInit == false)
            return -1;
        return readExternalADC(p);
    }
    
    
    double analogReadVoltage(PinID p)
    {
        int b = analogReadPin(p);
        
        if(usedSource == InternalADC)
            return (double)b * INTERNAL_ADC_mV_PER_TICK;
        if(externalInit == false)
            return -1;
        
        return (double)b * ADC_TLC_243::ADC_mV_PER_TICK;
    }

//    DDR  - Data Direction Register - read/write
//    PORT - Data Register           - read/write
//    PIN  - Input Pins Register     - read only 
    void digitalSetPinHeigh(PinID p)
    {
        if(!testPin(p))
              return;    
        
        volatile uint8_t tmp = SREG;
        cli(); //Critical section Start
        if(p < 8)
            PORTD = PORTD | (1 << p); 
        else 
            PORTB = PORTB | (1 << (p % 8));
        SREG=tmp; //Critical section End
    }
    
    void digitalSetPinLow(PinID p)
    {
        if(!testPin(p))
              return;    
   
        volatile uint8_t tmp = SREG;
        cli(); //Critical section Start
        if(p < 8)
            PORTD = PORTD & ~(1 << p);
        else
            PORTB = PORTB & ~(1 << (p % 8)); 
        SREG=tmp; //Critical section End
    }
    
    void digitalTogglePin(PinID p)
    {
        if(!testPin(p))
            return;    
         
        volatile int8_t tmp = SREG;
        cli(); //Critical section Start
        if(p < 8)
            PORTD = PORTD ^ ~(1 << p); //Use XOR
        else
            PORTB = PORTB ^ ~(1 << (p % 8)); 
        SREG=tmp; //Critical section End
    }
    
    PinState digitalReadPin(PinID p)
    {
        if(!testPin(p))
            return PIN_LOW;
            
        volatile uint8_t res = 0;
         if(p < 8)
            res = PIND & (1 << p);
        else
            res = PINB & (1 << p);
        
        if(res > 0)
            return PIN_HIGH;    
        return PIN_LOW;
    }
    //------------------------------ Private Functions -----------------------------

    static inline int readInternalADC(PinID pin)
    {
       return analogRead((uint8_t)pin);
    }
    
    static inline int readExternalADC(PinID pin)
    {
       if(pin > 10)
           return -1;
           
       //Send capture command and discard first result
       ADC_TLC_243::analogReadPrepareNext((ADC_TLC_243::InputChannel) pin);
       delay(10);
       return ADC_TLC_243::analogReadPrepareNext(ADC_TLC_243::Channel0); //Get result of capture channel
    }

    static inline bool testPin(PinID id)
    {
        return id < maxPinNumber;
    }
}
