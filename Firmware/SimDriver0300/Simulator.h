
#include "Arduino.h"
#include "Motor.h"

#ifndef Simulator_h
#define Simulator_h


class Simulator
{
  public:
  Motor M1;
  
  Simulator();
  void SendData();
  

  
};

#endif
