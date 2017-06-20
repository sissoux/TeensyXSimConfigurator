
#include "Arduino.h"
#include "Motor.h"
#include <ADC.h>

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
  const uint8_t AnalogIn1;
  const uint8_t AnalogIn2;
  

  Simulator(Stream *MainStream, Stream *MotorStream, int AnalogIn1, int AnalogIn2, uint16_t PID_Rate);
  void parseSerialData();
  void sendBoardInfo();
  void sendMotorInfo();
  void refreshState();
  void begin();
  void setPIDRefreshRate(uint16_t rateInMS);
  
private:
	ADC *converter = new ADC();
	ADC::Sync_result result;
	void adcUpdate();
	void initADC();

	Stream *serial;
	elapsedMillis SerialInTimer = 0;
	char input[500];

	uint16_t PIDRefreshRate;
	double PIDRefreshRateInS;
	elapsedMillis PIDRefreshTimer = 0;

	elapsedMillis InputRefreshTimer = 0;
#define ADC_IN_REFRESH_RATE 1

};

#endif
