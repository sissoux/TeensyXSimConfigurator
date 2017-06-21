#include "Simulator.h"

Simulator simulator(&Serial, &Serial1, A0, A1, 300);


void setup() {
  //Serial.begin(115200);
  while (!Serial);
  Serial.println("X-Simulator teensy PID controller.");
  Serial.println("V3.00  21/06/2017  by VisionOfLight");

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  simulator.begin();

  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
	simulator.refreshState();

}

