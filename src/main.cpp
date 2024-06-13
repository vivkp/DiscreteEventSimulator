#include "simulation.h"
#include<iostream>

int main(){
    Simulation simulation;
    simulation.initialize();

    while(simulation.numOfCompletions < MAX_COMPLETIONS) {
        if(simulation.eventQueue.EventMinHeap.empty()){
            std::cout<<"Event queue empty. Stopping simulation."<<std::endl;
            break;       
        }

        Event curEvent = simulation.eventQueue.next();
        
        simulation.timing(curEvent);

        simulation.updateStats();

        if(curEvent.type == ARRIVAL)
            simulation.onArrival(curEvent);
        else if(curEvent.type == CONTEXT_SWITCH)
            simulation.onContextSwitch(curEvent);
        else if(curEvent.type == PREEMPTION)
            simulation.onPreemption(curEvent);
        else if(curEvent.type == DEPARTURE)
            simulation.onDeparture(curEvent);
    }

    const std::string line = "|" + std::string( 53, '-' ) + '|' ;
    std::cout <<line<<"\n";        
    std::cout<<"\nStopping simulation..."<<std::endl;

    simulation.printMetrics();
    return 0;
}