#pragma once
#include "request.h"
#include "thread.h"
#include "core.h"
#include<vector>

class System {
    public:
    ThreadPool threadPoolContainer;
    RequestBuffer requestBuffer;
    std::vector<Core> cpu;
    int numOfCores;

    System(int v1, int v2, int v3);
};