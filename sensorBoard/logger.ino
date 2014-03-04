#include "logger.h"  

namespace Logger
{
    //-------------------------- Private Types -------------------------------------
     
    //-------------------- Private Function Prototypes -----------------------------

    //------------------------- Private Data ---------------------------------------
    LogLevel outputLevel = INFO;
    bool enabled = true;
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
        if(!enabled)
            return;
        Serial.begin(serialBaudRate); 
    }
    void log(LogLevel level, const __FlashStringHelper* message)
    {
       if(!enabled || level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.println(message);
    }
    
    void changeLoggerStatus(FunctionStatus status)
    {
        if(status == ENABLE)
          enabled = true;
        else
          enabled = false;
    }

    void printSeperator()
    {
        if(enabled)
            Serial.println(F("---------------------------------------"));
    }

    void changeOutputLogLevel(LogLevel level)
    {
        if(level > ERROR)
            return;
        outputLevel = level;
    }

    void logInt(LogLevel level, const __FlashStringHelper* message, unsigned int number )
    {
       if(!enabled || level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }

    void logDouble(LogLevel level, const __FlashStringHelper* message, double number )
    {
       if(!enabled || level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }
    
    void logOString(LogLevel level, const String str)
    {
       if(!enabled || level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(str);
    }
    
    void logString(LogLevel level, const __FlashStringHelper* message, const char* str)
    {
       if(!enabled || level < outputLevel)
            return;
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(str);
    }
    //------------------------------ Private Functions -----------------------------

}
