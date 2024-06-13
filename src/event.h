# pragma once

#include<queue>

//event types
#define ARRIVAL 0
#define CONTEXT_SWITCH 1
#define PREEMPTION 2
#define DEPARTURE 3

class Event {
    
    public:
    int type, requestId, coreId;
    float time;
    Event(int v1, int v2, int v3, int v4);
};


struct myComparator
{
    bool operator() (Event const& e1, Event const& e2)
    {
        return e1.time > e2.time;
    }
};

class EventQueue {
    public:
    std::priority_queue<Event, std::vector<Event>, myComparator> EventMinHeap;

    EventQueue();
    int schedule(Event e);
    Event next();
};