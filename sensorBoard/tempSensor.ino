#include "tempSensor.h"
#include "logger.h"
#include "sensorErrorTypes.h"

namespace TemperatureSensor
{
    
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    
    void readLM35(double* mes);
    TestSeries::TestSeriesCheckResult testTestSeries(const void* s);
    bool testEvaluatedValue(const TemperatureSensor* sen,const Temperature dep);
    
    static void logTemperaturTestSeriesError(SensorError::TestSeriesError ab);
    static void logAverageTemperaturErrors(SensorError::AverageMeasurementError b);
    
    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    //------------------------ Public Functions ------------------------------------
    
    bool construct(TemperatureSensor* tSen, 
                    const Sensor::SensorConstData* constDa, 
                    const TestSeries::TestSeriesControll* controll,
                    const Sensor::SensorConstraints* cons, 
                    TemperatureSensorType t, uint32_t size)
    {
        if(tSen == NULL || constDa == NULL || controll == NULL || cons == NULL)
            return false;
    
        tSen->lastTemperature = 0.0;
        tSen->constrains = cons;
        tSen->constData = constDa;    
        switch(t)
        {
            case LM35:
                return TestSeries::construct(&(tSen->series),  controll, readLM35, testTestSeries, size);                  
            break;
        }              
        return false; 
    }
    
    Temperature getLastMeasurementF(const TemperatureSensor* con)
    {
         if(con == NULL)
              return 0;
         return con->lastTemperature;
    }
    
    void initADC_PIN(const TemperatureSensor* con)
    {
       // HAL::(con->getPin((Sensor::Sensor*)con),INPUT);
        Logger::log(Logger::INFO, F("temperatur sensor initialized"));
    }
    
    Sensor::MeasurementResult measureTemperatureF(TemperatureSensor* tSen)
    {
        if(tSen == NULL)
        {
            Logger::log(Logger::ERROR,F("Can not measure for null instance of temperatur sensor"));
            return Sensor::MeasurementError;
        }    
        Logger::log(Logger::INFO, F("---------------------------------------"));
        Logger::logInt(Logger::INFO, F("Start with test series for temperature sensor with id: "),(uint32_t)tSen->constData->ID);
        TestSeries::TestSeriesCheckResult res = TestSeries::takeTestSeries(&(tSen->series));
        
        if(res != TestSeries::TestSeriesOK)
        {
           Logger::log(Logger::ERROR,F("Could not measure temperature"));
           Logger::log(Logger::INFO, F("---------------------------------------"));
           tSen->lastTemperature = 0.0;
           return Sensor::MeasurementError;
        }
        Sensor::MeasurementResult result = Sensor::MeasurementOK;
        Temperature avgTemperaturOfSeries = TestSeries::getAverageMeanOfSeries(&(tSen->series));
       
        //Keep sensor value but generate callback if not as acpected. 
        if(!testEvaluatedValue(tSen,avgTemperaturOfSeries))
            result = Sensor::MeasurementValueUnexpected;
    
        Logger::logInt(Logger::INFO, F("Temperatur [C]: "), avgTemperaturOfSeries);
        Logger::log(Logger::INFO, F("---------------------------------------"));
        tSen->lastTemperature = avgTemperaturOfSeries;
        return result;
    }
    
    
    //------------------------------ Private Functions -----------------------------
    void readLM35(double* mes)
    {
        static int a = 0;
        *mes = a++;
     /*   double tempvalue = (double)analogRead(tempSensorPin);
        Logger::logDouble(Logger::INFO, "Temp [C]: ",(5000.0 /1024.0 * tempvalue / 10.0));
        return  (temperatur)(5000.0 /1024.0 * tempvalue / 10.0);
     */
    }  
    
    TestSeries::TestSeriesCheckResult testTestSeries(const void* s)
    { 
        if(s == NULL)
            return TestSeries::TestSeriesCancelMeasurement;
    
        using namespace SensorError;
    
        const Sensor::SensorConstraints* con = ((TemperatureSensor*) s)->constrains;
    
        TestSeriesError errorTypes = TestSeriesOK;
        TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(&(((TemperatureSensor*)s)->series));
    
        double meanVariation = res.max - res.min;
        if(res.min < 0.0001 && res.max < 0.0001)
            errorTypes = SensorOutOfFunction;
        else if(res.min < con->MINIMAL_EXPECTED_SENSOR_VALUE)
            errorTypes = TestSeriesUnderMinRange;
        else if(res.max > con->MAXIMAL_EXPECTED_SENSOR_VALUE)
            errorTypes = TestSeriesAboveMaxRange;
        else if(meanVariation > con->ALLOWED_TEST_SERIES_VARIATION)
            errorTypes = TestSeriesMeanVariationToBig;
    
        logTemperaturTestSeriesError(errorTypes); //log errors
    
        if(errorTypes == SensorOutOfFunction)
            return TestSeries::TestSeriesCancelMeasurement; //Cancel further       
        if(errorTypes != TestSeriesOK)
            return TestSeries::TestSeriesInvalid;
            
        return TestSeries::TestSeriesOK;
    }
    
    bool testEvaluatedValue(const TemperatureSensor* sen, const Temperature dep)
    {
        if(sen == NULL)
        {
            Logger::log(Logger::WARNING,F("Could not test new temperature because no constraint defined"));        
            return false;
        }   
    
        const Sensor::SensorConstraints* con = sen->constrains;
       
        bool res = true;
        SensorError::AverageMeasurementError errorCode = SensorError::AverageMeasurmentOK;
        Temperature depthDiff;
    
        //Calc diff between new and last Depth
        if(dep > sen->lastTemperature)
            depthDiff = dep - sen->lastTemperature;
        else	
            depthDiff = sen->lastTemperature - dep; 
        
        if(sen->lastTemperature != 0 && depthDiff >  con->MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
            errorCode = SensorError::AverageMeasurmentNotInRange;
    
        logAverageTemperaturErrors(errorCode); //log errors
    
        return true;
    }
    
    //-------------------------------------- E r r o r s ------------------------
    
    static void logTemperaturTestSeriesError(SensorError::TestSeriesError ab)
    {
      using namespace SensorError;
      switch(ab)
      {
          case TestSeriesOK:
            return;
          case SensorOutOfFunction:
              Logger::log(Logger::ERROR, F("Sensor seams to be out of function. Only 0 measures"));
          break;
          case TestSeriesUnderMinRange:
              Logger::log(Logger::ERROR, F("On of the sensor's values is out of minimal range"));
          break;
          case TestSeriesAboveMaxRange:
              Logger::log(Logger::ERROR, F("On of the sensor's values is out of maximal range"));
          break;
          case TestSeriesMeanVariationToBig:
              Logger::log(Logger::ERROR, F("Mean variaion of sensor's values is out of range"));
          break;
          default:
              Logger::log(Logger::ERROR, F("General error in test series of depth sensor"));
      }
    }
    
    static void logAverageTemperaturErrors(SensorError::AverageMeasurementError b)
    {
        using namespace SensorError;
        switch(b)
        {
            case AverageMeasurmentOK:
                return;
            case AverageMeasurmentNotInRange:
                Logger::log(Logger::ERROR, F("Average measurement was out of defined range"));
            break;
            default:
                Logger::log(Logger::ERROR, F("General error when testing average depth measurement."));
       
        }
    }
}
