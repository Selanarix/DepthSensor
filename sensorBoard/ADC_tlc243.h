#ifndef  ADC_TLC_243_H_
#define  ADC_TLC_243_H_

namespace ADC_TLC_243
{ 
    const unsigned int ADC_MAX_VALUE = 4096;
    const unsigned short InvalidCaptureChannel = 0x80;

    typedef enum
    {
        Channel0 = 0,
        Channel1 = 0x1,
        Channel2 = 0x2,
        Channel3 = 0x3,
        Channel4 = 0x4,
        Channel5 = 0x5,
        Channel6 = 0x6,
        Channel7 = 0x7,
        Channel8 = 0x8,
        Channel9 = 0x9,
        Channel10 = 0xA
    } InputChannel;  

    typedef enum
    {
        V_Diff = 0xB,
        V_Plus = 0xD,
        V_Minus = 0xC
    } TestChannel;

    typedef enum
    {
        ADC_MSB = 0x0,
        ADC_LSB = 0x1
    } OutputDataFormat;

    typedef enum
    {
        Initialized,
        IllegalParameters,
        UnsupportedMode
    } ADC_InitResult;

    typedef enum
    {
        Unipolar = 0x0,
        Bipolar = 0x1
    } OutputPolarity;

    typedef enum
    {
        Bit8 = 0x1,
        Bit12 = 0x0,
        Bit16 = 0x3
    } OutputDataLength;

    typedef enum
    {   
        ADCWorking,
        ADCOutOfFunction
    } TestResult;

    ADC_InitResult initAndPreorderConversion(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len);
    TestResult testTLC243(TestChannel testChannel);
    unsigned short analogReadPrepareNext(InputChannel channel);
    void softwarePowerDownAndRestoreState();
}
#endif
