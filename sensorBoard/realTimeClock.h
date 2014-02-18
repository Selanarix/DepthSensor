#ifndef MY_RTC_H
#define MY_RTC_H
#include <Arduino.h>

const uint32_t timeBase_s = 8; //Seconds

bool isTickOverrun();
void resetTick();

void setUpRealTimeClock();
bool isRTC_FlagAndClear();
void enterSleep(void);

#endif
