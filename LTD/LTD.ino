#include <Arduino.h>
#include "LTD.h"

void setup(){
    initLTD();
}

void loop(){
    dispatch();
    updateDHT();
    humidFan();
    FSM();
}
