#include <Arduino.h>
#include "LTD.h"

void setup(){
    Serial.begin(9600);
    initLTD();
    delay(100);
    Serial.println("OK");
}

void loop(){
    dispatch();
    //update();
    //humidFan();
    //FSM();
}
