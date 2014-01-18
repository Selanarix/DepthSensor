#ifndef LOGGER_H_
#define LOGGER_H_

#include <Arduino.h>

namespace Logger
{
    typedef enum
    {
      INFO,
      WARNING,
      ERROR  
    } LogLevel;

    void initLogger();
    
    void log(LogLevel level, const char* message);
    void logInt(LogLevel level, const char* message, unsigned int number );
    void logDouble(LogLevel level, const char* message, double number );
    void logString(LogLevel level, const String str);
}
#endif