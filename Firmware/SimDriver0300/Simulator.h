
#include "Arduino.h"
#include "Motor.h"

#ifndef Simulator_h
#define Simulator_h

#define SERIAL_IN_TIMEOUT 2000


class Simulator
{
  public:
  Motor M1;
  Motor M2;
  
  Stream *serial;
  elapsedMillis SerialInTimer = 0;
  char input[500];

  Simulator();
  void SendData();
  void parseSerialData();
  

  
};

#endif
