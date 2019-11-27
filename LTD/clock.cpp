#include"clock.h"

LinkedList* taskQueue;
LinkedList* waitQueue;

void initializeTimer(uint32_t _interval){
    noInterrupts();
    taskQueue = new LinkedList();
    waitQueue = new LinkedList();

    
    timer1_isr_init();
    timer1_attachInterrupt(ISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(5000 * _interval); 
}

void startTimer(){
    interrupts();
}

uint8_t addTask(void (*_func)(void), uint8_t _delay, uint8_t _interval){
    Node* newNode = new Node((func)_func, _delay, _interval);
    taskQueue->addNodeCount(newNode);
}

void dispatch(){
    waitQueue->releaseQueue(taskQueue);
}

void removeTask(uint8_t _id){
    waitQueue->removeNode(_id);
    taskQueue->removeNode(_id);
}

void stopTimer(){
    timer1_disable();
    /*delete taskQueue;
    delete waitQueue;
    taskQueue = nullptr;
    waitQueue = nullptr;*/
}

void ISR(){
    taskQueue->checkLinkedList(waitQueue);
}
