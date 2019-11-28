#include "LTD.h"

void setup(){
    initLTD();
}

void loop(){
    update();
    humidFan();
    FSM();
}
