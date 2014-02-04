#include "ADC_tlc243.h"
#include "logger.h"
#include "SPI.h"

namespace ADC_TLC_243
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    unsigned short read16BitAndPreorder(InputChannel nextInputForConversion);
    unsigned short read8BitAndPreorder(InputChannel nextInputForConversion);
    void initPeripheral();

    inline uint8_t buildConfigPartOfCommand(OutputDataFormat format, OutputPolarity pol, OutputDataLength len);
    inline uint8_t changeInputForCommand(uint8_t inputSource, uint8_t lastMode);
    inline bool testParameters(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len);
    //------------------------- Private Data ---------------------------------------
    static uint8_t actualCommand = 0;
    unsigned short (*readFunction)(InputChannel nextInputForConversion) = 0;

    //------------------------ Read only ------------------------------------------
   const uint8_t softwarePowerDown = 0xE;
   const uint8_t NSS_Pin = 9;

    //------------------------------- Public Functions -----------------------------
    ADC_InitResult initAndPreorderConversion(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len)
    {
        if(!testParameters(channel,format, pol, len))
            return IllegalParameters;

        initPeripheral();
        
        switch(len)
        {
            case Bit8:
                readFunction = read8BitAndPreorder;
            break;
            case Bit12:
                return UnsupportedMode;
            break;
            case Bit16:
                readFunction = read16BitAndPreorder;
            break;
        }
        //Set up ADC command byte
        actualCommand = buildConfigPartOfCommand(format, pol, len);
        //send command and throw away first adc result        
        readFunction(channel);

        return Initialized;
    }

    /**
    * The ADC has 3 test input channels to test if the conversation is working
    * proper. This function will configure on of those channels to see if the result is
    * within a proper range and will return the result of the test. This function is going
    * to use the 16 bit ADC mode. 
    *
    * @param testChannel: test channel this function will use to test if ADC is working as
                          aspected. 
    * @return result of test. ADCOutOfFunction if the parameter is not valid. Or the received
    *         value is out of expected range. Range is maped to testChannel like the following
    *        V_Diff: measurment within 2^12/2 +- delta
    *        V_Plus: measurment within 2^12 and 2^12- delta
    *        V_Minus: measurment within 0 and delta
    */
    TestResult testTLC243(TestChannel testChannel)
    {
        if(testChannel > V_Plus)
            return ADCOutOfFunction;
        
        initPeripheral();    
        delay(10); //Give some warm up time
        
        readFunction = read16BitAndPreorder;
        actualCommand = buildConfigPartOfCommand(ADC_MSB, Unipolar, Bit16);
        readFunction((InputChannel)testChannel); //Configure test channel and discard result
        delay(10); //Give conversion time
        uint16_t res = readFunction(Channel0);
        const uint32_t delta = 10;
        switch(testChannel)
        {
            case V_Diff:
                if(res > ADC_MAX_VALUE/2-delta && res < ADC_MAX_VALUE/2+delta)
                    return ADCWorking;
                return ADCOutOfFunction;
            case V_Plus:
                if(res > (ADC_MAX_VALUE - delta))
                    return ADCWorking;
                return ADCOutOfFunction;
            case V_Minus:
               if(res < delta)
                    return ADCWorking;
                return ADCOutOfFunction;                   
        };
        return ADCOutOfFunction;
    }

    /**
    * Sets ADC to software reset and configure input channel for next capture. This function
    * will only perform one reset and will set up ADC for next conversation afterwards. Be aware
    * this function will discard the result of the last ordered input channel. If the given
    * parameter is invalid nothing will be done. 
    * 
    * @param nextInputForConversion: Input channel you would like to read from in the next read 
    *                                command
    */
    void softwarePowerDownAndPrepare(InputChannel channel)
    {
        if(channel > Channel10)
            return ;
        readFunction((InputChannel)softwarePowerDown);
        readFunction(channel); 
    }

    /**
    * Reads whether 12 or 8 bit accurancy ADC value and configures simultaneously the capture cannel
    * you would like to read from next time. Init DAC first before using this function. If the given
    * channel is invalid the function will return 0x80(128 dec) as capture result. 
    * 
    * @param nextInputForConversion: Input channel you would like to read from in the next read 
    *                                command
    * @return 12 or 8 bit sample result from previous configured capture channel. Or InvalidCaptureChannel 
    *         if parameter is invalid.
    */
    unsigned short analogReadPrepareNext(InputChannel channel)
    {
         if(channel > Channel10)
            return InvalidCaptureChannel;

         return readFunction(channel);
    }

    //------------------------------ Private Functions ----------------------------

    /**
    * Reads 16 bit and simultaneously sends a new 8 bit command to ADC for next conversation. 
    * Because SPI allows full duplex communication you receive capture result from last 
    * capture command and send new capture next for next reading. Because the ADC only supports
    * 12 bits resolution the 4 high bits are discarded and set to 0.
    * 
    * @param nextInputForConversion: Input channel you would like to read from in the next read 
    *                                command
    * @return 12 bit sample result from previous configured capture channel
    */
    unsigned short read16BitAndPreorder(InputChannel nextInputForConversion)
    {
        volatile uint16_t resultMsb=0, resultLsb=0;
        actualCommand = changeInputForCommand(nextInputForConversion, actualCommand);
        volatile uint8_t tmp = SREG;
        cli(); //Critical section Start
        digitalWrite(NSS_Pin,LOW);
        resultMsb=SPI.transfer(actualCommand);
        resultLsb=SPI.transfer(0x00);
        digitalWrite(NSS_Pin,HIGH);
        SREG=tmp; //Critical section End
        return ((resultMsb<<8 | resultLsb) >> 4);
    }

    /**
    * Reads 8 bit and simultaneously sends a new 8 bit command to ADC for next conversation. 
    * Because SPI allows full duplex communication you receive capture result from last 
    * capture command and send new capture next for next reading. Even it the ADC has
    * 12 bit resolution it supports only 8 bit in this mode. So you loose resolution. 
    * 
    * @param nextInputForConversion: Input channel you would like to read from in the next read 
    *                                command
    * @return 8 bit sample result from previous configured capture channel
    */
    unsigned short read8BitAndPreorder(InputChannel nextInputForConversion)
    {
        volatile uint16_t result;
        actualCommand = changeInputForCommand(nextInputForConversion, actualCommand);
        volatile uint8_t tmp = SREG;
        cli(); //Critical section Start
        digitalWrite(NSS_Pin,LOW);
        result=SPI.transfer(actualCommand);
        digitalWrite(NSS_Pin,HIGH);
        SREG=tmp; //Critical section End
        return result;
    }    

    /**
    * Inits IO pin for chip select (NSS) and sets up SPI for ADC communication. 
    * Clock is idle low and samples on rising ege. The max frequency supported is
    * 10 MHz. 
    */
    void initPeripheral()
    {
        /* Init chip select*/        
        pinMode(NSS_Pin,OUTPUT);
        
        /* Init SPI Bus*/
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.setClockDivider(SPI_CLOCK_DIV64);
        SPI.begin();
    }

    /**
    * Builds up a new command byte for ADC based on given Settings. Call changeInputForCommand
    * function to add input source information to configuration. 
    *
    * @param format: Output format for ADC (MSB or LSB)
    * @param pol: Polarity of output for ADC (Unipoloar or Bipoloar(2er compliment))
    * @param len: Lenght of output sampled by the ADC (8 or 12 or 16 bit)
    * @return ADC output command byte with given basic ADC configuation 
    */
    inline uint8_t buildConfigPartOfCommand(OutputDataFormat format, OutputPolarity pol, OutputDataLength len)
    {
        return  (len << 2) | (format << 1) | pol;
    }

    /**
    * Builds up a command byte for ADC by adding input source information to preconfigured command. 
    * Call buildConfigPartOfCommand function before using this function to get a entire command.
    *
    * @param inputSource: Input source the ADC should sample from
    * @param lastMode: Preconfiguration byte containing ADC config without input source
    * @return ADC output command byte with given input source
    */
    inline uint8_t changeInputForCommand(uint8_t inputSource, uint8_t lastMode)
    {
        return (lastMode&0xF) | (inputSource << 4);
    }

    /**
    * Tests if given parameters are in a valid range.
    * 
    * @param channel: Input capture channel for ADC (0-10)
    * @param format: Output format for ADC (MSB or LSB)
    * @param pol: Polarity of output for ADC (Unipoloar or Bipoloar(2er compliment))
    * @param len: Lenght of output sampled by the ADC (8 or 12 or 16 bit)
    * @return boolean of result. (True means all ok)
    */
    inline bool testParameters(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len)
    {
        return (channel <= Channel10 && format <= ADC_LSB && pol <= Bipolar && len <= Bit16);
    }
}
