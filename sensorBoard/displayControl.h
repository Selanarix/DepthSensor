#ifndef DISPLAY_CONTROL_H_
#define DISPLAY_CONTROL_H_

#include <Arduino.h>
#include "sensor.h"

#define DISPLAY_ERROR_LINES 3
#define DISPLAY_CHARS_PER_ERROR 40

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
    boolean isNewLogInfo();
    void showLogMessageScreen();
    void prepareStaticDisplay();
    void showIndicator(Indicator b);
    void hideIndicator(Indicator b);
    void setUpMeasurementWarning(MeasurementOutputSpace a);
    void setUpMeasurementError(MeasurementOutputSpace a);
    void clearMeasurmentIcon(MeasurementOutputSpace a);
    void updatedepthMeasurementOutputSpace(MeasurementOutputSpace a, const Sensor::SensorStringInformation* data, int32_t value);
}
#endif
