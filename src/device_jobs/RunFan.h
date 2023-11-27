/*
  RunFan.h
*/

#ifndef RunFan_h
#define RunFan_h

#include <Arduino.h>
#include <FanMgr.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class RunFan: public Job {
  public:     
    // Constructors    
    RunFan (int fanId);
    ~RunFan();
    
    void SetParam1(long param) override;
        
    void Start ();
    void Update();
    void Stop (ResponseData *responseData); 
  
  private:    
    FanMgr* _fanMgr;
    int     _speed;
};
#endif
