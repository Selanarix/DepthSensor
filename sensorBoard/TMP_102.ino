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

    const unsigned int SDA_Pin = A4;
    const unsigned int SCL_Pin = A5;
    const double DegreePerTick = 0.0625;

    //Methods
    static I2C_Addresses getAddr(const TMP102* const thi);
    static const TermostatLimits* getLimit(const TMP102* const thi);
    static double simpleRead(const TMP102* const thi);
    static double oneRead(const TMP102* const thi);
    static void toLowPower(const TMP102* const thi);
    static void termostatEnable(const TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol);

    static bool initI2C(const TMP102* const thi);
    static inline void setPointerReg(const TMP102* const thi, PointerAddress pAddr)
    static inline uint16_t getConfig(const TMP102* const thi);
    static inline void setConfig(const TMP102* const thi,uint16_t conf);
    

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
        thi->setToLowPowerMode = toLowPower;//D
        thi->setTermostatMode = termostatEnable
        thi->getTermostatLimits = getLimit; //D
        thi->getAddress = getAddr;//D

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
        setPointerReg(thi, Temperatur);
        Wire.endTransmission();

        Wire.requestFrom(thi->addr, 2);

        uint16_t msb = Wire.read();
        uint16_t lsb = Wire.read();

        int value = ((msb<<8 | lsb)) >> 4;

        return (double)value * DegreePerTick;
    }

    static void toLowPower(const TMP102* const thi)
    {
        uint16_t conf = getConfig(thi);
        conf |= (1<<8);
        setConfig(thi, conf);
    }

    static void termostatEnable(const TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol)
    {
        
    }

    static void initI2C(const TMP102* const thi)
    {
        pinMode(SDA_Pin, INPUT_PULLUP);
        pinMode(SCL_Pin, INPUT_PULLUP);
    }

    static inline void setPointerReg(const TMP102* const thi, PointerAddress pAddr)
    {
        Wire.beginTransmission(thi->addr); 
        Wire.write(pAddr);
    }

    static inline uint16_t getConfig(const TMP102* const thi)
    {
        uint16_t res = 0;
        setPointerReg(thi, Config);
        Wire.endTransmission();
        Wire.requestFrom(thi->addr, 2);
        res = Wire.read();
        res = (res<<8) | Wire.read();
        return res;
    }

    static inline void setConfig(const TMP102* const thi,uint16_t conf)
    {
        uint16_t res = 0;
        setPointerReg(thi, Config);
        Wire.write((uint8_t)(conf>>8));
        Wire.write((uint8_t)conf);
        Wire.endTransmission();
    }
}
