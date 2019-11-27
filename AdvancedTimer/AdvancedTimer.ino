#include"clock.h"
#include"Task.h"

volatile bool flag = false;
volatile bool flagStop = false;

void task6(){
  flag = true;
}
void task7(){
  flagStop = true;
}

void setup() {
  Serial.begin(9600);
  
  initializeTimer(100);
  addTask(task1, 50, 1);
  addTask(task2, 50, 5);
  addTask(task4, 60, 2);
  ID = addTask(task3, 40, 3);
  addTask(task5, 100, 0);
  addTask(task6, 150, 0);
  addTask(task7, 200, 8);

  startTimer();
}

void loop() {
  dispatch();
  if(flag){
    flag = false;
    Serial.println("Delete tasks 3");
    removeTask(ID);
  }
  if(flagStop){
    flagStop = false;
    Serial.println("Delete all tasks");
    stopTimer();
  }
}
