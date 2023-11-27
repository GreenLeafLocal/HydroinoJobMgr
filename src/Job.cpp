/**
 * @file Job.cpp
 * @author oc
 * @brief This file contains the implementation of the Job class.
 * 
 * The Job class is responsible for managing a job, including setting and getting job parameters,
 * job type, job status, and other job-related information.
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

#include "Arduino.h"
#include "Job.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
Job::Job () {
  // Init member vars
  _devicePin = -1;
  _stopJob = false;
  _type = JobTypeEnum::None;
  _status = JobStatusEnum::Empty;
  _param1 = -1;
  _param2 = -1;
  _precedingJobIndex = -1;
  _precedingJobStatus = JobStatusEnum::Empty;
}

// Blank Destructor
Job::~Job() {
}

/*------------------------------------------------
  Private methods
-------------------------------------------------*/


/*------------------------------------------------
  Public methods
-------------------------------------------------*/
/*
* Getters/Setter
*/
JobStatusEnum Job::GetStatus() {return _status;}
void Job::SetStatus(JobStatusEnum status) {
  _status = status;
  }

JobTypeEnum Job::GetType() {return _type;}
void Job::SetType(JobTypeEnum type) {_type = type;}

long Job::GetParam1() {return _param1;}
void Job::SetParam1(long param) {_param1 = param;}

long Job::GetParam2() {return _param2;}
void Job::SetParam2(long param) {_param2 = param;}

unsigned long Job::GetStopTime() {return _stopTime;}
void Job::SetStopTime(unsigned long stopTime) {_stopTime = stopTime;}

int Job::GetDevicePin() {return _devicePin;}
void Job::SetDevicePin(int devicePin) {_devicePin = devicePin;}

int Job::GetPrecedingJobIndex() {return _precedingJobIndex;}
void Job::SetPrecedingJobIndex(int precedingIndex){ _precedingJobIndex = precedingIndex;}

JobStatusEnum Job::GetPrecedingJobStatus() {return _precedingJobStatus;}
void Job::SetPrecedingJobStatus(JobStatusEnum precedingStatus){ _precedingJobStatus = precedingStatus;}