
#include "Arduino.h"
#include <float.h>

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
    
    void begin();
	void computePID(uint16_t Error);
	void writeSpeed(int8_t speed);
	void getInput();
    
  private:
    Stream *serial;
    uint8_t _Id;
    uint16_t _Speed;
    uint16_t _AnalogInput;
    uint16_t _LLimit;
    uint16_t _HLimit;

    double dispKp;       // * we'll hold on to the tuning parameters in user-entered
    double dispKi;        //   format for display purposes
    double dispKd;        //

    double Kp;                  // * (P)roportional Tuning Parameter
    double Ki;                  // * (I)ntegral Tuning Parameter
    double Kd;                  // * (D)erivative Tuning Parameter

	double FeedBack;     //PID Feedback Signal to compute error
	double ClampedSetPoint;

    elapsedMillis PIDTimer;
    uint16_t PIDCalculationRate;
    double ITerm, LastInput;

    bool inAuto;
    
};
#endif

