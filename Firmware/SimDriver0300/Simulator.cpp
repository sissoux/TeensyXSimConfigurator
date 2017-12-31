#include "Simulator.h"
#include <ArduinoJson.h>
#include <ADC.h>



Simulator::Simulator(Stream *MainSerial, Stream *MotorSerial, int ADCIn1, int ADCIn2, int ADCIn3, uint16_t PID_Rate) : AnalogIn1(ADCIn1), AnalogIn2(ADCIn2), CurrentAnalogIn(ADCIn3), M1(1, 2, 0, 0, MotorSerial), M2(2, 2, 0, 0, MotorSerial)
  {
	  this->serial = MainSerial;
	  setPIDRefreshRate(PID_Rate);
  }
  
void Simulator::begin()
{
	if (EEPROM.read(EEPROM_START_ADDRESS) == INITIALIZED_VALUE)
		recallEEPROM();
	initADC();
	M1.setTarget(32768, CLAMPED_MODE);
	M2.setTarget(32768, CLAMPED_MODE);
	M1.setSampleTime(PIDRefreshRate);
	M2.setSampleTime(PIDRefreshRate);
	//M1.startPID();
	//M2.startPID();
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
			  newCaptureTrigger(50000, 1);
			  //Trigger Capture
			  //Once finished Send data buffer

		  }
		  else if (strcmp(Command, "getBoardInfo") == 0) //{Cmd:getBoardInfo}
		  {
			  sendBoardInfo();
		  }
		  else if (strcmp(Command, "setPosition") == 0) //{Cmd:setPosition}
		  {
			  M1.setTarget(root["MotorInfo"][0]["target"], CLAMPED_MODE);
			  M2.setTarget(root["MotorInfo"][1]["target"], CLAMPED_MODE);
		  }
		  else if (strcmp(Command, "XSIMPos") == 0) //{Cmd:setPosition}
		  {
			  M1.setTarget(root["M1"], CLAMPED_MODE);
			  M2.setTarget(root["M2"], CLAMPED_MODE);
		  }
		  else if (strcmp(Command, "StartSim") == 0) //{Cmd:StartSim}
		  {
			  M1.setToMiddlePoint();
			  M2.setToMiddlePoint();
			  M1.startPID();
			  M2.startPID();
		  }
		  else if (strcmp(Command, "StopSim") == 0) //{Cmd:StopSim}
		  {
			  M1.setToMiddlePoint();
			  M2.setToMiddlePoint();
			  M1.stopPID();
			  M2.stopPID();
		  }
		  else if (strcmp(Command, "save") == 0) //{Cmd:save}
		  {
			  this->saveToEEPROM();
		  }
		  else if (strcmp(Command, "clearEEPROM") == 0) //{Cmd:save}
		  {
			  this->clearEEPROM();
		  }
		  else if (strcmp(Command, "setMotorParameters") == 0)
		  {
			  M1.setPID(root["MotorInfo"][0]["kp"], root["MotorInfo"][0]["ki"], root["MotorInfo"][0]["kd"]);
			  M2.setPID(root["MotorInfo"][1]["kp"], root["MotorInfo"][1]["ki"], root["MotorInfo"][1]["kd"]);
		  }
		  else if (strcmp(Command, "setHL") == 0)
		  {
			  int motorID = root["MotorInfo"][0]["ID"];
			  switch (motorID)
			  {
			  case 1:
				  M1.setHighLimit();
				  break;
			  case 2:
				  M2.setHighLimit();
				  break;
			  default:
				  break;
			  }
		  }
		  else if (strcmp(Command, "setLL") == 0)
		  {
			  int motorID = root["MotorInfo"][0]["ID"];
			  switch (motorID)
			  {
			  case 1:
				  M1.setLowLimit();
				  break;
			  case 2:
				  M2.setLowLimit();
				  break;
			  default:
				  break;
			  }
		  }
		  else if (strcmp(Command, "setOffset") == 0)
		  {
			  int motorID = root["MotorInfo"][0]["ID"];
			  switch (motorID)
			  {
			  case 1:
				  M1.setOffset();
				  break;
			  case 2:
				  M2.setOffset();
				  break;
			  default:
				  break;
			  }
		  }
		  else if (strcmp(Command, "resetLimits") == 0)
		  {
			  M1.setLimits(65535, 0, HALF_16BIT);
			  M2.setLimits(65535, 0, HALF_16BIT);
		  }
		  else if (strcmp(Command, "getMotorParameters") == 0)
		  {
			  sendMotorInfo();
		  }
		  else if (strcmp(Command, "getPosition") == 0)
		  {
			  Serial.println(M1.FeedBack);
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
	  root["PIDRate"] = this->PIDRefreshRate;

	  root.printTo(Serial);
	  Serial.println();
  }

  void Simulator::sendMotorInfo()
  {
	  DynamicJsonBuffer jsonBuffer;
	  JsonObject& root = jsonBuffer.createObject();

	  root["Cmd"] = "MotorInfo";

	  JsonArray& MotorInfo = root.createNestedArray("MotorInfo");

	  JsonObject& Motor1Object = MotorInfo.createNestedObject();
	  Motor1Object["ID"] = 1;
	  Motor1Object["Position"] = M1.FeedBack;
	  Motor1Object["kp"] = M1.dispKp;
	  Motor1Object["ki"] = M1.dispKi;
	  Motor1Object["kd"] = M1.dispKd;
	  Motor1Object["HL"] = M1.dispHL;
	  Motor1Object["LL"] = M1.dispLL;
	  Motor1Object["Offset"] = M1.dispOffset;
	  JsonObject& Motor2Object = MotorInfo.createNestedObject();
	  Motor2Object["ID"] = 2;
	  Motor2Object["Position"] = M2.FeedBack;
	  Motor2Object["kp"] = M2.dispKp;
	  Motor2Object["ki"] = M2.dispKi;
	  Motor2Object["kd"] = M2.dispKd;
	  Motor2Object["HL"] = M2.dispHL;
	  Motor2Object["LL"] = M2.dispLL;
	  Motor2Object["Offset"] = M2.dispOffset;
	  
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
		  M1.writeOutput();
		  M2.writeOutput();
	  }


	  if (CaptureSate == RUNNING && CaptureStep > 0 && CaptureTimer >= CAPTURE_SAMPLING_RATE)
	  {
		  uint16_t currentStep = CAPTURE_STEPS - CaptureStep;
		  TimeBuffer[currentStep] = currentStep*CAPTURE_SAMPLING_RATE;
		  OutputBuffer[currentStep] = ActiveCaptureMotor->Output;
		  SetPointBuffer[currentStep] = (uint16_t)ActiveCaptureMotor->ClampedSetPoint;
		  PositionBuffer[currentStep] = (uint16_t)ActiveCaptureMotor->FeedBack;
		  CaptureTimer -= CAPTURE_SAMPLING_RATE;
		  CaptureStep--;
		  if (currentStep == CAPTURE_STEPS_DELAY)
			  ActiveCaptureMotor->setTarget(CaptureTarget, CLAMPED_MODE);
	  }
	  else if (CaptureSate == INIT && CaptureTimer >= CAPTURE_INIT_DELAY)	//We wait some time for the motor to reach middle point, once done we launch the capture sequence
	  {
		  CaptureTimer = 0;
		  CaptureSate = RUNNING;
		  digitalWrite(13, LOW);
	  }
	  else if (CaptureSate == RUNNING && CaptureStep == 0)	//Capture is finished, time to send buffers
	  {
		  CaptureSate = STOPPED;
		  digitalWrite(13, HIGH);
		  sendCapture(CAPTURE_STEPS);
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
	  M1.setSampleTime(this->PIDRefreshRateInS);
	  M2.setSampleTime(this->PIDRefreshRateInS);
  }

  void Simulator::newCaptureTrigger(uint16_t Target, uint8_t motorID)
  {
	  if (motorID == 1) ActiveCaptureMotor = &M1;
	  else ActiveCaptureMotor = &M2;
	  CaptureSate = INIT;
	  CaptureTarget = Target;
	  CaptureStep = CAPTURE_STEPS;
	  ActiveCaptureMotor->setToMiddlePoint();
	  CaptureTimer = 0;	//Reset local timer
  }

  void Simulator::sendCapture(uint16_t NumberOfPoints)
  {
	  uint8_t NumberOfFrames = (NumberOfPoints - 1) / FRAME_CAPACITY +1;	//Create enough Frames to send all data

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

		  for (int j = 0; j < FRAME_CAPACITY; j++)
		  {
			 uint16_t CurrentPoint = j + Frame * FRAME_CAPACITY;
			 if (j >= NumberOfPoints) break;
				JsonObject& temp = Buffer.createNestedObject();
				temp["T"] = TimeBuffer[CurrentPoint];
				temp["CSP"] = SetPointBuffer[CurrentPoint];
				  temp["RawPos"] = PositionBuffer[CurrentPoint];
				  temp["Out"] = OutputBuffer[CurrentPoint];
		  }
		  root.printTo(Serial);
		  Serial.println();
	  }
  }

  void Simulator::saveToEEPROM()
  {
	  EEPROM.write(EEPROM_START_ADDRESS , INITIALIZED_VALUE);	//Set "EEPROM Initialized"
	  int Index = 1;
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispKp);
	  Index += sizeof(M1.dispKp);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispKi);
	  Index += sizeof(M1.dispKi);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispKd);
	  Index += sizeof(M1.dispKd);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispHL);
	  Index += sizeof(M1.dispHL);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispLL);
	  Index += sizeof(M1.dispLL);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M1.dispOffset);
	  Index += sizeof(M1.dispOffset);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispKp);
	  Index += sizeof(M2.dispKp);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispKi);
	  Index += sizeof(M2.dispKi);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispKd);
	  Index += sizeof(M2.dispKd);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispHL);
	  Index += sizeof(M2.dispHL);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispLL);
	  Index += sizeof(M2.dispLL);
	  EEPROM.put(EEPROM_START_ADDRESS + Index, M2.dispOffset);
  }
  void Simulator::clearEEPROM()
  {
	  for (int i = 0; i < EEPROM.length(); i++) {
		  EEPROM.write(i, 0);
	  }
  }
  void Simulator::recallEEPROM()
  {
	  double p, i, d;
	  uint16_t offset, HL, LL;
	  int Index = 1;
	  EEPROM.get(EEPROM_START_ADDRESS + Index,p);
	  Index += sizeof(M1.dispKp);
	  EEPROM.get(EEPROM_START_ADDRESS + Index,i );
	  Index += sizeof(M1.dispKi);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, d);
	  Index += sizeof(M1.dispKd);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, HL);
	  Index += sizeof(M1.dispHL);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, LL);
	  Index += sizeof(M1.dispLL);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, offset);
	  M1.setPID(p, i, d);
	  M1.setLimits(HL, LL, offset);
	  Index += sizeof(M1.dispOffset);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, p);
	  Index += sizeof(M1.dispKp);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, i);
	  Index += sizeof(M1.dispKi);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, d);
	  Index += sizeof(M1.dispKd);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, HL);
	  Index += sizeof(M1.dispHL);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, LL);
	  Index += sizeof(M1.dispLL);
	  EEPROM.get(EEPROM_START_ADDRESS + Index, offset);
	  M2.setPID(p, i, d);
	  M2.setLimits(HL, LL, offset);
  }
