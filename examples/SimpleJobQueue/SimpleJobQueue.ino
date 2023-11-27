/**
 * @file RunDevice.cpp
 * @author oc
 * @brief This is an example of how to use the HydroinoJobMgr library to manage jobs in an Arduino project.
 * The example demonstrates setting up I2C communication, handling received data, and processing a job queue.
 * 
 * Dependencies: This example depends on the Wire, OneWire, and HydroinoJobMgr libraries.
 * 
 * @license Apache License 2.0
 */

#include <Wire.h>
#include <OneWire.h>
#include <HydroinoJobMgr.h>

// Define constants for communication and buffer sizes
#define RESPONSE_LEN    32
#define CMD_BUFFER_LEN  30
#define I2C_ADDRESS     0x41

// Union to allow easy conversion between byte array and structured data
union byte_response {
  byte  byteArray[RESPONSE_LEN];
  ResponseData response;
} _responseUnion;

// Define variables for response data and buffers
ResponseData _response[1];
byte _defaultMsg[JOB_MSG_LEN] = {};  
byte _receivedData[CMD_BUFFER_LEN] ={};  
byte _defaultData[CMD_BUFFER_LEN] ={};   

// Create an instance of the HydroinoJobMgr class
HydroinoJobMgr _hydroinoJobMgr (_response);

// Setup function for Arduino
void setup() {
  // Begin I2C communication and set up receive and request handlers
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(I2CReceived);
  Wire.onRequest(I2CResponse);
  Serial.begin(9600);
  
  // Initialize response data
  _response->floatValue1 = 0.0;
  _response->floatValue2 = 0.0;
  _response->longValue1 = 0;
  _response->longValue2 = 0;
  _response->msgLength = 0;
  memcpy(_response->msg, _defaultMsg, JOB_MSG_LEN);
}

// Main loop for Arduino
void loop() {    
  // Check if new data has been received
  if (memcmp(_receivedData, _defaultData, CMD_BUFFER_LEN) != 0) {   
    // If new data is received, enqueue a new job
    _hydroinoJobMgr.EnqueueJob(_receivedData);
    // Reset the received data to default
    memcpy(_receivedData, _defaultData, CMD_BUFFER_LEN);
  }
   
  // Process the job queue
  _hydroinoJobMgr.ProcessJobQueue();
}

// Function to handle data received from I2C
void I2CReceived(int numberOfBytes) {
  // Read the received bytes into the received data buffer
  Wire.readBytes(_receivedData, numberOfBytes);
}

// Function to handle data request from I2C
void I2CResponse() {
  // Copy the response data to the response union
  _responseUnion.response.floatValue1 = _response->floatValue1;
  _responseUnion.response.floatValue2 = _response->floatValue2;
  _responseUnion.response.longValue1 = _response->longValue1;  
  _responseUnion.response.longValue2 = _response->longValue2; 
  _responseUnion.response.msgLength = _response->msgLength;
  strcpy(_responseUnion.response.msg, _response->msg);

  // Write the response data as a byte array
  Wire.write((char*)(_responseUnion.byteArray), RESPONSE_LEN);

  // Clear the response data
  memset(_response, 0, RESPONSE_LEN);
}