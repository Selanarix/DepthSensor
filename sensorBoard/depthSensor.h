
#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

#include "sensor.h"

namespace DepthSensor 
{
  typedef unsigned int Depth;
  
  const Depth MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE 	 	= 0x10;
  const Depth MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE	 	= 0xffffffff;
  const Depth MAX_ALLOWED_AVERAGED_DEPTH_VALUE_CHANGE	        = 0xffffffff;
  const Depth ALLOWED_DEPTH_TEST_SERIES_VARIATION 	 	= 0xffffffff;
  
  Depth getLastMeasurement();
  void initDepthSensorHW();
  Sensor::SensorMeasurmentResult measureDepth();
}
#endif /* DEPTHSENSOR_H_ */
