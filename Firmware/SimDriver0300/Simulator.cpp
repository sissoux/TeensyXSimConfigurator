#include "Simulator.h"
#include <ArduinoJson.h>
#include <ADC.h>



Simulator::Simulator(Stream *MainSerial, Stream *MotorSerial, int ADCIn1, int ADCIn2, uint16_t PID_Rate) : AnalogIn1(ADCIn1), AnalogIn2(ADCIn2), M1(0, 10, 10, 10, MotorSerial), M2(1, 10, 10, 10, MotorSerial)
  {
	  this->serial = MainSerial;
	  setPIDRefreshRate(PID_Rate);
	  initADC();
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
		  if (!root.success())
		  {
			  Serial.println("parseObject() failed");
			  return;
		  }

		  const char* Command = root["Cmd"];
		  if (strcmp(Command, "getCapture") == 0) //{Cmd:getClampedSetPoint}
		  {

		  }
		  else if (strcmp(Command, "getBoardInfo") == 0) //{Cmd:getBoardInfo}
		  {
			  sendBoardInfo();
		  }
		  else if (strcmp(Command, "save") == 0) //{Cmd:save}
		  {

		  }
		  else if (strcmp(Command, "setMotorParameters"))
		  {
			  M1.setLimits(root["MotorInfo"][0]["HL"], root["MotorInfo"][0]["LL"], root["MotorInfo"][0]["Offset"]);
			  M2.setLimits(root["MotorInfo"][1]["HL"], root["MotorInfo"][1]["LL"], root["MotorInfo"][1]["Offset"]);
		  }
		  else if (strcmp(Command, "getMotorParameters"))
		  {
			  
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

  void Simulator::sendMotorInfo()
  {
	  DynamicJsonBuffer jsonBuffer;
	  JsonObject& root = jsonBuffer.createObject();

	  root["Cmd"] = "MotorInfo";

	  JsonArray& MotorInfo = root.createNestedArray("MotorInfo");
	  for (int i = 0; i < 2; i++)
	  {
		  JsonObject& temp = MotorInfo.createNestedObject();
		  temp["ID"] = i;
		  temp["Position"] = i;
		  temp["kp"] = i;
		  temp["ki"] = i;
		  temp["kd"] = i;
		  temp["HL"] = i;
		  temp["LL"] = i;
		  temp["Offset"] = i;
	  }

	  root.printTo(Serial);
	  Serial.println();
  }

  void Simulator::refreshState()
  {
	  parseSerialData();

	  if (InputRefreshTimer >= ADC_IN_REFRESH_RATE)
	  {
		  InputRefreshTimer -= ADC_IN_REFRESH_RATE;
		  adcUpdate();
	  }

	  if (PIDRefreshTimer >= PIDRefreshRate)
	  {
		  PIDRefreshTimer -= PIDRefreshRate;
		  M1.updateState();
		  M2.updateState();
	  }
  }

  void Simulator::adcUpdate()
  {
	  converter->startSynchronizedSingleRead(AnalogIn1, AnalogIn2);
	  result = converter->readSynchronizedSingle();
	  M1.setFeedback(result.result_adc0);
	  M2.setFeedback(result.result_adc1);
  }

  void Simulator::initADC()
  {
	  converter->setResolution(16);
	  converter->setAveraging(16);
	  converter->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
	  converter->setResolution(16, ADC_1);
	  converter->setAveraging(16, ADC_1);
	  converter->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED, ADC_1);
  }

  void Simulator::setPIDRefreshRate(uint16_t rateInMS)
  {
	  this->PIDRefreshRate = rateInMS;
	  this->PIDRefreshRateInS = (double)rateInMS / 1000.0;
	  
  }