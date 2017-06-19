#include "Simulator.h"
#include <ArduinoJson.h>



Simulator::Simulator(Stream *MainStream, Stream *MotorStream, int AnalogIn1, int AnalogIn2, uint16_t PID_Rate) : M1(0, 10, 10, 10, AnalogIn1, MotorStream), M2(1, 10, 10, 10, AnalogIn2, MotorStream)
  {
	  this->serial = MainStream;
	  PIDRefreshRate = PID_Rate;
  }
  

  void Simulator::parseSerialData()
  {
	  if (serial->available())
	  {
		  DynamicJsonBuffer jsonBuffer;
		  char lastChar = '\0';
		  int i = 0;
		  SerialInTimer = 0;
		  while (lastChar != '\r')
		  {
			  if (SerialInTimer >= SERIAL_IN_TIMEOUT)
			  {
				  Serial.println("Timeout");
				  return;
			  }
			  if (Serial.available())
			  {
				  lastChar = Serial.read();
				  input[i] = lastChar;
				  i++;
			  }
		  }
		  JsonObject& root = jsonBuffer.parseObject(input);
		  const char* Command = root["Cmd"];
		  if (strcmp(Command, "getCapture") == 0) //{Cmd:getClampedSetPoint}
		  {
			  //sendClampedSetPoint(4, 500);
		  }
		  else if (strcmp(Command, "getBoardInfo") == 0) //{Cmd:getBoardInfo}
		  {
			  sendBoardInfo();
		  }
		  else if (strcmp(Command, "save") == 0) //{Cmd:save}
		  {
			  //sendBoardInfo();
		  }
	  }
  }

  void Simulator::sendBoardInfo()
  {
		  DynamicJsonBuffer jsonBuffer;
		  JsonObject& root = jsonBuffer.createObject();

		  root["Cmd"] = "BoardInfo";
		  root["BN"] = BOARD_NAME;
		  root["FirmRev"] = FIRMWARE_REVISION;
		  root["BoardRev"] = BOARD_REVISION;

		  root.printTo(Serial);
		  Serial.println();
}

  void Simulator::refreshState()
  {
	  parseSerialData();

	  if (InputRefreshTimer >= ADC_IN_REFRESH_RATE)
	  {
		  InputRefreshTimer -= ADC_IN_REFRESH_RATE;
		  
	  }
  }