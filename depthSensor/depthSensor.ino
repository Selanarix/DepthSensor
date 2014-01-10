namespace sensor 
{
#include "depthSensor.h"  

int sensorPinDiff = A0;    // Input Vout of DiffSensor

  
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
const uint32_t MEASUREMENT_RETRIE_DELAY_IN_MSEC = 2000;
const uint32_t TEST_SERIES_MEASURMENT_DELAX_IN_MSEC = 10;
void callback1(TestSeriesTestResult);
void callback2(AverageMeasuementTestResult);
void (*testSeriesErrorCallback)(TestSeriesTestResult) = callback1;
void (*SensorValuesOutOfCourseCallback)(AverageMeasuementTestResult) = callback2;


void initSensorsPart();
void initNetworkStack();

void takeTestSeries();
TestSeriesTestResult testTestSeries();
depth evaluateTestSeries();
AverageMeasuementTestResult testEvaluatedValue(const depth depth);
inline void measureDepth(depth* measurementOfSeries);

MinMax evaluateMinMax(const depth* series, uint32_t size);
depth testSeries[AMOUNT_OF_SPOT_TESTS] = {0};

//---------------------------------- H W - I n i t --------------------------
/**
 * Init Hw components of the project
 */
void initHW()
{
        pinMode(sensorPinDiff, INPUT_PULLUP); 
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
	depth avgDepthOfSeries = 0;


	TestSeriesTestResult seriesTestResult = SensorOutOfFunction;

	//Try to get a valid test serie of depth measurments
	for(seriesRetries = 0; seriesTestResult != TestSeriesOK &&
			       seriesRetries < TEST_SERIES_RETRIES_IF_ERROR;
	seriesRetries++)
	{
		takeTestSeries();
		seriesTestResult = testTestSeries();
        //Serial.println(seriesTestResult);
		if(seriesTestResult != TestSeriesOK)
			delay(MEASUREMENT_RETRIE_DELAY_IN_MSEC);
	}
	//Could not build a valid test series
	if(seriesTestResult != TestSeriesOK)
	{
		if(testSeriesErrorCallback != 0)
			testSeriesErrorCallback(seriesTestResult);
		return 0;
	}

	//Process data out of test series
	avgDepthOfSeries = evaluateTestSeries();
	AverageMeasuementTestResult averageSensorTestResult = testEvaluatedValue(avgDepthOfSeries);
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
//                Serial.println(testSeries[measurement]);
		delay(TEST_SERIES_MEASURMENT_DELAX_IN_MSEC);	
	}		
}

inline void measureDepth(depth* measurementOfSeries)
{
	//TODO: //Hier fehlt noch was
	//read value from sensor and assign it to measurementOfSeries
        double pressure = 0.0;
        
//        pressure = (analogRead(sensor::sensorPinDiff)/5.0 - 0.04)/0.0018; //pressure in kPa
   //     pressure = (analogRead(sensor::sensorPinDiff);
        pressure = ((double)analogRead(sensor::sensorPinDiff))*(5000.0/1024.0);
        
        pressure = pressure - 260.0;
        Serial.print("Rohwert: ");
        Serial.println(pressure);
        pressure = (pressure/5000.0)/0.0018;
        Serial.print("Druck: ");
        Serial.println(pressure);
         if (pressure<0)
           pressure = 1.0;
	*measurementOfSeries = (depth)pressure;
}

TestSeriesTestResult testTestSeries()
{
	MinMax res = evaluateMinMax(testSeries,AMOUNT_OF_SPOT_TESTS);

	depth meanVariation = res.max - res.min;
 // Serial.println(res.max);
 // Serial.println(res.min);
	if(res.min == 0)
		return SensorOutOfFunction;
	if(res.min < MINIMAL_EXPECTED_SENSOR_VALUE)
		return TestSeriesUnderMinRange;
	if(res.max > MAXIMAL_EXPECTED_SENSOR_VALUE)
		return TestSeriesAboveMaxRange;
	if(meanVariation > ALLOWED_TEST_SERIES_VARIATION)
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

//-------------------------------------- E r r o r s ------------------------

void callback1(TestSeriesTestResult){
  Serial.println("Series Error");
}

void callback2(AverageMeasuementTestResult){
    Serial.println("Avrg Error");
}

//-------------------------------------- U t i l l s ------------------------

MinMax evaluateMinMax(const depth* series, uint32_t size)
{
	MinMax res;
	res.min = 0xffffffff;
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
//---------------------------------- S e n d  D a t a -----------------------
void processData(const depth d)
{
    //Serial.println("hello world!");
    Serial.println(d);

}


}

void setup(){
  Serial.begin(9600);  
  sensor::initHW();
}

void loop(){
		sensor::processData(sensor::measureDepth());
            
  delay(100);

}
