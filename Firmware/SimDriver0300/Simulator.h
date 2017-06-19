
#include "Arduino.h"
#include "Motor.h"

#ifndef Simulator_h
#define Simulator_h


#define BOARD_NAME "X-Sim teensyBoard"
#define FIRMWARE_REVISION "3.00"
#define BOARD_REVISION "2.00"

#define SERIAL_IN_TIMEOUT 2000


class Simulator
{
  public:
  Motor M1;
  Motor M2;
  
  Stream *serial;
  elapsedMillis SerialInTimer = 0;
  char input[500];

  uint16_t PIDRefreshRate;
  elapsedMillis PIDRefreshTimer = 0;

  elapsedMillis InputRefreshTimer = 0;
#define ADC_IN_REFRESH_RATE 1

  Simulator(Stream *MainStream, Stream *MotorStream, int AnalogIn1, int AnalogIn2, uint16_t PID_Rate);
  void parseSerialData();
  void sendBoardInfo();
  void refreshState();
  
private:

};

#endif
