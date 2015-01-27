#ifndef  TMP_102_H_
#define  TMP_102_H_
#include "generalTypes.h"

namespace TMP102
{ 
    typedef enum
    {
        GND = 0x48,
        VCC = 0x49,
        SCL = 0x4A,
        SDA = 0x4B
    } I2C_Addresses; 

    typedef enum
    {
        F_250milliHz = 0x0, 
        F_1Hz = 0x1,
        F_4Hz = 0x2,
        F_8Hz = 0x3
    } ConversionRate;

    typedef enum
    {
        Comperator = 0x00,
        Interrupt = 0x01
    } TermostatMode;

    typedef enum
    {
        ActiveLow = 0x00,
        ActiveHigh = 0x01
    } AlertPinPolarity;

    typedef enum
    {
        Fault1 = 0x00,
        Faul2 = 0x01,
        Faul4 = 0x02,
        Faul6 = 0x03
    } ConsecutiveFaults;

    typedef struct
    {
        double min;
        double max;
    } TermostatLimits;
    
    typedef struct _TMP102 TMP102;

    struct _TMP102 
    {
        I2C_Addresses addr;
        bool lowPowerMode;
        bool isExtendetMode;
        TermostatLimits limits;
    };

    bool construct(TMP102* thi,I2C_Addresses add); 
    I2C_Addresses getAddr(const TMP102* const thi); 
    const TermostatLimits* getLimit(const TMP102* const thi);
    double oneRead(const TMP102* const thi);
    double simpleRead(const TMP102* const thi);
    void setExtendetMod(TMP102* const thi, FunctionStatus status);
    void setMeasureFrequ(const TMP102* const thi, ConversionRate rate);
    void powerMode(TMP102* const thi, FunctionStatus status);
    void termostatEnable(TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol);
}
#endif
