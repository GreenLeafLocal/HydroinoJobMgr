/*
  ReadAirTemp.h

*/

#ifndef ReadAirTemp_h
#define ReadAirTemp_h

#include <Arduino.h>
#include <DHT.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define DHTTYPE DHT22
#define MAX_RETRIES 2
#define READ_DELAY_MILLIS 1000

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadAirTemp: public Job {
  public:     
    // Constructors    
    ReadAirTemp (int sensorId);
    ~ReadAirTemp();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    DHT *_dhtSensor;
    float _sensorValue;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
};
#endif
