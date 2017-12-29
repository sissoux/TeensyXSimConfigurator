#include "Simulator.h"

Simulator simulator(&Serial, &Serial1, A1, A0, A2, 500);	//(Main Com Serial, Motor Serial, Position feedback 1, Position feedback 2, PID refresh rate in ms)

#define MOTOR_STOP_PIN 20
#define STATUS_LED_PIN 13
#define OUT_EN_PIN 2

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial1.begin(9600);
  Serial.println("X-Simulator teensy PID controller.");
  Serial.println("V3.00  21/06/2017  by VisionOfLight");

  pinMode(A0, INPUT);	// Position Feedback 1
  pinMode(A1, INPUT);	// Position Feedback 2
  pinMode(A2, INPUT);	// PSU Current
  pinMode(20, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(OUT_EN_PIN, OUTPUT);
  digitalWrite(20, LOW);
  digitalWrite(STATUS_LED_PIN, HIGH);
  simulator.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
	simulator.refreshState();
}

