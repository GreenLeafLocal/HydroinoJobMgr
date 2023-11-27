/**
 * @file RunFan.cpp
 * @author oc
 * @brief This file contains the implementation of the RunFan class.
 * 
 * The RunFan class is responsible for managing a fan, including starting,
 * updating, and stopping the fan. It also handles setting parameters for the fan.
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
#include "RunFan.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunFan::RunFan (int fanId) {  
  // Set default values
  _devicePin = 0;
  _fanMgr = new FanMgr;
    
  _devicePin = fanId;
  _fanMgr->InitializeFan(fanId); 

  #ifdef DEBUG
    Serial.print("fan pin set to: ");
    Serial.println(_devicePin);
  #endif

  if (_devicePin != 0){ 
    pinMode(_devicePin, OUTPUT); 
  }
}

// Blank Destructor
RunFan::~RunFan() {
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
void RunFan::SetParam1(long param) {
  _speed = param;
}

/*
* Methods
*/
void RunFan::Start () { 
  _fanMgr->ControlFan (_devicePin, _speed);
}

void RunFan::Update() {  
  _status = JobStatusEnum::Complete; 
}

void RunFan::Stop (ResponseData *responseData) { 

}