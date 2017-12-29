#include "Simulator.h"
#include <ArduinoJson.h>
#include <ADC.h>



Simulator::Simulator(Stream *MainSerial, Stream *MotorSerial, int ADCIn1, int ADCIn2, int ADCIn3, uint16_t PID_Rate) : AnalogIn1(ADCIn1), AnalogIn2(ADCIn2), CurrentAnalogIn(ADCIn3), M1(0, 0.05, 1, 0.001, MotorSerial), M2(1, 0.05, 1, 0.001, MotorSerial)
  {
	  this->serial = MainSerial;
	  setPIDRefreshRate(PID_Rate);
  }
  
void Simulator::begin()
{
	initADC();
	M1.setTarget(32768, CLAMPED_MODE);
	M2.setTarget(32768, CLAMPED_MODE);
	M1.setSampleTime(PIDRefreshRate);
	M2.setSampleTime(PIDRefreshRate);
	M1.startPID();
	M2.startPID();
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

			  //Trigger Capture
			  //Once finished Send data buffer

		  }
		  else if (strcmp(Command, "getBoardInfo") == 0) //{Cmd:getBoardInfo}
		  {
			  sendBoardInfo();
		  }
		  else if (strcmp(Command, "SetManualSpeed") == 0) //{Cmd:SetManualSpeed,Speed:127}
		  {
			  M1.writeSpeed(root["Speed"]);
		  }
		  else if (strcmp(Command, "save") == 0) //{Cmd:save}
		  {

		  }
		  else if (strcmp(Command, "setMotorParameters") == 0)
		  {
			  M1.setLimits(root["MotorInfo"][0]["HL"], root["MotorInfo"][0]["LL"], root["MotorInfo"][0]["Offset"]);
			  M2.setLimits(root["MotorInfo"][1]["HL"], root["MotorInfo"][1]["LL"], root["MotorInfo"][1]["Offset"]);
		  }
		  else if (strcmp(Command, "getMotorParameters") == 0)
		  {
			  sendMotorInfo();
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
	  if (CaptureSate != RUNNING) parseSerialData();
	  

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
	  

	  if (CaptureStep > 0 && CaptureTimer >= CAPTURE_SAMPLING_RATE)
	  {
		  uint16_t currentStep = CAPTURE_STEPS - CaptureStep;
		  TimeBuffer[currentStep] = CaptureTimer;
		  OutputBuffer[currentStep] = ActiveCaptureMotor->Output;
		  SetPointBuffer[currentStep] = (uint16_t)ActiveCaptureMotor->ClampedSetPoint;
		  PositionBuffer[currentStep] = (uint16_t)ActiveCaptureMotor->FeedBack;
		  CaptureTimer -= CAPTURE_SAMPLING_RATE;
		  CaptureStep--;
		  if (currentStep == CAPTURE_STEPS_DELAY)
			  ActiveCaptureMotor->setTarget(CaptureTarget, CLAMPED_MODE);
	  }
  }

  void Simulator::adcUpdate()
  {
	  M1.setFeedback(converter->analogRead(AnalogIn1));
	  M2.setFeedback(converter->analogRead(AnalogIn2));
	  /*/
	  converter->startSynchronizedSingleRead(AnalogIn1, AnalogIn2);
	  result = converter->readSynchronizedSingle();
	  M1.setFeedback(result.result_adc0);
	  M2.setFeedback(result.result_adc1);*/
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

  void Simulator::newCaptureTrigger(uint16_t Target, uint8_t motorID)
  {
	  if (motorID == 1) ActiveCaptureMotor = &M1;
	  else ActiveCaptureMotor = &M2;
	  CaptureSate = TRIGGER;
	  CaptureTarget = Target;
	  CaptureStep = CAPTURE_STEPS;

  }

  void Simulator::sendCapture(uint8_t NumberOfFrames, uint8_t NumberOfPoints)
  {/*
	  for (int Frame = 0; Frame < NumberOfFrames; Frame++)
	  {
		  DynamicJsonBuffer jsonBuffer;
		  JsonObject& root = jsonBuffer.createObject();

		  if (Frame == 0) root["Cmd"] = "NewCapture";
		  else root["Cmd"] = "Capture";

		  root["NOfPoints"] = NumberOfPoints;
		  root["NOfFrames"] = NumberOfFrames;
		  root["FrameN"] = Frame + 1;
		  JsonArray& Buffer = root.createNestedArray("Buffer");

		  for (int j = 0; j < NumberOfPoints / NumberOfFrames; j++)
		  {
			  int Point = NumberOfPoints / NumberOfFrames*Frame + j;
			  JsonObject& temp = Buffer.createNestedObject();
			  temp["T"] = Point;
			  temp["CSP"] = clamped_SetPoint[Point];
		  }
		  root.printTo(Serial);
		  Serial.println();
	  }*/
  }
