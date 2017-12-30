
#include "Arduino.h"
#include "Motor.h"
#include "EEPROM.h"

#include <ADC.h>

#ifndef Simulator_h
#define Simulator_h


#define BOARD_NAME "X-Sim teensyBoard"
#define FIRMWARE_REVISION "3.00"
#define BOARD_REVISION "2.00"

#define SERIAL_IN_TIMEOUT 2000
#define STOPPED 0
#define INIT 1
#define RUNNING 2
#define READY 3
#define CAPTURE_SAMPLING_RATE 5	//Capture State each 10ms
#define CAPTURE_STEPS 500
#define CAPTURE_INIT_DELAY 1000	//ms to wait before capture (Send motor to offsetPoint, then we start the capture)
#define CAPTURE_STEPS_DELAY 50
#define FRAME_CAPACITY 100

#define EEPROM_START_ADDRESS 100
#define INITIALIZED_VALUE 120

class Simulator
{
  public:
  Motor M1;
  Motor M2;
  const uint8_t AnalogIn1;
  const uint8_t AnalogIn2;
  const uint8_t CurrentAnalogIn;
  

  Simulator(Stream *MainStream, Stream *MotorStream, int AnalogIn1, int AnalogIn2, int CurrentAnalogIn, uint16_t PID_Rate);
  void parseSerialData();
  void sendBoardInfo();
  void sendMotorInfo();
  void refreshState();
  void begin();
  void setPIDRefreshRate(uint16_t rateInMS);
  
private:
	uint8_t CaptureSate = STOPPED;
	elapsedMillis CaptureTimer = 0;
	int16_t CaptureStep = 0;
	uint32_t TimeBuffer[500];
	int8_t OutputBuffer[500];
	uint16_t SetPointBuffer[500];
	uint16_t PositionBuffer[500];
	Motor *ActiveCaptureMotor;
	uint16_t CaptureTarget = 0;
	void newCaptureTrigger(uint16_t Target, uint8_t motorID);
	void sendCapture(uint16_t NumberOfPoints);

	ADC *converter = new ADC();
	ADC::Sync_result result;
	void adcUpdate();
	void initADC();
	void saveToEEPROM();
	void recallEEPROM();

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
