#ifndef LTD_H
#define LTD_H
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "clock.h" // The scheduler

void initLTD();

void FSM();

void taskHeater();

void taskHeatpump();

void humidFan();

#endif