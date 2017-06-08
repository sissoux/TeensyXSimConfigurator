#include "Arduino.h"

#ifndef Motor_h
#define Motor_h

#define AUTOMATIC  1
#define MANUAL  0
#define DIRECT  0
#define REVERSE  1


class Motor
{
  public:

    Motor(byte id, double kp, double ki, double kd, uint8_t AnalogInput, Stream *serialPtr);
    void Hello();
    
  private:
    Stream *serial;
    uint8_t _Id;
    uint16_t _Speed;
    uint16_t _AnalogInput;
    uint16_t _LLimit;
    uint16_t _HLimit;

};
#endif

