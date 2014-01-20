#include "ADC_tlc243.h"
#include "logger.h"
#include "SPI.h"

namespace DepthSensor 
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    unsigned short read16BitAndPreorder(InputChannel nextInputForConversion);
    unsigned short read8BitAndPreorder(InputChannel nextInputForConversion);

    inline uint8_t buildConfigPartOfCommand(OutputDataFormat format, OutputPolarity pol, OuputDateLength len);
    inline uint8_t changeInputForCommand(uint8_t inputSource, uint8_t lastMode);
    inline bool testParameters(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len));
    //------------------------- Private Data ---------------------------------------
    static uint8_t actualCommand = 0;
    static (unsigned short)(*readFunction)(InputChannel nextInputForConversion) = 0;

    //------------------------ Read only ------------------------------------------
   const unsigned char softwarePowerDown = 0xE;

    //------------------------------- Public Functions -----------------------------
    void initTLC243AndPreorderConversion(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len);
    {
        if(!testParameters(channel,format, pol, len))
            return; //TODO

        /* Init chip select*/        
        pinMode(9,OUTPUT);
        
        /* Init SPI Bus*/
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.setClockDivider(SPI_CLOCK_DIV64);
        SPI.begin();
        
        switch(len)
        {
            case Bit8:
                readFunction = read8BitAndPreorder;
            break;
            case Bit12:
                return ; //TODO
            break;
            case Bit16:
                readFunction = read16BitAndPreorder;
            break;
        }
        //Set up ADC command byte
        actualCommand = buildConfigPartOfCommand(format, pol, len);
       
        //send command and throw away first adc result        
        readFunction(channel);
    }

    void softwarePowerDownAndPrepare(InputChannel channel)
    {
        if(channel > Channel10)
            return 0; //TODO
        readFunction(softwarePowerDown);
        readFunction(channel); 
    }

    unsigned short analogReadPrepareNext(InputChannel channel)
    {
         if(channel > Channel10)
            return 0; //TODO
         return readFunction(channel);
    }

    //------------------------------ Private Functions -----------------------------
    unsigned short read16BitAndPreorder(InputChannel nextInputForConversion)
    {
        uint16_t resultMsb=0, resultLsb=0;
        actualCommand = changeInputForCommand(nextInputForConversion, actualCommand);
        digitalWrite(9,LOW);
        resultMsb=SPI.transfer(actualCommand);
        resultLsb=SPI.transfer(0x00);
        digitalWrite(9,HIGH);
        return ((resultMsb<<8 | resultLsb) >> 4);
    }

    unsigned short read8BitAndPreorder(InputChannel nextInputForConversion)
    {
        uint16_t result;
        actualCommand = changeInputForCommand(nextInputForConversion, actualCommand);
        digitalWrite(9,LOW);
        result=SPI.transfer(actualCommand);
        digitalWrite(9,HIGH);
        return result;
    }    

    inline uint8_t buildConfigPartOfCommand(OutputDataFormat format, OutputPolarity pol, OuputDateLength len)
    {
        return  | (len << 2) | (format << 1) | pol);
    }

    inline uint8_t changeInputForCommand(uint8_t inputSource, uint8_t lastMode)
    {
        return (lastMode&0xF) | (inputSource << 4);
    }

    inline bool testParameters(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len))
    {
        return (channel < 11 && format < 2 && pol < 2 && len < 3);
    }
}
