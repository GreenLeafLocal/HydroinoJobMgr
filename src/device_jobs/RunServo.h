/*
  RunServo.h
*/

#ifndef RunServo_h
#define RunServo_h

#include <Arduino.h>
#include <PWMServo.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define MAX_ANGLE 165
#define MIN_ANGLE 15

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/


/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class RunServo: public Job {
  public:     
    // Constructors    
    RunServo (int deviceID);
    ~RunServo();

    void SetParam1(long param) override;

    void Start ();
    void Update();
    void Stop (ResponseData *responseData);
  
  private:    
    int _position;
    PWMServo _servo;
};
#endif
