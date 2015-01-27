#ifndef MY_RTC_H
#define MY_RTC_H
#include <Arduino.h>

boolean isTickOverrun();
void resetTick();

void setUpRealTimeClock();
boolean isRTC_FlagAndClear();
boolean isTimePeriodOver(uint32_t period);
void enterSleep(void);

#endif
