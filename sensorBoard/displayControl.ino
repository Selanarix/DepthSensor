#include "displayControl.h"
#include "display.h"
#include "logger.h"
#include "generalTypes.h"

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
    } MeasurmentOutputSpace;
    
    //-------------------- Private Function Prototypes -----------------------------
    static inline void setUpIndicator(const IndicatorData* data);
    //------------------------- Private Global Data ---------------------------------------
    //------------------------ Read only ------------------------------------------
    static const IndicatorData indies [] =
    {
     {"E", 200 ,23}, //Error
     {"N", 230 ,27}, //Network
     {"M", 170 ,19}, //Measurement
    };
    
    static const MeasurmentOutputSpace slots[] = 
    {
      {0,3},
      {20,3},
      {40,3},
      {0,6},
      {20,6},
      {40,6},
    };
    //------------------------------- Public Functions -----------------------------
    void setUpDisplay()
    {
        Logger::changeLoggerStatus(DISABLE);
        fillUnitNameArray();
        using namespace Display; 
        initSerialHW(B_9600);
        displayStartScreen(DISABLE);
        clearDisplay();
        setCursor(DISABLE);
        prepareStaticDisplay();
    }

    void prepareStaticDisplay()
    {
        using namespace Display;
        drawHorizontalBar(12, 0);
        setDisplayMode(NOT);
        setFontType(Default);

        setAndWriteString(0, 0, "Sensor Board");

        setUpIndicator(&indies[0]);
        setUpIndicator(&indies[1]);
        setUpIndicator(&indies[2]);
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
        setAndWriteString(slots[a].x, slots[a].y+1, "23 min old");
        Serial.println();
    }
    //------------------------------ Private Functions -----------------------------
    static inline void setUpIndicator(const IndicatorData* data)
    {
        using namespace Display;
        setAndWriteString(data-> gridCord, 0, data->desc);
        drawFilledCircle(data->pixCord, 5, 4);
    }
}
