/**
 * @file ReadAirTemp.cpp
 * @author oc
 * @brief This file contains the implementation of the ReadAirTemp class.
 * 
 * The ReadAirTemp class is responsible for reading the air temperature from a sensor,
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
#include "ReadAirTemp.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadAirTemp::ReadAirTemp (int sensorId){ 
  // Set member values
  _devicePin = sensorId;
  _sensorValue = 0.0;  
  _dhtSensor = new DHT (_devicePin, DHTTYPE);
}

// Destructor
ReadAirTemp::~ReadAirTemp() {          
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
void ReadAirTemp::Start (){ 
  #ifdef DEBUG
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);
  #endif

  _dhtSensor->begin();  // Initialize the sensor
  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadAirTemp::Update() {    
  float sensorReading;

  if (_retryTime < millis()) { // Delay next read
    float temp = _dhtSensor->readTemperature();
    float humidity = _dhtSensor->readHumidity();        
    sensorReading = _dhtSensor->computeHeatIndex(temp, humidity, false);

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
    
      _sensorValue = 0.0;
      _status = JobStatusEnum::Complete;
    }
  }
}

void ReadAirTemp::Stop (ResponseData *responseData) { 
    responseData->floatValue1 = _sensorValue;
}