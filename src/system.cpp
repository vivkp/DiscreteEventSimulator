#include"system.h"
#include<iostream>

System::System(int v1, int v2, int v3): threadPoolContainer(v1), requestBuffer(v2), numOfCores(v3){
        for(int i = 0; i < numOfCores; i++)
            cpu.push_back(Core(i));
        std::cout<<"System initialized"<<std::endl;
    }