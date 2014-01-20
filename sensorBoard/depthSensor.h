
#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

#define AMOUNT_OF_DEPTH_SPOT_TESTS 10
namespace DepthSensor 
{
  typedef unsigned int depth;
  
  const depth MINIMAL_EXPECTED_DEPTH_SENSOR_VALUE 	 	= 0;
  const depth MAXIMAL_EXPECTED_DEPTH_SENSOR_VALUE	 	= 0xffffffff;
  const depth MAX_ALLOWED_AVERAGED_DEPTH_VALUE_CHANGE	        = 0xffffffff;
  const depth ALLOWED_DEPTH_TEST_SERIES_VARIATION 	 	= 0xffffffff;
  
  
  void initDepthSensorHW();
  depth measureDepth();
}
#endif /* DEPTHSENSOR_H_ */
