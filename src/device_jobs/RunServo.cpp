/**
 * @file RunServo.cpp
 * @author oc
 * @brief This file contains the implementation of the RunServo class.
 * 
 * The RunServo class is responsible for managing a servo, including starting,
 * updating, and stopping the servo. It also handles setting parameters for the servo.
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
#include "RunServo.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunServo::RunServo (int deviceID){
  // Set default values
   _devicePin = deviceID;   
   _servo.attach(_devicePin);
}

// Blank Destructor
RunServo::~RunServo() {
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
void RunServo::SetParam1(long param) {
  _position = param;
}

/*
* Methods
*/
void RunServo::Start () {    
  if (_position < MIN_ANGLE)
    _position = MIN_ANGLE;
  else if (_position > MAX_ANGLE)
    _position = MAX_ANGLE;

  _servo.write(_position);
}

void RunServo::Update() { 
  _status = JobStatusEnum::Complete;
}

void RunServo::Stop (ResponseData *responseData) {  
  
}
