#include "myUtil.h"

namespace Utils
{
    MinMax evaluateMinMax(const double* series, unsigned int size)
    {
	    MinMax res;
	    res.min = 1000000.0;
            res.max = 0;
            
	    for(uint32_t i=0; i<size; i++)
	    {
	        if(series[i] < res.min)
		    res.min = series[i];
                else if(series[i] > res.max)
		    res.max = series[i];
	    }
	    return res;
    }
}
