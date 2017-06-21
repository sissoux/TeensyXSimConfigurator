
#include "Arduino.h"
#include <float.h>

#ifndef Motor_h
#define Motor_h

#define DIRECT  0
#define REVERSE  1

#define SCALED_MODE 0
#define CLAMPED_MODE 1
#define MAX_UINT16 65535
#define HALF_16BIT 32767
#define RAW_OUT_MIN -32768.0
#define RAW_OUT_MAX 32767.0
#define OUT_MIN -128
#define OUT_MAX 127


class Motor
{
  public:
	double FeedBack;     //PID Feedback Signal to compute error
	int8_t Output;

    Motor(byte id, double kp, double ki, double kd, Stream *serialPtr);
    
    void begin();
	void computePID(double Error);
	void writeSpeed(int8_t speed);
	void setFeedback(uint16_t value);
	void updateState();
	void setPID(double kp, double ki, double kd);
	void initialize();
	void startPID();
	void stopPID();
	void setSampleTime(double SampleTime);
	uint16_t setTarget(uint16_t setPoint, uint8_t mode); 
	void setLimits(uint16_t High, uint16_t Low, uint16_t offset);


  private:
    Stream *serial;
    uint8_t _Id;
    uint16_t _Speed;
    uint16_t _AnalogInput;
    uint16_t _LLimit = 0;
    uint16_t _HLimit = 65535;
	int16_t Offset = 0;
	bool PIDOn = false;
	double SampleTimeInS;

    double dispKp;       // * we'll hold on to the tuning parameters in user-entered
    double dispKi;        //   format for display purposes
    double dispKd;        //

    double Kp;                  // * (P)roportional Tuning Parameter
    double Ki;                  // * (I)ntegral Tuning Parameter
    double Kd;                  // * (D)erivative Tuning Parameter

	//double FeedBack;     //PID Feedback Signal to compute error
	double ClampedSetPoint;

    elapsedMillis PIDTimer;
    uint16_t PIDCalculationRate;
    double ITerm, LastFeedback;

    bool inAuto;
    
};
#endif

