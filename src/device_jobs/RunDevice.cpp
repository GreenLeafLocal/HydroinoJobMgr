/**
 * @file RunDevice.cpp
 * @author oc
 * @brief This file contains the implementation of the RunDevice class.
 * 
 * The RunDevice class is responsible for managing a device, including starting,
 * updating, and stopping the device. It also handles setting parameters for the device.
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
#include "RunDevice.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunDevice::RunDevice (int deviceID,  int startValue){
  // Set default values
   _stopJob = false;
   _devicePin = deviceID;
   _stopTime = 0;

  // Determine start/stop values
  if (startValue == 1) {
    _startValue = HIGH;
    _stopValue = LOW;
  }
  else if (startValue == 0) {   
    _startValue = LOW;
    _stopValue = HIGH;
  }
}

// Blank Destructor
RunDevice::~RunDevice() {
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
// Param 1 used to indicate if job is done
void RunDevice::SetParam1(long param) {
  if (param == 1)
    _stopJob = false;
  else if (param == 0)
    _stopJob = true;
}

// Param 2 used to indicate job duration
// It is expected this value will be provide in seconds 
// and thus needs to be converted to milliseconds
void RunDevice::SetParam2(long param) {   
  #ifdef DEBUG  
    Serial.print("Param 2 is ");
    Serial.println(param);
  #endif   

  // Check duration is within limits
  if (MIN_DURATION > param || param > MAX_DURATION)
    return;
  
  // Adjust param to milliseconds
  param = param * 1000;

  // Set run duration / job stop time
  if (param > 0) {    
    _stopTime = millis() + param;    
  }
}

/*
* Methods
*/
void RunDevice::Start () { 
  #ifdef DEBUG  
    Serial.print("Running start job for device ");
    Serial.println(_devicePin);
  #endif   
   
  
  // Check if job duration is complete, unless job already flagged as finished
  if (!_stopJob && _stopTime > 0)
    _stopJob = _stopTime < millis() ? true : false;

  if (!_stopJob) {    
    pinMode(_devicePin, OUTPUT);  
    digitalWrite(_devicePin, _startValue);
  }
}

void RunDevice::Update() { 
  // Check if job duration is complete, unless job already flagged as finished
  if (!_stopJob && _stopTime > 0)
    _stopJob = _stopTime < millis() ? true : false;

  if (_stopJob) {  
    _status = JobStatusEnum::Complete;
  }
}

void RunDevice::Stop (ResponseData *responseData) { 
  pinMode(_devicePin, OUTPUT); 
  digitalWrite(_devicePin, _stopValue);  
}