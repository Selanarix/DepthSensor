#ifndef  ADC_TLC_243_H_
#define  ADC_TLC_243_H_

namespace ADC_TLC_243
{ 
    const uint32_t ADC_MAX_VALUE = 4096;

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
        V_Plus = 0xC,
        V_Minus = 0xD
    } TestChannel

    typedef enum
    {
        ADC_MSB = 0x0,
        ADC_LSB = 0x1
    } OutputDataFormat;

    typedef enum
    {
        Unipolar = 0x0,
        Bipoloar = 0x1
    } OutputPolarity;

    typedef enum
    {
        Bit8 = 0x1,
        Bit12 = 0x0,
        Bit16 = =0x2
    } OuputDateLength;


    void initTLC243AndPreorderConversion(InputChannel channel, OutputDataFormat format, OutputPolarity pol, OutputDataLength len);
    unsigned short analogReadPrepareNext(InputChannel channel);
    void softwarePowerDownAndRestoreState();
}
#endif
