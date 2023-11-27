/**
 * @file HydroinoJobMgr.cpp
 * @author oc
 * @brief This file contains the implementation of the HydroinoJobMgr class.
 * 
 * The HydroinoJobMgr class is responsible for managing jobs in the Hydroino system,
 * including adding, processing, and managing the job queue.
 * 
 * @license Apache License 2.0
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include "HydroinoJobMgr.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
HydroinoJobMgr::HydroinoJobMgr(ResponseData *responseData) {
  _responseData = responseData;
}

// Blank Destructor
HydroinoJobMgr::~HydroinoJobMgr() {
}

/*------------------------------------------------
  Private methods
-------------------------------------------------*/
// Get the index of an empty spot in the job queue
int HydroinoJobMgr::GetQueueIndexForNewJob(JobTypeEnum jobType, int devicePin, bool *jobAlreadyQueued) {
  // Size of the queue translates to the next free index in the job queue
  int i = 0;
  int emptyQueueIndex = _jobQueue.size();

  // Is job already in the queue
  while (i < _jobQueue.size()) {
    if (_jobQueue[i]->GetType() == jobType && _jobQueue[i]->GetDevicePin() == devicePin) {
      #ifdef DEBUG
        Serial.println("Job already in the queue");
      #endif
      
      emptyQueueIndex = i;
      *jobAlreadyQueued = true;
      break;
    }

    i++;
  }

  // No empty slot?
  if (i == QUEUE_LEN)
    emptyQueueIndex = -1;

  return emptyQueueIndex;
}

void HydroinoJobMgr::InitializeNewJob (byte *receivedData, int queueIndex, JobTypeEnum jobType, int devicePin){
  // Add job type to queue -- SWITCH  statement didn't work here.  Not sure why
  if (jobType == JobTypeEnum::RunDevice) {
    byteArrayAsLong startValue;

    startValue.byteArray[0] = receivedData[6];
    startValue.byteArray[1] = receivedData[7];
    startValue.byteArray[2] = receivedData[8];
    startValue.byteArray[3] = receivedData[9];

    std::shared_ptr<RunDevice> runDevice(new RunDevice(devicePin, startValue.longVal));   
    _jobQueue.push_back(runDevice);
    
  }
  // Fan
  else if (jobType == JobTypeEnum::RunFan) {   
    std::shared_ptr<RunFan> runFan(new RunFan(devicePin));
    _jobQueue.push_back(runFan);

  }
  // Servo 
  else if (jobType == JobTypeEnum::RunServo) {
    std::shared_ptr<RunServo> runServo(new RunServo(devicePin));
    _jobQueue.push_back(runServo);

  } 
  // Air humidity sensor
  else if (jobType == JobTypeEnum::ReadAirHumidity) {
    std::shared_ptr<ReadAirHumidity> readAirHumidity(new ReadAirHumidity(devicePin));
    _jobQueue.push_back(readAirHumidity);

  } 
  // Air temp sensor
  else if (jobType == JobTypeEnum::ReadAirTemp) {
    std::shared_ptr<ReadAirTemp> readAirTemp(new ReadAirTemp(devicePin));
    _jobQueue.push_back(readAirTemp);

  } 
  // Moisture
  else if (jobType == JobTypeEnum::ReadMoisture) {
    byteArrayAsFloat calibrationValue;
    byteArrayAsFloat additionalParam;
    
    calibrationValue.byteArray[0] = receivedData[6];
    calibrationValue.byteArray[1] = receivedData[7];
    calibrationValue.byteArray[2] = receivedData[8];
    calibrationValue.byteArray[3] = receivedData[9];
    
    additionalParam.byteArray[0] = receivedData[10];
    additionalParam.byteArray[1] = receivedData[11];
    additionalParam.byteArray[2] = receivedData[12];
    additionalParam.byteArray[3] = receivedData[13]];

    std::shared_ptr<ReadMoisture> readMoisture(new ReadMoisture(devicePin, calibrationValue.floatVal, additionalParam.floatVal));
    _jobQueue.push_back(readMoisture);

  } else {
    Serial.println("Unknown job type");
    return;
  }

  _jobQueue[queueIndex]->SetStatus (JobStatusEnum::Queued);
  _jobQueue[queueIndex]->SetType (jobType);
}

void HydroinoJobMgr::SetJobParams (byte *receivedData, int queueIndex, JobTypeEnum jobType) {
  byteArrayAsLong param1;
  byteArrayAsLong param2;
  
  switch (jobType) {
    case JobTypeEnum::RunDevice:
      // Start / stop flag
      param1.byteArray[0] = receivedData[14];
      param1.byteArray[1] = receivedData[15];
      param1.byteArray[2] = receivedData[16];
      param1.byteArray[3] = receivedData[17];  

      // Duration
      param2.byteArray[0] = receivedData[18];
      param2.byteArray[1] = receivedData[19];
      param2.byteArray[2] = receivedData[20];
      param2.byteArray[3] = receivedData[21];   
    break;

    case JobTypeEnum::RunServo:
      // Position
      param1.byteArray[0] = receivedData[6];
      param1.byteArray[1] = receivedData[7];
      param1.byteArray[2] = receivedData[8];
      param1.byteArray[3] = receivedData[9];

      // Not used
      param2.byteArray[0] = 0;
      param2.byteArray[1] = 0;
      param2.byteArray[2] = 0;
      param2.byteArray[3] = 0;
    break;    
    
    case JobTypeEnum::RunFan:
      // Speed
      param1.byteArray[0] = receivedData[6];
      param1.byteArray[1] = receivedData[7];
      param1.byteArray[2] = receivedData[8];
      param1.byteArray[3] = receivedData[9];

      // Not used
      param2.byteArray[0] = 0;
      param2.byteArray[1] = 0;
      param2.byteArray[2] = 0;
      param2.byteArray[3] = 0;
    break;
  }

  _jobQueue[queueIndex]->SetParam1 (param1.longVal);
  _jobQueue[queueIndex]->SetParam2 (param2.longVal);
}


/*------------------------------------------------
  Public methods
-------------------------------------------------*/
/*
* Getters/Setter
*/
JobQueueType HydroinoJobMgr::GetJobQueue() {
  return _jobQueue;
}

int HydroinoJobMgr::GetJobQueueMaxLength() {
  return QUEUE_LEN;
}

/*
* Process the job queue
*/
void HydroinoJobMgr::ProcessJobQueue() {
  JobStatusEnum newJobStatus;

  // Iterate the process queue
  for (int i = 0; i < _jobQueue.size(); i++) {
    // Check proc status
    switch (_jobQueue[i]->GetStatus()) {
    // Queued jobs
    case JobStatusEnum::Queued:
      newJobStatus = JobStatusEnum::Running;

      // Debug print 
      Serial.print("Queued job at index: ");
      Serial.println(i);

      // Check if there is a precondition job
      if (_jobQueue[i]->GetPrecedingJobIndex() >= 0)
      {
        // The job should not be empty
        if (_jobQueue[_jobQueue[i]->GetPrecedingJobIndex()]->GetStatus() == JobStatusEnum::Empty) {
          newJobStatus = JobStatusEnum::Error;
          Serial.print("An error has occured processing job ");
          Serial.print(i);
          Serial.print(".  The precondition ");
          Serial.print(_jobQueue[i]->GetPrecedingJobIndex());
          Serial.println(" is Empty.");
        }
        // If the preceding job is not finished the new job cannot run
        else if (_jobQueue[_jobQueue[i]->GetPrecedingJobIndex()]->GetStatus() != _jobQueue[i]->GetPrecedingJobStatus())
          newJobStatus = JobStatusEnum::Queued;
      }

      if (newJobStatus == JobStatusEnum::Running) {
        _jobQueue[i]->Start ();
      }
      
      _jobQueue[i]->SetStatus (newJobStatus);
      break;

    // Running jobs
    case JobStatusEnum::Running:
    #ifdef DEBUG
      Serial.print("Job is running at index: ");
      Serial.println(i);
    #endif      

      // Update the current job
      _jobQueue[i]->Update();
      break;

    // Complete jobs
    case JobStatusEnum::Complete:   
      char msg[] = "Complete";
      _jobQueue[i]->Stop(_responseData);
      DequeueJobAtIndex(i);       
      _responseData->msgLength = strlen (msg);
      strcpy(_responseData->msg, msg);
      break;

    // Job errors
    case JobStatusEnum::Error:
        Serial.print("Job ");
        Serial.print(i);
        Serial.println(" has status of Error.");
      break;

    default:
      // Unknown Status 
      // TODO: error state
      break;
    }
  }
}

/*
* Add a new job to the queue
*/
int HydroinoJobMgr::EnqueueJob (byte *receivedData) {
  int queueIndex;
  JobTypeEnum jobType;
  bool jobAlreadyQueued = false;
  byteArrayAsLong devicePin;
  byteArrayAsLong param1;

  // Get the job type
  jobType =  (JobTypeEnum)receivedData[0];
  Serial.print("jobType is ");
  Serial.println((int)jobType);

  // Get device ID (aka pin #)- first 4 bites
  devicePin.byteArray[0] = receivedData[2];
  devicePin.byteArray[1] = receivedData[3];
  devicePin.byteArray[2] = receivedData[4];
  devicePin.byteArray[3] = receivedData[5];

  // Find empty queue index
  queueIndex = GetQueueIndexForNewJob(jobType, devicePin.longVal, &jobAlreadyQueued);

  if (queueIndex <0) {
    Serial.println("Job cannot be queued");
    return;
  }

  if (!jobAlreadyQueued){
    InitializeNewJob (receivedData, queueIndex, jobType, devicePin.longVal);
  }

  // Set paras for a new job or update params for an existing job
  SetJobParams (receivedData, queueIndex, jobType);

  return queueIndex;
}

// Remove job from queue at specific index
void HydroinoJobMgr::DequeueJobAtIndex (int queueIndex){  
  Serial.print("Dequeuing job at index ");
  Serial.println(queueIndex);

  _jobQueue.remove(queueIndex);
}
