#ifndef DISPLAY_CONTROL_H_
#define DISPLAY_CONTROL_H_

#include <Arduino.h>
#include "sensor.h"

namespace DisplayControl
{   
    typedef enum
    {
          ERROR,
          NETWORK,
          MEASUREMENT
     }Indicator;
  
    typedef enum
    {
         Space0,
         Space1,
         Space2,
         Space3,
         Space4,
         Space5 
    }MeasurementOutputSpace;
    
    void setUpDisplay();
    void prepareStaticDisplay();
    void showIndicator(Indicator b);
    void hideIndicator(Indicator b);
    void updatedepthMeasurementOutputSpace(MeasurementOutputSpace a, const Sensor::SensorStringInformation* data, int32_t value);
}
#endif
