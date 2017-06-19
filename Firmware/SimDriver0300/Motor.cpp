#include "Motor.h"
#include <ADC.h>
//#include <math.h>


Motor::Motor(byte id, double kp, double ki, double kd, uint8_t AnalogInput, Stream *serialPtr)
{
  serial = serialPtr;
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



void Motor::getInput()
{

}