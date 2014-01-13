#ifdef a


#include "depthSensor.h"  
#include <SPI.h>
#include <Ethernet.h>

//-------------------------- Private Types -------------------------------------
  
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


//-------------------- Private Function Prototypes -----------------------------

void initSensorsPart();
void initNetworkStack();

void takeTestSeries();
TestSeriesTestResult testTestSeries();
depth evaluateTestSeries();
AverageMeasuementTestResult testEvaluatedValue(const depth depth);
void readDepthSensor(depth* measurementOfSeries);

void callback1(TestSeriesTestResult);
void callback2(AverageMeasuementTestResult);

MinMax evaluateMinMax(const depth* series, uint32_t size);
void handleLAN();
void httpRequest(const depth d);

//------------------------- Private Data ---------------------------------------

static void (*testSeriesErrorCallback)(TestSeriesTestResult) = callback1;
static void (*SensorValuesOutOfCourseCallback)(AverageMeasuementTestResult) = callback2;

static EthernetClient client;

static depth testSeries[AMOUNT_OF_SPOT_TESTS] = {0};

//------------------------ Read only ------------------------------------------

const uint32_t TEST_SERIES_RETRIES_IF_ERROR = 10;
const uint32_t MEASUREMENT_RETRIE_DELAY_IN_MSEC = 2000;
const uint32_t TEST_SERIES_MEASURMENT_DELAX_IN_MSEC = 10;

/*
* Input Vout of DiffSensor
*/
const int sensorPinDiff = A0;

/* 
* Enter a MAC address for your controller below.
*/
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/*
* Set the static IP address to use if the DHCP fails to assign
*/
const IPAddress ip(95,175,153,209);
const IPAddress myDns(77,244,128,44);

/* if you don't want to use DNS (and reduce your sketch size)
* use the numeric IP instead of the name for the server:
*/
const IPAddress server(95,175,153,207);  // numeric IP for my Notebook (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

//------------------------------- Public Functions -----------------------------
/**
 * Init Hw components of the project
 */
void initHW()
{
    pinMode(sensorPinDiff, INPUT_PULLUP); 
	initSensorsPart();
	initNetworkStack();
}

/**
* Measures the depth by taking several measurments and calculates a
* average one. 
*/

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


/**
/* Process measured data
*/
void processData(const depth d)
{
    Serial.print("Mittelwert [cm]: ");
    Serial.println(d);
    
    //Send data per LAN to Web-Server...
    httpRequest(d);
}


//------------------------------ Private Functions -----------------------------

void initSensorsPart()
{
	//TODO: //Hier fehlt noch was
}

void initNetworkStack()
{
        // start the Ethernet connection:
        if (Ethernet.begin(mac) == 0) {
            Serial.println("Failed to configure Ethernet using DHCP");
            //congifure using IP address instead of DHCP
            Ethernet.begin(mac, ip, myDns);
        }
 
        // give the Ethernet shield a second to initialize:
        delay(1000);
        Serial.print("My IP address: ");
        Serial.println( Ethernet.localIP() );
}

void takeTestSeries()
{
	uint32_t measurement;
	for(measurement = 0; measurement < AMOUNT_OF_SPOT_TESTS; measurement ++)
	{
		readDepthSensor(&(testSeries[measurement]));
//                Serial.println(testSeries[measurement]);
		delay(TEST_SERIES_MEASURMENT_DELAX_IN_MSEC);	
	}		
}

void readDepthSensor(depth* measurementOfSeries)
{
	//read value from sensor and assign it to measurementOfSeries
        double pressure = 0.0;
        double adcvalue = 0.0;
        double sensorVoltageADC = 0.0;
        const double offset = 0.27;

        adcvalue = (double)analogRead(sensor::sensorPinDiff);

//Formel aus Datenblatt (in kPa): Vout = Vs*(0,0018*p+0,04)        
//Umgestellt nach p (in hPa!!): p=(Vout-0,2)/(9*10^-4)

        sensorVoltageADC = (adcvalue)*(5.0/1023.0);
        Serial.print("Rohwert [V]: ");
        Serial.println(sensorVoltageADC);
        
//        pressure = (sensorVoltageADC*10.0-(offset*10))*1000/9;
        pressure = (sensorVoltageADC-offset)*1111;
        Serial.print("Druck [hPa]: ");
        Serial.println(pressure);
         if (pressure <= 0.0001)
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

void handleLAN()
{
    static boolean lastConnected = false;            // state of the connection last time through the main loop
    // if there are incoming bytes available 
    // from the server, read them and print them:
    if (client.available()) 
    {
        char c = client.read();
        Serial.println(c);
    }
  
    // if the server's disconnected, stop the client:
    if (!client.connected() && lastConnected) 
    {
        Serial.println();
        Serial.println("disconnecting from server...");
        client.stop();
    }
    
    // store the state of the connection for next time
    lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest(const depth d) 
{
  // if there's a successful connection:
  if (client.connect(server, 80)) 
  {
      Serial.println("connecting...");
      // send the HTTP PUT request:
      client.print("PUT /latest.txt ");
      client.println(d);
  }
  else 
  {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

#endif
