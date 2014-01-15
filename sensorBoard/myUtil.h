
#ifndef SENSOR_UTILS_H_
#define SENSOR_UTILS_H_

namespace Utils
{
  typedef struct
  {
      double min;
      double max;
  } MinMax;
  
  MinMax evaluateMinMax(const double* series, unsigned int size);
}
#endif
