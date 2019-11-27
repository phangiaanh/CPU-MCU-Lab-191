#ifndef DC_H
#define DC_H
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Arduino.h>

#define SPEED1 0.5
#define BUTTON1 D3
#define BUTTON2 D4

void readButtons();

void displayLCD();

void initialize();

void initializeTimer01();

void initializeLCD();

void FSM();

void ISR();
#endif
