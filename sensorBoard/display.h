#ifndef  DISPLAY_H_
#define  DISPLAY_H_
#include "generalTypes.h"

namespace Display
{ 
    typedef uint8_t Coord;
    typedef uint8_t Dimension;
    typedef uint8_t TextGrid;

    typedef enum
    {
        Default = 0,
        Font1 = 6,
        Font2 = 10,
        Font3 = 18,
        Font4 = 51,
        Font5 = 120,
        Font6 = 123,

        User1 = 200,
        User2 = 201,
        User3 = 202,
        User4 = 203
    } TextFonts;
    
    typedef enum
    {
        AND = '&',
        XOR = '|',
        OR = '^',
        NOT = '!'
    } PixelDisplayLogic;


    typedef enum
    {
        B_300 = 300,
        B_1200 = 1200,
        B_2400 = 2400,
        B_4800 = 4800,
        B_9600 = 9600,
        B_14400 = 14400, 
        B_19200 = 19200, 
        B_28800 = 28800, 
        B_38400 = 38400,
        B_57600 = 57600,
        B_115200 = 115200
   } SerialBaudrate;
 

    void initSerialHW(SerialBaudrate rate);
    void setCursor(FunctionStatus status);
    void setDisplayMode(PixelDisplayLogic a);
    void setGraphicPosition(Coord x, Coord y);
    void drawLineTo(Coord x, Coord y);
    void drawLine(Coord x1, Coord y1,Coord x2, Coord y2);
    void drawFilledRectangle(Coord x, Coord y,Dimension width, Dimension height);
    void drawHorizontalBar(Dimension height, Coord y);
    void drawRectangle(Coord x1, Coord y1, Dimension width, Dimension height);
    void clearDisplay();
    void drawFilledCircle(Coord x1, Coord y1, Dimension radius);
    void drawCircle(Coord x1, Coord y1, Dimension radius);
    void drawPixel(Coord x, Coord y);
    void drawImg(Coord x, Coord y, Dimension width, Dimension height, uint8_t* data, uint32_t size);
    void setCursor(TextGrid x, TextGrid y);
    void setFontType(TextFonts font);
    void newline();

    void writeFString(const __FlashStringHelper* message);
    void writeString(const char* b);
    void writeInt(int32_t b);
    void writeUint(uint32_t b);
    void writeDouble(double b);  

    void setAndWriteFString(TextGrid x, TextGrid y,const __FlashStringHelper* message);
    void setAndWriteString(TextGrid x, TextGrid y, const char* b);
    void setAndWriteInt(TextGrid x, TextGrid y, int32_t b);
    void setAndWriteUInt(TextGrid x, TextGrid y, uint32_t b);
    void setAndWriteDouble(TextGrid x, TextGrid y, double b);
}
#endif
