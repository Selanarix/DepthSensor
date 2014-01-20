
#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#define a 10

namespace TemperaturSensor 
{
  
  typedef unsigned int temperatur;
  
  void initTemperaturSensorHW();
  temperatur measureTemperatur();
}

#endif /* DEPTHSENSOR_H_ */
