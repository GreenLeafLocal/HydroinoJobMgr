/**
 * @file ReadAirHumidity.cpp
 * @author oc
 * @brief This file contains the implementation of the ReadAirHumidity class.
 * 
 * The ReadAirHumidity class is responsible for reading the air humidity from a sensor,
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
#include "ReadAirHumidity.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadAirHumidity::ReadAirHumidity (int sensorId){ 
  // Set member values
  _devicePin = sensorId;
  _sensorValue = 0.0; 
  _dhtSensor = new DHT (_devicePin, DHTTYPE);
}

// Destructor
ReadAirHumidity::~ReadAirHumidity() {        
  delete _dhtSensor;
  _dhtSensor = nullptr;
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
void ReadAirHumidity::Start (){ 
  #ifdef DEBUG
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);
  #endif
  
  _dhtSensor->begin();  // Initialize the sensor      
  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadAirHumidity::Update() {    
  float sensorReading;

  if (_retryTime < millis()) { // Delay next read
    sensorReading = _dhtSensor->readHumidity();  
    
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
        Serial.print("Reading sensor - value is not a number\n");
        _sensorValue = 0.0;
      #endif
      _status = JobStatusEnum::Complete;
    }
  }
}

void ReadAirHumidity::Stop (ResponseData *responseData) { 
    responseData->floatValue1 = _sensorValue;
}