/*
  ReadMoisture.h
*/

#ifndef ReadMoisture_h
#define ReadMoisture_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define DHTTYPE DHT22
#define MAX_RETRIES 2
#define READ_DELAY_MILLIS 1000
#define MIN_MOISTURE_VALUE 0 // used for mapping actual ready to a standard scale
#define MAX_MOISTURE_VALUE 10 // used for mapping actual ready to a standard scale

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadMoisture: public Job {
  public:     
    // Constructors    
    ReadMoisture (int sensorId, float calibrationValue, float additionalParam);
    ~ReadMoisture();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    float _sensorValue;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
    float     _calibrationValue;
    int       _moisturePowerPin;
};
#endif
