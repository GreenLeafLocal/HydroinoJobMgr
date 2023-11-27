# HydroinoJobMgr

This library provides functions to control a GreenLeafLocal OpenFarms grow enclosure. The GreenLeafLocal OpenFarms grow enclosure is a system for managing and automating indoor farming. This library allows you to enqueue and process jobs for the enclosure.

## Installation

1. Download this library from [GitHub](https://github.com/GreenLeafLocal/HydroinoJobMgr) or clone the repository using the following command: `git clone actual repository URL`.
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library.

## Dependencies

This library depends on the following libraries:

- `Array` - [Array library](https://www.arduino.cc/reference/en/libraries/array/)
- `ArxSmartPtr` - [ArxSmartPtr library](https://www.arduino.cc/reference/en/libraries/arxsmartptr/)
- `PWMServo` - [PWMServo library](https://www.arduino.cc/reference/en/libraries/pwmservo/)
- `Adafruit_Sensor` - [Adafruit_Sensor library](https://www.arduino.cc/reference/en/libraries/adafruit-unified-sensor/)
- `DHT ` - [DHT  library](https://www.arduino.cc/reference/en/libraries/dht-sensor-library/)

## Basic Usage

Include the library, create a `HydroinoJobMgr` object, and call its methods. Here's a basic example (see examples/SimpeJobQueue.ino for full example):

```cpp
// include the library
#include "HydroinoJobMgr.h"

// buffer size
#define CMD_BUFFER_LEN  30

// Union to allow easy conversion between byte array and structured data
union byte_response {
  byte  byteArray[RESPONSE_LEN];
  ResponseData response;
} _responseUnion;

// byte array for job details sent to the Arduino 
byte _receivedData[CMD_BUFFER_LEN] ={};  

// create a HydroinoJobMgr object
ResponseData _response[1];  

// Create an instance of the HydroinoJobMgr class
HydroinoJobMgr _hydroinoJobMgr (_response);

// Receive new job request (via I2C for example)
...

// Enqueue a new job based on new job
_hydroinoJobMgr.EnqueueJob(_receivedData);
   
// Process the job queue.  You'd likely call this on every iteration of the loop() function
_hydroinoJobMgr.ProcessJobQueue();