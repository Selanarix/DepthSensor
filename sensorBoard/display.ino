#include "display.h"

namespace Display
{
    void initSerialHW(SerialBaudrate rate)
    {
        Serial.begin(9600);
        
    }
    void setCursor(FunctionStatus status)
    {
    }
    void setGraphicPosition(Coord x, Coord y)
    {
    }
    void drawLineTo(Coord x, Coord y)
    {
    }
    void drawLine(Coord x1, Coord y1,Coord x2, Coord y2)
    {
    }
    void drawFilledRectangle(Coord x, Coord y)
    {
    }
    void drawRectangle(Coord x1, Coord y1, Dimension width, Dimension height)
    {
    }
    void clearDisplay()
    {
    }
    void changePowerStatus(FunctionStatus status)
    {
    }
    void drawCircle()
    {
    }
    void drawPixel(Coord x, Coord y)
    {
    }
    void drawImg(Coord x, Coord y, Dimension width, Dimension height, uint8_t data, uint32_t size)
    {
    }
    void setCursor(TextGrid x, TextGrid y)
    {
    }
    void setFontType(TextFonts font)
    {
    }
    void newline()
    {
    }

    void writeString(const char* b)
    {
    }
    void writeInt(int32_t b)
    {
    }
    void writeUint(uint32_t b)
    {
    }
    void writeDouble(double b) 
    {
    }

    void setAndWriteString(TextGrid x, TextGrid y, const char* b)
    {
    }
    void setAndWriteInt(TextGrid x, TextGrid y, int32_t b)
    {
    }
    void setAndWriteUInt(TextGrid x, TextGrid y, uint32_t b)
    {
    }
    void setAndWriteDouble(TextGrid x, TextGrid y, double b)
    {
    }
}
