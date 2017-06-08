#include "Simulator.h"
#include "ArduinoJson\ArduinoJson.h"



Simulator::Simulator() : M1(0, 10, 10, 10, A0, &Serial), M2(0, 10, 10, 10, A0, &Serial)
  {
    
  }

  void Simulator::SendData()
  {
    M1.Hello();
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
		  if (strcmp(Command, "getClampedSetPoint") == 0) //{Cmd:getClampedSetPoint}
		  {
			  //sendClampedSetPoint(4, 500);
		  }
		  else if (strcmp(Command, "getBoardInfo") == 0) //{Cmd:getBoardInfo}
		  {
			  //sendBoardInfo();
		  }
	  }
  }