#include "display.h"

namespace Display
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    static inline void sendCommand(const uint8_t* com, uint32_t size);
    static inline void terminateByteStream();
    static inline void sendTextCommand();
  
    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    static const uint8_t xDim = 239;
    static const uint8_t yDim = 63;
    //------------------------ Public Functions ------------------------------------  
  
    void initSerialHW(SerialBaudrate rate)
    {
        Serial.begin(9600);
        uint8_t a[] = {'S','B'};
        sendCommand(a, sizeof(a));
        Serial.print(rate);
        delay(100);
        Serial.begin(rate);
    }
    void setCursor(FunctionStatus stat)
    {
        uint8_t a[] = {'C','S',0};
        if(stat == ENABLE)
            a[2] = 1;
        sendCommand(a, sizeof(a));
    }
    
    void displayStartScreen(FunctionStatus status)
    {
       uint8_t a[] = {'D','S','S',0};
       if(status == ENABLE)
            a[3] = 1;
       sendCommand(a, sizeof(a));
    }
    
    void setTextPosBack()
    {
        uint8_t a[] = {'E','T', 'B'};
        sendCommand(a, sizeof(a));
    }
    
    void setDisplayMode(PixelDisplayLogic g)
    {
        uint8_t a[] = {'D','M', g};
        sendCommand(a, sizeof(a));
    }
    
    void setGraphicPosition(Coord x, Coord y)
    {
      uint8_t a[] = {'G','B', x, y};
      sendCommand(a, sizeof(a));
    }
    
    void drawLineTo(Coord x, Coord y)
    {
      uint8_t a[] = {'L','T', x, y};
      sendCommand(a, sizeof(a));
    }
    
    void drawLine(Coord x1, Coord y1,Coord x2, Coord y2)
    {
        uint8_t a[] = {'L','B', x1, y1, x2, y2};
        sendCommand(a, sizeof(a));
    }
    
    void drawFilledRectangle(Coord x1, Coord y1,Dimension width, Dimension height)
    {
        uint8_t a[] = {'F','R', x1, y1, x1 + width, y1 + height};
        sendCommand(a, sizeof(a));
    }
    
    void drawHorizontalBar(Dimension height, Coord y)
    {
        uint8_t a[] = {'F','R', 0, y, 239, y + height};
        sendCommand(a, sizeof(a));
    }
    
    void drawRectangle(Coord x1, Coord y1, Dimension width, Dimension height)
    {
        uint8_t a[] = {'D','R', x1, y1, x1 + width, y1 + height};
        sendCommand(a, sizeof(a));
    }
    void clearDisplay()
    {
        uint8_t a[] = {'C','L'};
        sendCommand(a, sizeof(a));
    }
    void drawCircle(Coord x1, Coord y1, Dimension radius)
    {
        uint8_t a[] = {'C','C', x1, y1, radius, 0};
        sendCommand(a, sizeof(a));
    }
    
    void drawFilledCircle(Coord x1, Coord y1, Dimension radius)
    {
        uint8_t a[] = {'C','C', x1, y1, radius, 1};
        sendCommand(a, sizeof(a));
    }
    
    void drawPixel(Coord x, Coord y)
    {
        uint8_t a[] = {'D','P', x, y};
        sendCommand(a, sizeof(a));
    }
    void drawImg(Coord x, Coord y, Dimension width, Dimension height, uint8_t* data, uint32_t size)
    {
        uint8_t a[] = {'D','I','M', x, y, width, height};
        sendCommand(a, sizeof(a));
        sendCommand(data, size);
    }
    
    void setCursor(TextGrid x, TextGrid y)
    {
        uint8_t a[] = {'T','P',x,y};
        sendCommand(a, sizeof(a)); 
    }
    void setFontType(TextFonts font)
    {
        uint8_t a[] = {'S','F',font};
        sendCommand(a, sizeof(a));   
    }
    
    void newline()
    {
        uint8_t a[] = {'T','R','T'};
        sendCommand(a, sizeof(a));
    }

    void writeString(const char* b)
    {
        sendTextCommand();
        Serial.print(b);
        terminateByteStream();
    }
    void writeInt(int32_t b)
    {
        sendTextCommand();
        Serial.print(b);
        terminateByteStream();
    }
    void writeUint(uint32_t b)
    {
        sendTextCommand();
        Serial.print(b);
        terminateByteStream();
    }
    void writeDouble(double b) 
    {
        sendTextCommand();
        Serial.print(b);
        terminateByteStream();
    }
    void writeFString(const __FlashStringHelper* message)
    {
        sendTextCommand();
        Serial.print(message);
        terminateByteStream();
    }

    void setAndWriteFString(TextGrid x, TextGrid y,const __FlashStringHelper* message)
    {
          setCursor(x, y);
          writeFString(message);
    }
    void setAndWriteString(TextGrid x, TextGrid y, const char* b)
    {
          setCursor(x, y);
          writeString(b);
    }
    void setAndWriteInt(TextGrid x, TextGrid y, int32_t b)
    {
          setCursor(x, y);
          writeInt(b);
    }
    void setAndWriteUInt(TextGrid x, TextGrid y, uint32_t b)
    {
          setCursor(x, y);
          writeUint(b);
    }
    void setAndWriteDouble(TextGrid x, TextGrid y, double b)
    {
          setCursor(x, y);
          writeDouble(b);
    }

    //------------------------------ Private Functions -----------------------------
    
    static inline void sendTextCommand()
    {
        uint8_t a[] = {'T','T'};
        sendCommand(a, sizeof(a));
    }
    
    static inline void terminateByteStream()
    {
        Serial.write(0);
    }
   
    static inline void sendCommand(const uint8_t* com, uint32_t size)
    {
         for(uint32_t index = 0; index < size; index ++)
            Serial.write(com[index]); 
    }
}
