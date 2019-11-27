#include "LTD.h"
#define HEATER_PERI 10
#define HEATER_TIMEOUT 10
// Heatpump is turned on every 15 seconds and will be timed out after 15 seconds
#define HEATPUMP_PERI 15 
#define HEATPUMP_TIMEOUT 15

#define HEATER 1
#define HEATPUMP 2

#define FAN_1 3
#define FAN_2 4
#define FAN_3 5 

#define HEAT_SENSOR 6

DHT sensor(HEAT_SENSOR, DHT11);

enum {HEATER_ON, HETPUMP_ON} state;
bool flagHeater = true, flagHeatpump = false;

void initLTD(){
    pinMode(HEATER, OUTPUT);
    pinMode(HEATPUMP, OUTPUT);
    pinMode(FAN_1, OUTPUT);
    pinMode(FAN_2, OUTPUT);
    pinMode(FAN_3, OUTPUT);


}