#ifndef  TMP_102_H_
#define  TMP_102_H_

namespace TMP_102
{ 
    typedef enum
    {
        ENABLE,
        DISABLE
    } FunctionStatus;

    typedef enum
    {
        GND = 0x48,
        VCC = 0x49,
        SCL = 0x4A,
        SDA = 0x4B
    } I2C_Addresses; 

    typedef enum
    {
        F_250kHz = 0x0, 
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
        double (*readTemperatur)(const TMP102* const);
        double (*oneShotRead)(const TMP102* const);
        void (*setPowerMode)(TMP102* const, FunctionStatus status);
        void (*setTermostatMode)(TMP102* const, double , double, TermostatMode, ConsecutiveFaults, AlertPinPolarity);
        const TermostatLimits* (*getTermostatLimits)(const TMP102* const);
        I2C_Addresses (*getAddress)(const TMP102* const);
        void (*setExtendetMode)(TMP102* const, FunctionStatus);
        void (*setMeasureFrequency)(const TMP102* const, ConversionRate);
    };

    bool construct(TMP102* thi,I2C_Addresses add); 
}
#endif
