#ifndef TCLED_H
#define TCLED_H
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Arduino.h>

// Timer interrupt every 10 ms
#define interval 10
// Change state after 1(or 5)s
#define pressRequire1 1
#define pressRequire2 5
// Change counter after every 0.5(or 0.1)s
#define speed1 0.5
#define speed2 0.1

#define button1 D3
#define button2 D4


void ISR();

void initialize();

void initializeTimer1();

void initializeLCD();

// Function used to modify counter
void modCounter(bool up);

// FSM
void updateStep();

#endif
