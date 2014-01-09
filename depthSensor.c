#include "depthSensor.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum
{
	TestSeriesOK,
	SensorOutOfFunction,
	TestSeriesUnderMinRange,
	TestSeriesAboveMaxRange,
	TestSeriesMeanVariationToBig
} TestSeriesTestResult;

typedef enum
{
	AverageMeasurmentOK,
	AverageMeasurmentNotInRange
} AverageMeasuementTestResult;

typedef struct
{
	depth min;
	depth max;
} MinMax;

const uint32_t TEST_SERIES_RETRIES_IF_ERROR = 10;
const uint32_t AVG_MEASUREMENT_RETRIES_IF_ERROR = 2;
const uint32_t MEASUREMENT_RETRIE_DELAY_IN_MSEC = 2000;
const uint32_t TEST_SERIES_MEASURMENT_DELAX_IN_MSEC = 100;
const void (*testSeriesErrorCallback)(AverageMeasuementTestResult) = 0;
const void (*SensorValuesOutOfCourseCallback)(TestSeriesTestResult) = 0;

void initSensorsPart();
void initNetworkStack();

void takeTestSeries();
TestSeriesTestResult testTestSeries();
void evaluateTestSeries();
AverageMeasuementTestResult testEvaluatedValue(const depth depth);
inline void measureDepth(depth* measurementOfSeries);

void delay(uint32_t micros);
MinMax evaluateMinMax(const depth* series, uint32_t size);
depth testSeries[AMOUNT_OF_SPOT_TESTS] = {0};

//---------------------------------- H W - I n i t --------------------------
/**
 * Init Hw components of the project
 */
void initHW()
{
	initSensorsPart();
	initNetworkStack();
}

void initSensorsPart()
{
	//TODO: //Hier fehlt noch was
}

void initNetworkStack()
{
	//TODO: //Hier fehlt noch was
}

//---------------------------------- M e a s u r e --------------------------
depth measureDepth()
{
	uint32_t seriesRetries;
	uint32_t meausreRetries;
	depth avgDepthOfSeries = 0;


	TestSeriesTestResult seriesTestResult = SensorOutOfFunction;

	//Try to get a valid test serie of depth measurments
	for(seriesRetries = 0; seriesTestResult != TestSeriesOK &&
			       seriesRetries < TEST_SERIES_RETRIES_IF_ERROR;
	seriesRetries++)
	{
		takeTestSeries();
		seriesTestResult = testTestSeries();
		if(seriesTestResult != TestSeriesOK)
			delay(MEASUREMENT_RETRIE_DELAY_IN_MSEC);
	}
	//Could not build a vlaid test series
	if(seriesTestResult != TestSeriesOK)
	{
		if(testSeriesErrorCallback != 0)
			testSeriesErrorCallback(seriesTestResult);
		return 0;
	}

	//Process data out of test series
	avgDepthOfSeries = evaluateTestSeries();
	averageSensorTestResult = testEvaluatedValue(avgDepthOfSeries);
	//Keep sensor value but generate callback if not as acpected. 
	if(averageSensorTestResult != AverageMeasurmentOK)
	{
		if(SensorValuesOutOfCourseCallback != 0)
			SensorValuesOutOfCourseCallback(averageSensorTestResult);
	}
	return avgDepthOfSeries;
}

void takeTestSeries()
{
	uint32_t measurement;
	for(measurement = 0; measurement < AMOUNT_OF_SPOT_TESTS; measurement ++)
	{
		measureDepth(&(testSeries[measurement]));
		delay(TEST_SERIES_MEASURMENT_DELAX_IN_MSEC);	
	}		
}

inline void measureDepth(depth* measurementOfSeries)
{
	//TODO: //Hier fehlt noch was
	//read value from sensor and assign it to measurementOfSeries
	*measurementOfSeries = 0;
}

TestSeriesTestResult testTestSeries()
{
	MinMax res = evaluateMinMax(testSeries,AMOUNT_OF_SPOT_TESTS);

	depth meanVariation = res.max - res.min;
	if(meanVariation == 0)
		return SensorOutOfFunction;
	if(res.min < MINIMAL_EXPECTED_SENSOR_VALUE)
		return TestSeriesUnderMinRange;
	if(res.max > MAXIMAL_EXPECTED_SENSOR_VALUE)
		return TestSeriesAboveMaxRange;
	if(meanVariation < ALLOWED_TEST_SERIES_VARIATION)
		return TestSeriesMeanVariationToBig;

	return TestSeriesOK;
}

depth evaluateTestSeries()
{
	double avg = 0;
	int t = 1;
	uint32_t measurement;

	for(measurement = 0; measurement < AMOUNT_OF_SPOT_TESTS; measurement ++)
	{
		double x = (double)testSeries[measurement];
		avg += (x - avg) / t;
		t++;
	}
	return (depth)avg;
}

AverageMeasuementTestResult testEvaluatedValue(const depth dep)
{
	static depth depthHistory = 0;
	
	AverageMeasuementTestResult res = AverageMeasurmentOK;
	depth depthDiff;

	//Calc diff between new and last depth
	if(dep > depthHistory)
		depthDiff = dep - depthHistory;
	else	
		depthDiff = depthHistory - dep; 

	if(depthHistory != 0 && depthDiff > MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
		res = AverageMeasurmentNotInRange;

	depthHistory = dep;	
	return res;
}


//-------------------------------------- U t i l l s ------------------------
void delay(uint32_t micros)
{
	//TODO: //Hier fehlt noch was
}

MinMax evaluateMinMax(const depth* series, uint32_t size)
{
	MinMax res;
	res.min = res.max = 0;
	for(; size > 0; size --)
	{
			if(series[size] < res.min)
				res.min = series[size];
			else if(series[size] > res.max)
				res.max = series[size];
	}
	return res;
}
//---------------------------------- S e n d  D a t a -----------------------
void processData()
{

}

int main()
{
	initHW();

	while(true)
	{
		measureDepth();
		processData();
	}
}
