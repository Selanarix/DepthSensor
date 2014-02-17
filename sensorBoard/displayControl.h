#ifndef DISPLAY_CONTROL_H_
#define DISPLAY_CONTROL_H_

#include <Arduino.h>

namespace DisplayControl
{   
    typedef enum
    {
          ERROR,
          NETWORK,
          MEASUREMENT
     }Indicator;
  
    void setUpDisplay();
    void prepareStaticDisplay();
    void showIndicator(Indicator b);
    void hideIndicator(Indicator b);
}
#endif
