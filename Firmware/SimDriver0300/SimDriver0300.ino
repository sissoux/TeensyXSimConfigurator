#include "Simulator.h"

Simulator mySim;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  mySim.SendData();
}

void loop() {
  // put your main code here, to run repeatedly:

}
