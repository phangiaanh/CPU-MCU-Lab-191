#include"TCLED.h"

void setup() {
   initialize();
   initializeLCD();
   initializeTimer1();
}

void loop() {
   updateStep();
}
