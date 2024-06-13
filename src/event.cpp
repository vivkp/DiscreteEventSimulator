#include "event.h"
#include <iostream>

Event::Event(int v1, int v2, int v3, int v4): type(v1), time(v2), requestId(v3), coreId(v4){}

EventQueue::EventQueue(){
        std::cout<<"EventQueue initialized"<<std::endl;
        }
int EventQueue::schedule(Event e) {
        EventMinHeap.push(e);
        return 0;
    }
Event EventQueue::next(){
        Event nextEvent = EventMinHeap.top();
        EventMinHeap.pop();
        return nextEvent;
    }