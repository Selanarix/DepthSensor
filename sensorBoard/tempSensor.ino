#include "tempSensor.h"
#include "logger.h"
#include "sensorErrorTypes.h"
#include "hal.h"

namespace TemperatureSensor
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    void readLM35(double* mes, const void*);
    void readTMP_102(double* mes, const void*);
    TestSeries::TestSeriesCheckResult testTestSeries(const TestSeries::TestSeries* s, const Sensor::SensorConstraints* con);
    bool testEvaluatedValue(const TemperatureSensor* sen,const Temperature dep);
    
    static void logTemperaturTestSeriesError(SensorError::TestSeriesError ab);
    static void logAverageTemperaturErrors(SensorError::AverageMeasurementError b);
    
    //------------------------ Private Data ----------------------------------------
    //------------------------ Read only -------------------------------------------
    const char* sensorNames [] = 
    {
        "LM35",
        "TMP_102"
    };
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
        tSen->type = t;    
        tSen->digitalSensor = NULL;
        switch(t)
        {
            case LM35:
                return TestSeries::construct(&(tSen->series),  controll, readLM35, testTestSeries, size);  
                
            case TMP102:
                return true;
        }              
        return false; 
    }
    
    bool constructDigital(TemperatureSensor* tSen, 
                    const Sensor::SensorConstData* constDa, 
                    const TestSeries::TestSeriesControll* controll,
                    const Sensor::SensorConstraints* cons, 
                    TemperatureSensorType t, uint32_t size, TMP102::TMP102* sen)
    {
        if(sen == NULL)
            return false;
            
        if(!construct(tSen,constDa,controll, cons, t,size))
            return false;
            
        tSen->digitalSensor = sen;
        
        return TestSeries::construct(&(tSen->series),  controll, readTMP_102, testTestSeries, size);
    }
    
    const Sensor::SensorStringInformation sensorGetStringInfo()
    {
       Sensor::SensorStringInformation res
       {
          F("Temp: "),
          UNIT_degC
       };
       return res;
    }
   
    uint8_t getSensorID(const TemperatureSensor* con)
    {
         if(con == NULL)
              return 0;
         return con->constData->ID;
    }
    
    Temperature getLastTemperature(const TemperatureSensor* con)
    {
         if(con == NULL)
              return 0;
         return con->lastTemperature;
    }
    
    void initTemperatureSensorHW(const TemperatureSensor* con)
    {
        Logger::log(Logger::INFO, F("temperatur sensor initialized"));
    }
    
    Sensor::MeasurementResult measureTemperature(TemperatureSensor* tSen)
    {
        if(tSen == NULL)
        {
            Logger::log(Logger::ERROR,F("Can not measure for null instance of temperatur sensor"));
            return Sensor::MeasurementError;
        }    
        Logger::printSeperator();//------------------------------------------
        Logger::logString(Logger::INFO, F("Measure temperature sensor: "), sensorNames[tSen->type]);
        Logger::logInt(Logger::DEBUG, F("Sensor id: "),(uint32_t)tSen->constData->ID);
        TestSeries::TestSeriesCheckResult res = TestSeries::takeTestSeries(&(tSen->series), tSen->constrains, tSen);
        
        if(res != TestSeries::TestSeriesOK)
        {
           Logger::log(Logger::ERROR,F("Could not measure temperature"));
           Logger::printSeperator();//------------------------------------------
           tSen->lastTemperature = 0.0;
           return Sensor::MeasurementError;
        }
        Sensor::MeasurementResult result = Sensor::MeasurementOK;
        Temperature avgTemperaturOfSeries = (Temperature)TestSeries::getAverageMeanOfSeries(&(tSen->series));
       
        //Keep sensor value but generate callback if not as acpected. 
        if(!testEvaluatedValue(tSen,avgTemperaturOfSeries))
            result = Sensor::MeasurementValueUnexpected;
    
        Logger::logInt(Logger::INFO, F("Temperatur [C]: "), avgTemperaturOfSeries);
        Logger::printSeperator();//------------------------------------------
        tSen->lastTemperature = avgTemperaturOfSeries;
        return result;
    }
    
    
    //------------------------------ Private Functions -----------------------------
    void readLM35(double* mes, const void* ob)
    {
       if(ob == NULL)
           return;
     
        const TemperatureSensor* thi = (TemperatureSensor*)ob;
        
        double sensor_mV = (double)HAL::analogReadVoltage(thi->constData->PIN);
        *mes = (sensor_mV*-0.485)+3200; //amplified->non-amplified
        *mes = (*mes-909)/10.0; //minus diode offset and 10mV/degree
    }  
    
    void readTMP_102(double* mes, const void* ob)
    {
        if(ob == NULL)
            return;
      
        const TemperatureSensor* thi = (TemperatureSensor*)ob;
        if(thi->digitalSensor == NULL)
            return;
    
        *mes = TMP102::simpleRead(thi->digitalSensor);
    }  
    
    TestSeries::TestSeriesCheckResult testTestSeries(const TestSeries::TestSeries* s, const Sensor::SensorConstraints* con)
    { 
        if(s == NULL || con == NULL)
            return TestSeries::TestSeriesCancelMeasurement;
    
        using namespace SensorError;

        TestSeriesError errorTypes = TestSeriesOK;
        TestSeries::MinMax res = TestSeries::evaluateMinMaxOfTestSeries(s);
    
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
        
        boolean res = true;
        const Sensor::SensorConstraints* con = sen->constrains;
       
        SensorError::AverageMeasurementError errorCode = SensorError::AverageMeasurmentOK;
        Temperature depthDiff;
    
        //Calc diff between new and last Depth
        if(dep > sen->lastTemperature)
            depthDiff = dep - sen->lastTemperature;
        else	
            depthDiff = sen->lastTemperature - dep; 
        
        if(sen->lastTemperature != 0 && depthDiff >  con->MAX_ALLOWED_AVERAGED_VALUE_CHANGE)
        {
            errorCode = SensorError::AverageMeasurmentNotInRange;
            res = false;        
        }
        logAverageTemperaturErrors(errorCode); //log errors
    
        return res;
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
