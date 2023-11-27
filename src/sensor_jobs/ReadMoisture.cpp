/**
 * @file ReadMoisture.cpp
 * @author oc
 * @brief This file contains the implementation of the ReadMoisture class.
 * 
 * The ReadMoisture class is responsible for reading the moisture level from a sensor,
 * and providing access to the read value.
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
#include "ReadMoisture.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadMoisture::ReadMoisture (int sensorId, float calibrationValue, float additionalParam){ 
  // Set member values
  _devicePin = sensorId;
  _calibrationValue = calibrationValue;
  _moisturePowerPin = additionalParam;
  _sensorValue = 0.0;
   
  pinMode(_moisturePowerPin, OUTPUT);
  digitalWrite(_moisturePowerPin, LOW);//Set to LOW so no power is flowing through the sensor
}

// Destructor
ReadMoisture::~ReadMoisture() { 
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

/*
* Methods
*/
void ReadMoisture::Start (){ 
  #ifdef DEBUG
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);
  #endif
  
  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadMoisture::Update() {    
  float sensorReading;

  if (_retryTime < millis()) { // Delay next read
    digitalWrite(_moisturePowerPin, HIGH);
    delay(100);
    sensorReading = analogRead(_devicePin);//Read the SIG value from sensor
    digitalWrite(_moisturePowerPin, LOW);
    
    #ifdef DEBUG
    Serial.print ("Sensor reading: ");
    Serial.println (sensorReading);
    #endif

    _retryCount++;
    _retryTime = millis() + READ_DELAY_MILLIS; // Wait before next read

    if (!isnan(sensorReading)){
      _sensorValue = sensorReading;
      _status = JobStatusEnum::Complete;
    }
    else if (isnan(sensorReading) && _retryCount == MAX_RETRIES) {
      #ifdef DEBUG
        Serial.println("Reading sensor - value is not a number");
      #endif
    
      _sensorValue = 0.0;
      _status = JobStatusEnum::Complete;
    }
  }
}

void ReadMoisture::Stop (ResponseData *responseData) { 
    responseData->floatValue1 = _sensorValue;
}