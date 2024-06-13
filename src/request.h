#pragma once
#include<queue>
class Request {
    public:
    int id, threadId;
    double arrivalTime, remainingServiceTime, serviceTime, timeoutTime;
    Request(int v1, int v2, double v3, double v4, double v5);
};

class RequestBuffer {
    public:
    int size;
    std::queue<int> buffer;
    
    RequestBuffer(int v1);
    int enqueue(int requestId);
    int dequeue();
};