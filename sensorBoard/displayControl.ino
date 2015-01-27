#include "displayControl.h"
#include "display.h"
#include "generalTypes.h"
#include "constString.h"
#include "img.h"

namespace DisplayControl
{
    //-------------------------- Private Types -------------------------------------
    typedef struct
    {
         const char* desc;
         Display::Coord pixCord;
         Display::TextGrid gridCord;
    } IndicatorData;
    
    typedef struct
    {
        Display::TextGrid x;
        Display::TextGrid y;
        Display::Coord xp;
        Display::Coord yp;
    } MeasurmentOutputSpace;
    
    //-------------------- Private Function Prototypes -----------------------------
    static inline void setUpIndicator(const IndicatorData* data);
    static inline void printErrorLine(uint8_t index, const char* mes); 
    //------------------------- Private Data ---------------------------------------
    
    static uint8_t firstItem = 0;
    char errorBuffer[DISPLAY_ERROR_LINES][DISPLAY_CHARS_PER_ERROR + 1] = {0,0};  
    ConstStr::ConstString errorLine[DISPLAY_ERROR_LINES] = {0};
    boolean newMessage = false;
    
    //------------------------ Read only ------------------------------------------
    static const IndicatorData indies [] =
    {
     {"E", 200 ,23}, //Error
     {"N", 230 ,27}, //Network
     {"M", 170 ,19}, //Measurement
    };
    
    static const Display::TextGrid logMessageXOffset = 0;
    static const Display::TextGrid logMessageYOffset = 3;
    
    static const MeasurmentOutputSpace slots[] = 
    {
      {5,3,3,17},
      {25,3,83,17},
      {45,3,163,17},
      {5,6,3,35},
      {25,6,83,35},
      {45,6,163,35},
    };
    //------------------------------- Public Functions -----------------------------
    void setUpDisplay()
    {
        fillUnitNameArray();
        using namespace Display; 
        initSerialHW(B_9600);
        displayStartScreen(DISABLE);
        setCursor(DISABLE);
        
        for(uint8_t index = 0; index < DISPLAY_ERROR_LINES; index ++)
            ConstStr::initConstString(&errorLine[index],errorBuffer[index],DISPLAY_CHARS_PER_ERROR + 1);
        
        Serial.println(F("2. display Setup"));
        
        prepareStaticDisplay();
    }

    boolean isNewLogInfo()
    {
        return newMessage;
    }

    void showLogMessageScreen()
    {
        using namespace Display;   
        clearDisplay();        

        drawHorizontalBar(12, 0);
        setDisplayMode(NOT);
        setFontType(Default);

        setAndWriteFString(0, 0, F("Log Output"));
        setFontType(Font1);
        setDisplayMode(OVERWRITE); 
        for(uint8_t index = 0; index < DISPLAY_ERROR_LINES; index ++) 
        {  
             int32_t arIndex = (firstItem - 1 - index);
             if(arIndex < 0)
                 arIndex += DISPLAY_ERROR_LINES;
             printErrorLine(index, ConstStr::getString(&errorLine[arIndex]));
        }
            
        newMessage = false;
    }
    
    ConstStr::ConstString* getFreeErrorString()
    {
        ConstStr::ConstString* res = &errorLine[firstItem];
        firstItem += 1;
        if(firstItem >= DISPLAY_ERROR_LINES)
            firstItem = 0;
        ConstStr::clear(res); 
        newMessage = true;
        return res;
    }
    
    
    void prepareStaticDisplay()
    {
        using namespace Display;
        clearDisplay();
        drawHorizontalBar(12, 0);
        setDisplayMode(NOT);
        setFontType(Default);

        setAndWriteFString(0, 0, F("Sensor Board"));
        setUpIndicator(&(indies[0]));
        setUpIndicator(&(indies[1]));
        setUpIndicator(&(indies[2]));
    }
    
    void showIndicator(Indicator b)
    {
         using namespace Display;
         setDisplayMode(OR);
         drawFilledCircle(indies[b].pixCord, 5, 3);
    }
    
    void hideIndicator(Indicator b)
    {
         using namespace Display;
         setDisplayMode(NOT);
         drawFilledCircle(indies[b].pixCord, 5, 3);
    }
    
    void setUpMeasurementWarning(MeasurementOutputSpace a)
    {
        using namespace Display;
        Image b = loadWarning();
        drawImg(slots[a].xp, slots[a].yp, b.width, b.height, b.data, b.size);
    }
    
    void setUpMeasurementError(MeasurementOutputSpace a)
    {
        using namespace Display;
        Image b = loadError();
        drawImg(slots[a].xp, slots[a].yp, b.width, b.height, b.data, b.size);
    }
    
    void clearMeasurmentIcon(MeasurementOutputSpace a)
    {
       using namespace Display;
       clearArea(slots[a].xp, slots[a].yp, 14, 14);
    }
    
    void updatedepthMeasurementOutputSpace(MeasurementOutputSpace a, const Sensor::SensorStringInformation* data, int32_t value)
    {
        using namespace Display;
        if(data == NULL)
            return;
       
        setFontType(Font1);    
        setDisplayMode(OVERWRITE);
        setAndWriteFString(slots[a].x, slots[a].y, data->desc);
        writeInt(value);
        writeFString(getUnitString(data->unit));
    }
    //------------------------------ Private Functions -----------------------------
    static inline void setUpIndicator(const IndicatorData* data)
    {
        using namespace Display;
        setAndWriteString(data-> gridCord, 0, data->desc);
        drawFilledCircle(data->pixCord, 5, 4);
    }
    
    static inline void printErrorLine(uint8_t index, const char* mes)
    {
        Display::setAndWriteString(logMessageXOffset, logMessageYOffset + index, mes);
    }
}
