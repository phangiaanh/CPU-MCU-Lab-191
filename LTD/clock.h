#ifndef CLOCK_H
#define CLOCK_H
#include"LinkedList.h"


#define INTERVAL 1000

void initializeTimer(uint32_t _interval);

void startTimer();

uint8_t addTask(void (*_func)(void), uint8_t _delay, uint8_t _interval);

void dispatch();

void removeTask(uint8_t _id);

void stopTimer();

void ISR();

#endif
