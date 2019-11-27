#include"DC.h"

void setup(){
   pinMode(BUTTON1, INPUT_PULLUP);
   pinMode(BUTTON2, INPUT_PULLUP);
   initialize();
}

void loop(){
   FSM();
}
