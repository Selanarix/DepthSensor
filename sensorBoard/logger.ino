#include "logger.h"  

namespace Logger
{
    //-------------------------- Private Types -------------------------------------
     
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------
    
    //------------------------ Read only ------------------------------------------
    const uint32_t serialBaudRate = 9600;
    
    const char* logLevelNames[] = 
    {
        "INFO",
        "WARNING",
        "ERROR"
    };
    //------------------------------- Public Functions -----------------------------
    void initLogger()
    {
        Serial.begin(serialBaudRate); 
    }
    void log(LogLevel level, const __FlashStringHelper* message)
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.println(message);
    }

    void logInt(LogLevel level, __FlashStringHelper* message, unsigned int number )
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }

    void logDouble(LogLevel level, __FlashStringHelper* message, unsigned int number )
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }
    
    void logString(LogLevel level, const String str)
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(str);
    }
    //------------------------------ Private Functions -----------------------------

}
