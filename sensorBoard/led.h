
#ifndef OWN_LED_H_
#define OWN_LED_H_

namespace ProjectLED
{
  typedef enum
  {
      LED0 = 3,  
  } LED;
  
  void initLedPins();
  void LED_On(LED led);
  void LED_Off(LED led);
  void LED_Toggle(LED led);
}
#endif
