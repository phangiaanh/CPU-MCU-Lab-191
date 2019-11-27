#ifndef TASK_H
#define TASK_H
#include<Arduino.h>

volatile uint8_t ID; 

void task1(){
    Serial.println("Task 1");
}
void task2(){
    Serial.println("Task 2");
}
void task3(){
    Serial.println("Task 3");
}
void task4(){
    Serial.println("Task 4");
}
void task5(){
    Serial.println("Task 5 - GoodByeeeeeeeeeeeeee");
}
#endif
