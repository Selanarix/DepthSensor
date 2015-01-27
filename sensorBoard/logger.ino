#include "logger.h"  
#include "displayControl.h"

namespace Logger
{
    //-------------------------- Private Types -------------------------------------
    typedef struct
    {
         void (*log_s)(LogLevel, const __FlashStringHelper*);
         void (*printSeperator_s)();
         void (*logInt_s)(LogLevel, const __FlashStringHelper*, int32_t);
         void (*logDouble_s)(LogLevel, const __FlashStringHelper*, double );
         void (*logOString_s)(LogLevel, const String str);
         void (*logString_s)(LogLevel, const __FlashStringHelper*, const char*);
         
    } Logger;
    //-------------------- Private Function Prototypes -----------------------------
#if ONBOARD_DISPLAY == 1    
    void displayLog(LogLevel level, const __FlashStringHelper* message);
    void displayLogInt(LogLevel level, const __FlashStringHelper* message, int32_t number );
    void displayLogDouble(LogLevel level, const __FlashStringHelper* message, double number );
    void displayLogOString(LogLevel level, const String str);
    void displayLogString(LogLevel level, const __FlashStringHelper* message, const char* str);
#else
    void serialLog(LogLevel level, const __FlashStringHelper* message);
    void serialPrintSeperator();
    void serialLogInt(LogLevel level, const __FlashStringHelper* message, int32_t number );
    void serialLogDouble(LogLevel level, const __FlashStringHelper* message, double number );
    void serialLogOString(LogLevel level, const String str);
    void serialLogString(LogLevel level, const __FlashStringHelper* message, const char* str);
#endif
    //------------------------- Private Data ---------------------------------------
    LogLevel outputLevel = INFO;
    
    Logger* logger;
    //------------------------ Read only ------------------------------------------
    const uint32_t serialBaudRate = 9600;
    
    const char* logLevelNames[] = 
    {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR"
    };
    
#if ONBOARD_DISPLAY == 1    
    Logger displayLogFuncSet = 
    {
        displayLog,
        NULL,
        displayLogInt,
        displayLogDouble,
        displayLogOString,
        displayLogString
    };
#else    
    Logger serialLogFuncSet = 
    {
        serialLog,
        serialPrintSeperator,
        serialLogInt,
        serialLogDouble,
        serialLogOString,
        serialLogString
    }; 
#endif
    //------------------------------- Public Functions -----------------------------
    void initLogger()
    {
#if ONBOARD_DISPLAY == 1
            logger = &displayLogFuncSet;   
#else
            logger = &serialLogFuncSet;
            Serial.begin(serialBaudRate); 
#endif
    }
    
    void changeOutputLogLevel(LogLevel level)
    {
        if(level > ERROR)
            return;
        outputLevel = level;
    }

    void log(LogLevel level, const __FlashStringHelper* message)
    {
        if(level < outputLevel)
            return;
        logger->log_s(level, message);
    }
 
    void printSeperator()
    {
         if(logger->printSeperator_s == NULL)
            logger->printSeperator_s();
    }


    void logInt(LogLevel level, const __FlashStringHelper* message, int32_t number )
    {
        if(level < outputLevel)
            return;
        logger->logInt_s(level, message, number);
    }

    void logDouble(LogLevel level, const __FlashStringHelper* message, double number )
    {
        if(level < outputLevel)
            return;        
        logger->logDouble_s(level, message, number);
    }
    
    void logOString(LogLevel level, const String str)
    {
        if(level < outputLevel)
            return;
        logger->logOString_s(level, str);
    }
    
    void logString(LogLevel level, const __FlashStringHelper* message, const char* str)
    {
        if(level < outputLevel)
            return;
        logger->logString_s(level, message, str);
    }

    //------------------------------ Private Functions -----------------------------
#if ONBOARD_DISPLAY == 1      
    void displayLog(LogLevel level, const __FlashStringHelper* message)
    {
        using namespace ConstStr;
        ConstString* str = DisplayControl::getFreeErrorString();
        addconstString(str,logLevelNames[level]);
        addChar(str,' ');
        addFString(str,message);
    }
    void displayLogInt(LogLevel level, const __FlashStringHelper* message, int32_t number )
    {
        using namespace ConstStr;
        ConstString* str = DisplayControl::getFreeErrorString();
        addconstString(str,logLevelNames[level]);
        addChar(str,' ');
        addFString(str,message);
        addLong(str,number,10);
    }
    void displayLogDouble(LogLevel level, const __FlashStringHelper* message, double number )
    {
        using namespace ConstStr;
        ConstString* str = DisplayControl::getFreeErrorString();
        addconstString(str,logLevelNames[level]);
        addChar(str,' ');
        addFString(str,message);
        addDouble(str,number,2);
    }
    void displayLogOString(LogLevel level, const String str)
    {
        using namespace ConstStr;
        ConstString* strra = DisplayControl::getFreeErrorString();
        addconstString(strra,logLevelNames[level]);
        addChar(strra,' ');
        addString(strra,str);
    }

    void displayLogString(LogLevel level, const __FlashStringHelper* message, const char* str)
    {
        using namespace ConstStr;
        ConstString* strra = DisplayControl::getFreeErrorString();
        addconstString(strra,logLevelNames[level]);
        addChar(strra,' ');
        addFString(strra,message);
        addString(strra,str);
    }
#else  
    void serialLog(LogLevel level, const __FlashStringHelper* message)
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.println(message);
    }
 
    void serialPrintSeperator()
    {
        Serial.println(F("---------------------------------------"));
    }


    void serialLogInt(LogLevel level, const __FlashStringHelper* message, int32_t number )
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }

    void serialLogDouble(LogLevel level, const __FlashStringHelper* message, double number )
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(number);
    }
    
    void serialLogOString(LogLevel level, const String str)
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(str);
    }
    
    void serialLogString(LogLevel level, const __FlashStringHelper* message, const char* str)
    {
        Serial.print(logLevelNames[level]);
        Serial.print(" : ");
        Serial.print(message);
        Serial.println(str);
    }
#endif
}
