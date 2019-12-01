#include "LTD.h"

// Heatẻ is turned on every 10 seconds and will be timed out after 10 seconds
#define HEATER_PERI 50
#define HEATER_TIMEOUT 10
// Heatpump is turned on every 15 seconds and will be timed out after 15 seconds
#define HEATPUMP_PERI 50 
#define HEATPUMP_TIMEOUT 15
// Maximum temperature: stop the heater 
#define MAXIMUM_TEMPERATURE 30


#define HEATER D4
#define HEATPUMP D5

#define FAN_1 D4
#define FAN_2 D3
#define FAN_3 D5

#define HEAT_SENSOR D6

DHT sensor(HEAT_SENSOR, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum {READY, HEATER_ON, HEATPUMP_ON} state;
extern bool flagHeater = false, flagHeatpump = false;
extern float humidity = 0, temperature = 0;

void initLTD(){
    Serial.begin(9600);

    lcd.init();
    sensor.begin();

    pinMode(HEAT_SENSOR, INPUT);
    pinMode(HEATER, OUTPUT);
    pinMode(HEATPUMP, OUTPUT);
    pinMode(FAN_1, OUTPUT);
    pinMode(FAN_3, OUTPUT);

    initializeTimer(INTERVAL);
    addTask(taskHeaterOn, 5, HEATER_PERI);
    addTask(taskHeaterOff, 5 + HEATER_TIMEOUT, HEATER_PERI);
    addTask(taskHeatpumpOn, 10 + HEATER_TIMEOUT, HEATPUMP_PERI);
    addTask(taskHeatpumpOff, 10 + HEATER_TIMEOUT + HEATPUMP_TIMEOUT, HEATPUMP_PERI);
    startTimer();
    
    Serial.println("Starting LTD...");
}

void updateDHT(){
    temperature = sensor.readTemperature();
    humidity = sensor.readHumidity();
    if(isnan(temperature) || isnan(humidity)) return;
    lcd.setCursor(0, 0);
    lcd.println("Temp: ");
    lcd.println(temperature/256);
    lcd.setCursor(0, 1);
    lcd.println("Humid: ");
    lcd.println(humidity/256);
}

void humidFan(){
    if(isnan(humidity)) return;
    if(humidity/256 >= 90){
        analogWrite(FAN_2, 1023); // Full speed
    }else{
        analogWrite(FAN_2, (int)(humidity*1023/50000)); // Change to analog value
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
        Serial.println("XXX Heater is off due to operation: TIME_OUT! XXX");
        flagHeater = false;
        digitalWrite(FAN_1, LOW);
        digitalWrite(HEATER, LOW);    
    }
    else if(!flagHeatpump){
        Serial.println("OOO Heater is off due to reaching MAXIMUM_TEMPERATURE! OOO");
    }
}

void taskHeatpumpOn(){
    Serial.println("Heatpump is on!");

    flagHeatpump = true;
    digitalWrite(FAN_3, HIGH);
    digitalWrite(HEATPUMP, HIGH);
    taskHeaterOff();
}

void taskHeatpumpOff(){
    if(flagHeatpump){
        Serial.println("YYY Heatpump is off due to operation: TIME_OUT! YYY");
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
            if(isnan(temperature)) return;
            if(temperature/256 >= MAXIMUM_TEMPERATURE){
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
