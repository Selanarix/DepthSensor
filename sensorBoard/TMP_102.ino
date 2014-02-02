#include "TMP_102.h"
#include <Wire.h>

namespace TMP_102
{
    typedef enum
    {
        Temperatur = 0x00,
        Config = 0x01,
        T_Low = 0x02,
        T_High = 0x02,
    } PointerAddress;

    const uint32_t SDA_Pin = A4;
    const uint32_t SCL_Pin = A5;
    const double DegreePerTick = 0.0625;

    const uint16_t lowPowerModeBit = 0x100;
    const uint16_t oneShotModeBit = 0x8000;
    const uint16_t extendedModeBit = 0x10;

    //Methods
    static double simpleRead(const TMP102* const thi);
    static double oneRead(const TMP102* const thi);
    static void powerMode(TMP102* const thi, FunctionStatus status);
    static void termostatEnable(TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol);
    static const TermostatLimits* getLimit(const TMP102* const thi);
    static I2C_Addresses getAddr(const TMP102* const thi);
    static void setExtendetMod( TMP102* const thi, FunctionStatus status);
    static void setMeasureFrequ(const TMP102* const thi, ConversionRate rate);
  

    static bool initI2C(const TMP102* const thi);
    static inline void setPointerReg(const TMP102* const thi, PointerAddress pAddr);
    static inline uint16_t temperatureToInt16_t(const TMP102* const thi, double n);
    static inline uint16_t getConfig(const TMP102* const thi);
    static inline void setConfig(const TMP102* const thi,uint16_t conf);
    static inline int16_t readTempReg(const TMP102* const thi);
    

    bool construct(TMP102* thi,I2C_Addresses add)
    {
        if(thi == NULL)
            return false;
        if(add < GND || add > SDA)
            return false;
        thi->addr = add;
        thi->lowPowerMode = false;
        thi->isExtendetMode = false;
        thi->limits.min = 0.0;
        thi->limits.max = 0.0;

        thi->readTemperatur = simpleRead;
        thi->oneShotRead = oneRead;
        thi->setPowerMode = powerMode;
        thi->setTermostatMode = termostatEnable;
        thi->getTermostatLimits = getLimit; 
        thi->getAddress = getAddr;
        thi->setExtendetMode = setExtendetMod;
        thi->setMeasureFrequency = setMeasureFrequ;

        return initI2C(thi);
    }

    static I2C_Addresses getAddr(const TMP102* const thi)
    {
        return thi->addr;
    }

    static const TermostatLimits* getLimit(const TMP102* const thi)
    {
        return &thi->limits;
    }

    static double oneRead(const TMP102* const thi)
    {
        if(!thi->lowPowerMode)
            return 0.0;
        uint16_t conf = getConfig(thi);
        conf |= oneShotModeBit;
        setConfig(thi, conf);
        while((getConfig(thi) & oneShotModeBit) == 0) {}
        
        setPointerReg(thi, Temperatur);
        Wire.endTransmission();

        Wire.requestFrom(thi->addr, 2);
        int16_t value = (int16_t)readTempReg(thi);
        
        conf &= ~oneShotModeBit;
        setConfig(thi, conf); 

        return (double)value * DegreePerTick;
    }

    static double simpleRead(const TMP102* const thi)
    {
        setPointerReg(thi, Temperatur);
        Wire.endTransmission();

        Wire.requestFrom(thi->addr, 2);

        uint16_t value = readTempReg(thi);

        return (double)value * DegreePerTick;
    }

    static void setExtendetMod(TMP102* const thi, FunctionStatus status)
    {
        uint16_t conf = getConfig(thi);
        if(status == ENABLE)
        {
            conf |= extendedModeBit; 
            thi->isExtendetMode = true;
        }
        else
        {
            conf &= ~extendedModeBit; 
            thi->isExtendetMode = false;
        }
        setConfig(thi, conf);
    }

    static void setMeasureFrequ(const TMP102* const thi, ConversionRate rate)
    {
        uint16_t conf = getConfig(thi);

        setConfig(thi, conf);
         //reset bits
        conf &= ~(0x03 << 6);
        //configure new
        conf |= (rate << 6);
        setConfig(thi, conf);
    }

    static void powerMode(TMP102* const thi, FunctionStatus status)
    {
        uint16_t conf = getConfig(thi);
        if(status == ENABLE)
        {
            conf |= lowPowerModeBit; 
            thi->lowPowerMode = true;
        }
        else
        {
            conf &= ~lowPowerModeBit; 
            thi->lowPowerMode = false;
        }
        setConfig(thi, conf);
    }


    static void termostatEnable(TMP102* const thi, double min, double max, TermostatMode mod, ConsecutiveFaults faul, AlertPinPolarity pol)
    {
        if(thi == NULL || min > max)
            return;
        uint16_t tLow = temperatureToInt16_t(thi, min);
        uint16_t tHigh = temperatureToInt16_t(thi, max);

        setPointerReg(thi, T_Low);
        Wire.write((uint8_t)(tLow>>8));
        Wire.write((uint8_t)tLow);
        Wire.endTransmission();

        setPointerReg(thi, T_High);
        Wire.write((uint8_t)(tHigh>>8));
        Wire.write((uint8_t)tHigh);
        Wire.endTransmission();

        thi->limits.min = min;
        thi->limits.max = max;

        uint16_t conf = getConfig(thi);
        //reset bits
        conf &= ~(0x0f << 9);
        //configure new
        conf |= (mod << 9);
        conf |= (faul << 11);
        conf |= (pol << 10);
        setConfig(thi, conf);
    }

    //------------- Private Helpers -------------

    static bool initI2C(const TMP102* const thi)
    {
        pinMode(SDA_Pin, INPUT_PULLUP);
        pinMode(SCL_Pin, INPUT_PULLUP);
        return true;
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

    static inline uint16_t temperatureToInt16_t(const TMP102* const thi, double n)
    {
        uint16_t res = 0;
        if(n < 0)
        {   
            n *= -1;
            res = (uint16_t)(n / DegreePerTick);
            res = (~res) + 1;
        }
        else
            res = (uint16_t)(n / DegreePerTick);
 
        if(!thi->isExtendetMode)
            return res << 4;
        else 
            return res << 3;
    }

    static inline int16_t readTempReg(const TMP102* const thi)
    {
        uint16_t msb = Wire.read();
        uint16_t lsb = Wire.read();
        int16_t res = ((msb<<8 | lsb));
        
        if(!thi->isExtendetMode)
            return res >> 4;
        else 
            return res >> 3;
        
    }
}
