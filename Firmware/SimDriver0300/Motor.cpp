#include "Motor.h"
#include <ADC.h>
//#include <math.h>

Motor::Motor(byte id, double kp, double ki, double kd, Stream *serialPtr)
{
  serial = serialPtr;
  dispKp = kp; dispKi = ki; dispKd = kd;
  _Id = id;
}

void Motor::updateState()
{
	if (PIDOn)
	{
		double target = (double)ClampedSetPoint;
		double error = target - FeedBack;

		ITerm += (Ki*error);
		if (ITerm > RAW_OUT_MAX) ITerm = RAW_OUT_MAX;
		else if (ITerm < RAW_OUT_MIN) ITerm = RAW_OUT_MIN;

		double dInput = (FeedBack - LastFeedback);
		double output = Kp * error + ITerm - Kd * dInput;

		output = output / 256.0;

		if (output > OUT_MAX) output = OUT_MAX;
		else if (output < OUT_MIN) output = OUT_MIN;

		Output = (int8_t)output;
		LastFeedback = FeedBack;
	}
}

void Motor::writeOutput()
{
	this->writeSpeed(this->Output);
}


//////////////////////////////////////////////
// Send motor speed by serial to sabertooth //
// Speed : between -127 and +127            //
//////////////////////////////////////////////

void Motor::writeSpeed(int8_t Speed)
{
	uint8_t AbsSpeed = (Speed < 0 ? -Speed : Speed);
	AbsSpeed = (AbsSpeed > 127 ? 127 : AbsSpeed);
  uint8_t Command = (_Id == 2 ? 4 : 0) + (Speed < 0 ? 1 : 0);
  

  serial->write((uint8_t)128);       //Address
  serial->write(Command);  //command
  serial->write(AbsSpeed);     //Rate
  serial->write((128 + Command + AbsSpeed) & 0b01111111); //Required checksum with mask
}

void Motor::setSampleTime(double SampleTime)
{
	SampleTimeInS = SampleTime/1000.0;
	Ki = dispKi * SampleTimeInS;
	if (SampleTimeInS != 0)	Kd = dispKd / SampleTimeInS;
}

void Motor::setFeedback(uint16_t value)
{
	FeedBack = (double)value;
	// Scaling to be done
}

uint16_t Motor::setTarget(uint16_t setPoint, uint8_t mode)
{
	if (mode == CLAMPED_MODE)
	{
		int32_t tempSetPoint = (int32_t)setPoint + (int32_t)Offset;
		tempSetPoint = (tempSetPoint > (int32_t)_HLimit ? _HLimit : tempSetPoint);
		ClampedSetPoint = (tempSetPoint < (int32_t)_LLimit ? _LLimit : (uint16_t)tempSetPoint);
	}
	else if (mode == SCALED_MODE)
	{

	}
	return ClampedSetPoint;
}

void Motor::setLimits(uint16_t High, uint16_t Low, uint16_t Neutral)
{
	_HLimit = High;
	dispHL = High;
	_LLimit = Low;
	dispLL = Low;
	Offset = (int16_t)((int32_t)Neutral - HALF_16BIT);
}
void Motor::setHighLimit()		//Set current position as High Limit
{
	uint16_t Value = (uint16_t)this->FeedBack;
	if (Value <= HALF_16BIT) return;	//Probably not what we want
	_HLimit = Value;
	dispHL = Value;
}
void Motor::setLowLimit()
{
	uint16_t Value = (uint16_t)this->FeedBack;
	if (Value >= HALF_16BIT) return;	//Probably not what we want
	_LLimit = Value;
	dispLL = Value;
}
void Motor::setOffset()
{
	uint16_t Value = (uint16_t)this->FeedBack;
	Offset = (int16_t)((int32_t)Value - HALF_16BIT);	//Offset is a value to add or remove to the center point (half 16 bits)
}

void Motor::setPID(double kp, double ki, double kd)
{
	if (kp < 0 || ki < 0 || kd < 0) return;

	dispKp = kp; dispKi = ki; dispKd = kd;

	Kp = kp;
	Ki = ki * SampleTimeInS;
	if (SampleTimeInS != 0)	Kd = kd / SampleTimeInS;
}


/* Initialize()****************************************************************
*	does all the things that need to happen to ensure a bumpless transfer
*  from manual to automatic mode.
******************************************************************************/
void Motor::initialize()
{
	ITerm = 0;
	LastFeedback = FeedBack;
	setPID(dispKp, dispKi, dispKd);
}

void Motor::startPID()
{
	if (!PIDOn)
	{
		initialize();
		PIDOn = true;
	}
}

void Motor::stopPID()
{
		PIDOn = false;
}

void Motor::setToMiddlePoint()
{
	this->setTarget(HALF_16BIT, CLAMPED_MODE);
}
