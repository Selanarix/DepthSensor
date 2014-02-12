#include "logger.h"  

namespace Logger
{
    //-------------------------- Private Types -------------------------------------
     
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------
    LogLevel outputLevel = INFO;
    
    //------------------------ Read only ------------------------------------------
    const uint32_t serialBaudRate = 9600;
    

    const char* logLevelNames[] = 
    {
        "DEBUG",
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
        if(level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.println(message);
    }

    void changeOutputLogLevel(LogLevel level)
    {
        if(level > ERROR)
            return;
        outputLevel = level;
    }

    void logInt(LogLevel level, const __FlashStringHelper* message, unsigned int number )
    {
        if(level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }

    void logDouble(LogLevel level, const __FlashStringHelper* message, double number )
    {
        if(level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }
    
    void logString(LogLevel level, const String str)
    {
        if(level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(str);
    }
    //------------------------------ Private Functions -----------------------------

}
