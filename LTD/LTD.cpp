#include "LTD.h"

// Heatẻ is turned on every 10 seconds and will be timed out after 10 seconds
#define HEATER_PERI 50
#define HEATER_TIMEOUT 10
// Heatpump is turned on every 15 seconds and will be timed out after 15 seconds
#define HEATPUMP_PERI 50 
#define HEATPUMP_TIMEOUT 15
// Maximum temperature: stop the heater 
#define MAXIMUM_TEMPERATURE 39


#define HEATER 1
#define HEATPUMP 2

#define FAN_1 3
#define FAN_2 4
#define FAN_3 5 

#define HEAT_SENSOR 6

DHT sensor(HEAT_SENSOR, DHT11);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

enum {READY, HEATER_ON, HEATPUMP_ON} state;
bool flagHeater = false, flagHeatpump = false;
float humidity = 0, temperature = 0;

void initLTD(){
    Serial.begin(9600);
    Serial.println("Starting LTD...");

    lcd.init();
    sensor.begin();

    pinMode(HEATER, OUTPUT);
    pinMode(HEATPUMP, OUTPUT);
    pinMode(FAN_1, OUTPUT);
    pinMode(FAN_2, OUTPUT);
    pinMode(FAN_3, OUTPUT);

    initializeTimer(1000);
    addTask(taskHeaterOn, 5, HEATER_PERI);
    addTask(taskHeaterOff, 5 + HEATER_TIMEOUT, HEATER_PERI);
    addTask(taskHeaterOn, 10 + HEATER_TIMEOUT, HEATPUMP_PERI);
    addTask(taskHeaterOn, 10 + HEATER_TIMEOUT + HEATPUMP_TIMEOUT, HEATPUMP_PERI);
    startTimer();
}

void update(){
    lcd.setCursor(0, 0);
    lcd.println("Temp: ");
    lcd.println(temperature);
    lcd.setCursor(0, 1);
    lcd.println("Humid: ");
    lcd.println(humidity);

    temperature = sensor.readTemperature();
    humidity = sensor.readHumidity();
}

void humidFan(){
    if(humidity >= 90){
        digitalWrite(FAN_2, HIGH); // Full speed
    }else{
        analogWrite(FAN_2, (int)(humidity*1023/100)); // Change to analog value
    } 
}

void taskHeaterOn(){
    Serial.println("Heater is on!");
    taskHeatpumpOff();

    flagHeater = true;
    digitalWrite(FAN_1, HIGH);
    digitalWrite(HEATER, HIGH);
}

void taskHeaterOff(){
    if(flagHeater){
        Serial.println("XXX Heater is off due to operation: TIME_OUT! XXX")
        flagHeater = false;
        digitalWrite(FAN_1, LOW);
        digitalWrite(HEATER, LOW);    
    }
}

void taskHeatpumpOn(){
    Serial.println("Heatpump is on!");
    taskHeaterOff();

    flagHeatpump = true;
    digitalWrite(FAN_3, HIGH);
    digitalWrite(HEATPUMP, HIGH);
}

void taskHeatpumpOff(){
    if(flagHeatpump){
        flagHeatpump = false;
        digitalWrite(FAN_3, LOW);
        digitalWrite(HEATPUMP, LOW);    
    }
}

void FSM(){
    switch(state){
        case READY:
            if(flagHeater){
                state = HEATER_ON;
            }
            if(flagHeatpump){
                state = HEATPUMP_ON;
            }
            break;
        case HEATER_ON:
            if(temperature >= MAXIMUM_TEMPERATURE){
                flagHeater = false;
                digitalWrite(FAN_1, LOW);
                digitalWrite(HEATER, LOW);    
            }
            if(!flagHeater){ // Heater is off
                state = READY;
            }
            break;
        case HEATPUMP_ON:
            if(!flagHeatpump){
                state = READY;
            } 
            break;
    }
}

