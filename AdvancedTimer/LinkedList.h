#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<Arduino.h>


#ifndef MAX_TASK
#define MAX_TASK 30
#endif

typedef void (*func)();


class Node
{
private:
    func pointerTask;
    Node* nextNode;
public: 
    uint8_t id;
    uint8_t enterQueue;
    uint8_t interval;
    uint8_t count;

    Node(func _pointerTask, uint8_t _beginTime = 0, uint8_t _interval = 0);
    void setNextNode(Node* _nextNode);
    Node* getNextNode();
    void exeTask();
    ~Node();
};


class LinkedList
{
private:
    Node* headNode;
    Node* tailNode;
public:
    uint8_t size;
    uint8_t nodeLog;
    uint64_t timeExist;
    
    
    LinkedList();
    uint8_t addNode(Node* _newNode);
    uint8_t addNodeCount(Node* _newNode);
    void removeNode(uint8_t _id);
    void checkLinkedList(LinkedList* _waitingQueue);
    void releaseQueue(LinkedList* _taskQueue);
    Node* popNode();
    ~LinkedList();
};
#endif