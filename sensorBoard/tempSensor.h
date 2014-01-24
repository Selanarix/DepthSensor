
#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "sensor.h"

namespace TemperatureSensor 
{
  
  typedef unsigned int Temperature;
  
  const Temperature MINIMAL_EXPECTED_TEMPERATURE_SENSOR_VALUE 	 	= 0;
  const Temperature MAXIMAL_EXPECTED_TEMPERATURE_SENSOR_VALUE	 	= 0xffffffff;
  const Temperature MAX_ALLOWED_AVERAGED_TEMPERATURE_VALUE_CHANGE	        = 0xffffffff;
  const Temperature ALLOWED_TEMPERATURE_TEST_SERIES_VARIATION 	 	= 0xffffffff;
  
  void initTemperatureSensorHW();
  Sensor::SensorMeasurmentResult measureTemperature();
  Temperature getLastMeasurement();
}

#endif /* DEPTHSENSOR_H_ */
