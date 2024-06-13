#pragma once
#include<queue>
class ThreadPool {
    public:
    std::queue<int> threadPool;
    
    ThreadPool(int size);
    int assignThread();
    void releaseThread(int threadId);

};