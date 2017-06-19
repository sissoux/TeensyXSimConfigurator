#include "Simulator.h"

Simulator simulator(&Serial, &Serial1, A0, A1, 5);


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
	simulator.parseSerialData();
}

