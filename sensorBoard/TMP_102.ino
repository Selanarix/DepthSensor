#include "TMP_102.h"
#include "wire.h"

namespace TMP_102
{
    typedef enum
    {
        WhileMeasure = 0x00;
        Start_CompletedOneShot = 0x01
    } OneShot;

    typedef enum
    {
        GND = 0x48,
        VCC = 0x49,
        SCL = 0x4A,
        SDA = 0x4B
    } I2C_Addresses;  

    typedef enum
    {
        Temperatur = 0x00,
        Config = 0x01,
        T_Low = 0x02,
        T_High = 0x02,
    } PointerAddress;

    //Methods
    static I2C_Addresses getAddr(const TMP102* const thi);
    static const TermostatLimits* getLimit(const TMP102* const thi);
    static double simpleRead(const TMP102* const thi);
    static double oneRead(const TMP102* const thi);
    static void toLowPower(const TMP102* const thi);
    static void termostatEnable(const TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol);

    static bool initI2C(const TMP102* const thi);

    

    bool construct(TMP102* thi,I2C_Addresses add)
    {
        if(thi == NULL)
            return false;
        if(add < GND || add > SDA)
            return false;
        thi->addr = add;
        thi->lowPowerMode = false;
        thi->limits.min = 0.0;
        thi->limits.max = 0.0;

        thi->readTemperatur = simpleRead;
        thi->oneShotRead = oneRead;
        thi->setToLowPowerMode = toLowPower;
        thi->setTermostatMode = termostatEnable
        thi->getTermostatLimits = getLimit; 
        thi->getAddress = getAddr;

        return initI
    }

    static I2C_Addresses getAddr(const TMP102* const thi)
    {
        return thi->addr;
    }

    static const TermostatLimits* getLimit(const TMP102* const thi)
    {
        return &thi->limits;
    }

    static double simpleRead(const TMP102* const thi)
    {
        
    }

    static void toLowPower(const TMP102* const thi)
    {
    
    }

    static void termostatEnable(const TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol)
    {
        
    }

    static void initI2C(const TMP102* const thi)
    {
        
    }
    

}
