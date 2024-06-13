#include"thread.h"
#include<iostream>

ThreadPool::ThreadPool(int size) {
        for(int i = 0; i < size; i++)
            threadPool.push(i);
        std::cout<<"ThreadPool initialized"<<std::endl;
    }
int ThreadPool::assignThread() {
        if(!threadPool.empty()) { // if thread available
            int threadId = threadPool.front();
            threadPool.pop();
            return threadId;
        }
        else 
            return -1;
    }
void ThreadPool::releaseThread(int threadId) {
        threadPool.push(threadId);
    }