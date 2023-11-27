/*
  RunDevice.h
*/

#ifndef RunDevice_h
#define RunDevice_h

#include <Arduino.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
// Duration limits
#define MAX_DURATION 3600
#define MIN_DURATION 0

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/


/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class RunDevice: public Job {
  public:     
    // Constructors    
    RunDevice (int deviceID, int startValue);
    ~RunDevice();

    void SetParam1(long param) override;
    void SetParam2(long param) override;

    void Start ();
    void Update();
    void Stop (ResponseData *responseData);
  
  private:    
    int _startValue;
    int _stopValue;
};
#endif