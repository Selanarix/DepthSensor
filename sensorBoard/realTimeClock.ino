#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "realTimeClock.h"

volatile int f_wdt=1;

ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {} //Overrun
}

void setUpRealTimeClock()
{
    /* Clear the reset flag. */
    MCUSR &= ~(1<<WDRF);
    
    /* In order to change WDE or the prescaler, we need to
     * set WDCE (This will allow updates for 4 clock cycles).
     */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    
    /* set new watchdog timeout prescaler value */
    WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
    
    /* Enable the WD interrupt (note no reset). */
    WDTCSR |= _BV(WDIE);
}

bool isRTC_FlagAndClear()
{
    if(f_wdt == 1)
    {
        f_wdt = 0;
        return true;
    }
    return false;
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);  
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}
