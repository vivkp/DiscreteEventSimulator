#include "core.h"
#include <iostream>

Core::Core(int _id): id(_id), status(IDLE){
        std::cout<<"Core initialized : "<<id<<std::endl;
    }

void Core::addToRunQueue(int requestId) {
        runQueue.push(requestId);
    }

int Core ::nextInRunQueue() {
        int nextRequest = runQueue.front();
        runQueue.pop();
        return nextRequest;
    }