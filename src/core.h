#pragma once
#include<queue>

#define IDLE 0
#define BUSY 1
#define CS 2


class Core {
    public:
    int id, status;
    std::queue<int> runQueue;

    Core(int _id);
    void addToRunQueue(int requestId);
    int nextInRunQueue();
};
