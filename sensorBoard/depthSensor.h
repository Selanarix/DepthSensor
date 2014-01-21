
#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

namespace DepthSensor 
{
  typedef unsigned int Depth;
  
  const Depth MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE 	 	= 0;
  const Depth MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE	 	= 0xffffffff;
  const Depth MAX_ALLOWED_AVERAGED_DEPTH_VALUE_CHANGE	        = 0xffffffff;
  const Depth ALLOWED_DEPTH_TEST_SERIES_VARIATION 	 	= 0xffffffff;
  
  
  void initDepthSensorHW();
  Depth measureDepth();
}
#endif /* DEPTHSENSOR_H_ */
