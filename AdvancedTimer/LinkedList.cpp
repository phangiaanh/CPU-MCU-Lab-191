#include"LinkedList.h"

Node::Node(func _pointerTask, uint8_t _beginTime, uint8_t _interval){
    pointerTask = _pointerTask;
    count = _beginTime;
    interval = _interval;
    id = 0;
    nextNode = nullptr;
}

void Node::setNextNode(Node* _nextNode){
    nextNode = _nextNode;
}

Node* Node::getNextNode(){
    return nextNode;
}

void Node::exeTask(){
    pointerTask();
}

Node::~Node(){
    delete nextNode;
}

LinkedList::LinkedList(){
    headNode = nullptr;
    tailNode = nullptr;
    size = 0;
    nodeLog = 0;
    timeExist = 0;
}

uint8_t LinkedList::addNode(Node* _newNode){
    // If task queue is full, return error code 0
    if(size == MAX_TASK) return 0;
    // First node
    if (size == 0)
    {
        headNode = _newNode;
        tailNode = _newNode;
        _newNode->setNextNode(nullptr);
    }
    else
    {
        tailNode->setNextNode(_newNode);
        tailNode = _newNode;
        _newNode->setNextNode(nullptr);
    }

    size++;
    if(_newNode->id == 0){
        nodeLog++;
        // Return id based on the number of nodes created
        _newNode->id = nodeLog;
    }
    _newNode->enterQueue = timeExist;
    return _newNode->id;
}

void LinkedList::removeNode(uint8_t _id){
    if(size == 0) return;
    Node* temp = headNode;
    Node* last = temp;
    if(temp->id == _id){
        if(size == 1){
            headNode = nullptr;
            tailNode = nullptr;
        }
        else
        {
            headNode = temp->getNextNode();
            temp->setNextNode(nullptr);    
        }
        delete temp;
        size--;
        return;
    }
    temp = temp->getNextNode();
    while (temp->getNextNode() != nullptr)
    {
        if(temp->id == _id){
            last->setNextNode(temp->getNextNode());
            temp->setNextNode(nullptr);
            delete temp;
            size--;
            return;
        }
        temp = temp->getNextNode();
        last = last->getNextNode();
    }
    if (temp->id == _id)
    {
        tailNode = last;
        last->setNextNode(nullptr);
        delete temp;
        size--;
    }
    else return;
}

uint8_t LinkedList::addNodeCount(Node* _newNode){
    // If task queue is full, return error code 0
    if(size == MAX_TASK) return 0;
    // First node
    if (size == 0)
    {
        headNode = _newNode;
        tailNode = _newNode;
        _newNode->setNextNode(nullptr);
    }
    else if (_newNode->count < (headNode->count - (timeExist - headNode->enterQueue)))
    {
        _newNode->setNextNode(headNode);
        headNode = _newNode;
    }
    else
    {
        Node* temp = headNode;
        while (temp->getNextNode() != nullptr)
        {
            if(((temp->getNextNode())->count - ((timeExist - ((temp->getNextNode())->enterQueue)))) > _newNode->count){
                _newNode->setNextNode(temp->getNextNode());
                temp->setNextNode(_newNode);
                break;
            }
            temp = temp->getNextNode();
        }
        if (temp->getNextNode() == nullptr)
        {
            return addNode(_newNode);
        }
    }
    
    size++;
    if(_newNode->id == 0){
        nodeLog++;
        // Return id based on the number of nodes created
        _newNode->id = nodeLog;
    }
    _newNode->enterQueue = timeExist;
    return _newNode->id;
}

void LinkedList::checkLinkedList(LinkedList* _waitingQueue){
    timeExist++;
    if (size == 0)
    {
        // Maybe a refresh function here
        return;
    }
    
    Node* temp = headNode;
    while((temp->count - (timeExist - temp->enterQueue)) == 0){
        temp->count = temp->interval;
        temp = temp->getNextNode();
        _waitingQueue->addNode(popNode());
        if(temp == nullptr) return;
    }
    return;
}

void LinkedList::releaseQueue(LinkedList* _taskQueue){
    if(size == 0) return;
    while (size != 0)
    {
        headNode->exeTask();
        if(headNode->interval == 0) removeNode(headNode->id);
        else
        {
            _taskQueue->addNodeCount(popNode());
        }
    }
    return;
}

Node* LinkedList::popNode(){
    if(size == 0) return nullptr;
    Node* temp = headNode;
    if(size == 1){
        headNode = nullptr;
        tailNode = nullptr;
    }
    else
    {
        headNode = temp->getNextNode();
    }
    size--;
    return temp;
}

LinkedList::~LinkedList(){
    delete headNode;
    delete tailNode;
}
