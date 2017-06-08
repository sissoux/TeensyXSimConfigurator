#include "Motor.h"


Motor::Motor(byte id, double kp, double ki, double kd, uint8_t AnalogInput, Stream *serialPtr)
{
  serial = serialPtr;
}

//////////////////////////////////////////////
// Send motor speed by serial to sabertooth //
// Speed : between -127 and +127            //
//////////////////////////////////////////////
/*/
void Motor::writeSpeed(int Speed)
{
  _Speed = Speed;
  byte Command = (_Id == 2 ? 4 : 0) + (Speed < 0 ? 1 : 0);
  byte BSpeed = (byte)abs(Speed);

  Serial1.write((byte)128);       //Address
  Serial1.write(Command);  //command
  Serial1.write(BSpeed);     //Rate
  Serial1.write((128 + Command + BSpeed) & 0b01111111); //Required checksum with mask
}
*/
void Motor::Hello()
{
  serial->println("Hello World!");
}

