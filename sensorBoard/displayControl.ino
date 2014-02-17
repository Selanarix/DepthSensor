#include "displayControl.h"
#include "display.h"
#include "logger.h"

namespace DisplayControl
{
    //-------------------------- Private Types -------------------------------------
    typedef struct
    {
       const char* desc;
       Display::Coord pixCord;
       Display::TextGrid gridCord;
    } IndicatorData;
    
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
    //------------------------------- Public Functions -----------------------------
    void setUpDisplay()
    {
        Logger::changeLoggerStatus(DISABLE);
        using namespace Display; 
        initSerialHW(B_9600);
        clearDisplay();
        prepareStaticDisplay();
    }

    void prepareStaticDisplay()
    {
        using namespace Display;
        drawHorizontalBar(12, 0);
        setDisplayMode(NOT);
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
    //------------------------------ Private Functions -----------------------------
    static inline void setUpIndicator(const IndicatorData* data)
    {
        using namespace Display;
        setAndWriteString(data-> gridCord, 0, data->desc);
        drawFilledCircle(data->pixCord, 5, 4);
    }
}
