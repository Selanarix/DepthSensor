#ifndef LOGGER_H_
#define LOGGER_H_

#include "generalTypes.h"
#include <Arduino.h>

namespace Logger
{
    typedef enum
    {
      DEBUG,
      INFO,
      WARNING,
      ERROR 
    } LogLevel;

    void initLogger();
    void changeLoggerStatus(FunctionStatus status);
    void changeOutputLogLevel(LogLevel level);
    void log(LogLevel level, const __FlashStringHelper* message);
    void printSeperator();
    void logInt(LogLevel level, const __FlashStringHelper* message, unsigned int number );
    void logDouble(LogLevel level, const __FlashStringHelper* message, double number );
    void logOString(LogLevel level, const String str);
    void logString(LogLevel level, const __FlashStringHelper* message, const char* str);
}
#endif
