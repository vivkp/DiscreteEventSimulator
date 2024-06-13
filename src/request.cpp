#include "request.h"

Request::Request(int v1, int v2, double v3, double v4, double v5): id(v1), 
    threadId(v2), arrivalTime(v3), serviceTime(v4), timeoutTime(v5) ,remainingServiceTime(v4){}

RequestBuffer::RequestBuffer(int v1): size(v1) {}
int RequestBuffer::enqueue(int requestId){
        // add request if buffer is not full
        if(buffer.size() < size) {
            buffer.push(requestId);
            return 0;
        }
        else   
            return -1;
    }


int RequestBuffer::dequeue(){
        int nextRequestId = buffer.front();
        buffer.pop();
        return nextRequestId;
    }