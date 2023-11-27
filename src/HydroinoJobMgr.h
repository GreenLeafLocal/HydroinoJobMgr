/*
  HydroinoJobMgr.h 
*/

#ifndef HydroinoJobMgr_h
#define HydroinoJobMgr_h

#include <Arduino.h>
#include <Array.h>
#include <ArxSmartPtr.h>
#include "Job.h"
#include "device_jobs/RunFan.h"
#include "device_jobs/RunServo.h"
#include "device_jobs/RunDevice.h"

#include "sensor_jobs/ReadAirHumidity.h"
#include "sensor_jobs/ReadAirTemp.h"
#include "sensor_jobs/ReadMoisture.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
// Queue constants
#define QUEUE_LEN 10

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/
// Byte array for param as an INT
union byteArrayAsLong{
  byte  byteArray[4];
  long longVal;
};

// Byte array for param as a FLOAT
union byteArrayAsFloat{
  byte  byteArray[4];
  float floatVal;
};

typedef Array<std::shared_ptr<Job>,QUEUE_LEN> JobQueueType;

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class HydroinoJobMgr {
  // Constructors
  public:
    HydroinoJobMgr(ResponseData *responseData);
    ~HydroinoJobMgr();
  
    // Methods
    int EnqueueJob(byte *receivedData);
    void DequeueJobAtIndex(int queueIndex);
    void ProcessJobQueue();
    
    // Accessors 
    int GetJobQueueMaxLength();    
    JobQueueType GetJobQueue();
  
  private:
    // vars  
    JobQueueType _jobQueue;
    ResponseData *_responseData;

    // methods
    int GetQueueIndexForNewJob(JobTypeEnum jobType, int devicePin, bool *jobAlreadyQueued);
    void InitializeNewJob(byte *receivedData, int queueIndex, JobTypeEnum jobType, int devicePin);
    void SetJobParams (byte *receivedData, int queueIndex, JobTypeEnum jobType);
};
#endif
