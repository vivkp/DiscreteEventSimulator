#include "simulation.h"
#include<iostream>
#include<iomanip>

Simulation::Simulation(){
        std::cout<<"Sim initialized"<<std::endl;
        // to call an instructor
        for(int i = 0; i < NUM_OF_CORES; i++)
            areaServerStatus.push_back(0);
        std::cout<<"Metric initialized"<<std::endl; 
    }

void Simulation::initialize() {
        simTime = 0;
        lastEventTime = 0;
        //create initial requests
        for(int i = 0; i < NUM_USERS; i++) {
            requestList.push_back(Request(i,-1,MEAN_INTER_ARRIVAL_TIME, MEAN_SERVICE_TIME, TIMEOUT_TIME));
            eventQueue.schedule(Event(ARRIVAL, requestList[i].arrivalTime, requestList[i].id, -1));            
        }

        std::cout<<"\n-----------------System parameters--------------------------------- "<<std::endl;
        std::cout<<std::setw(10)<<std::left<<"Number of cores: "
        <<std::setw(10)<<std::left<<NUM_USERS<<std::endl
        <<std::setw(10)<<std::left<<"Threadpool size: "
        <<std::setw(10)<<std::left<<THREADPOOL_SIZE<<std::endl
        <<std::setw(10)<<std::left<<"Request buffer size: "
        <<std::setw(10)<<std::left<<REQUEST_BUFFER_SIZE<<std::endl
        <<std::setw(10)<<std::left<<"Quantum time: "
        <<std::setw(10)<<std::left<<QUANTUM_TIME<<std::endl
        <<std::setw(10)<<std::left<<"Context switch time: "
        <<std::setw(10)<<std::left<<CONTEXT_SWITCH_TIME<<std::endl
        <<std::setw(10)<<std::left<<"Number of users: "
        <<std::setw(10)<<std::left<<NUM_USERS<<std::endl
        <<std::setw(10)<<std::left<<"Maximum number of completions: "
        <<std::setw(10)<<std::left<<MAX_COMPLETIONS<<std::endl
        <<std::setw(10)<<std::left<<"Mean think time: "
        <<std::setw(10)<<std::left<<THINK_TIME<<std::endl
        <<std::setw(10)<<std::left<<"Mean inter-arrival time: "
        <<std::setw(10)<<std::left<<MEAN_INTER_ARRIVAL_TIME<<std::endl
        <<std::setw(10)<<std::left<<"Mean service time: "
        <<std::setw(10)<<std::left<<MEAN_SERVICE_TIME<<std::endl;

        std::cout<<"\nSimulation starting...\n"<<std::endl;
        const std::string line = "|" + std::string( 53, '-' ) + '|' ;
        std::cout <<line<<"\n" 
        <<"|"<<std::setw(10)<<std::left<<"simTime"<<"|"
        <<std::setw(20)<<std::left<<"eventType"<<"|"
        <<std::setw(10)<<std::left<<"requestId"<<"|"
        <<std::setw(10)<<std::left<<"coreId"<<"|\n"
        <<line<<std::endl;
    }

void Simulation::timing(Event curEvent) {
        lastEventTime = simTime;
        simTime = curEvent.time;
        
        std::string eventType;
        if(curEvent.type == ARRIVAL) 
            eventType = "ARRIVAL";
        else if(curEvent.type == CONTEXT_SWITCH) 
            eventType = "CONTEXT_SWITCH";
        else if(curEvent.type == DEPARTURE) 
            eventType = "DEPARTURE";
        else if(curEvent.type == PREEMPTION) 
            eventType = "PREEMPTION";
        
        std::cout <<"|"<<std::setw(10)<<std::left<<simTime<<"|"
        <<std::setw(20)<<std::left<<eventType<<"|"
        <<std::setw(10)<<std::left<<curEvent.requestId<<"|"
        <<std::setw(10)<<std::left<<curEvent.coreId<<"|"<<std::endl;
    } 

void Simulation::updateStats() {
        for(int i = 0; i < NUM_OF_CORES; i++)
            areaServerStatus[i] += (system.cpu[i].status == BUSY) * (simTime - lastEventTime);
    }

void Simulation::printMetrics() {
        std::cout<<"\n-----------------------------Metrics--------------------------------------"<<std::endl;
        std::cout<<"Average response time: "<<accumulatedResponseTime/numOfCompletions<<std::endl;
        for(int i = 0; i < NUM_OF_CORES; i++)
            std::cout << "Core-"<<i<<" utilization: "<<areaServerStatus[i]/simTime<<std::endl;
        std::cout<<"Throughput: "<<numOfCompletions/simTime<<std::endl;
        std::cout<<"Goodput: "<<successes/simTime<<std::endl;
        std::cout<<"Badput: "<<timeouts/simTime<<std::endl;
    }

// Event Handlers
    void Simulation::onArrival(Event curEvent) {
        Request* curRequest = &requestList[curEvent.requestId]; 
        //threadPool empty?
        if(system.threadPoolContainer.threadPool.empty() == 1) {
            system.requestBuffer.enqueue(curEvent.requestId);
            //handle dropped requests here
        }
        //thread available
        else {
            //bind thread to request
            curRequest->threadId = system.threadPoolContainer.assignThread();
            //bind thread to core
            int coreId = curRequest->threadId % NUM_OF_CORES;
            //core busy or context switching?
            if(system.cpu[coreId].status == BUSY || system.cpu[coreId].status == CS) {
                system.cpu[coreId].addToRunQueue(curRequest->id);
            }
            // core idle
            else {
                Event contextSwitch(CONTEXT_SWITCH, simTime + CONTEXT_SWITCH_TIME, curRequest->id, coreId);
                eventQueue.schedule(contextSwitch);
                system.cpu[coreId].status = CS;
            }
        }
    }

    void Simulation::onContextSwitch(Event curEvent) {
        Request* curRequest = &requestList[curEvent.requestId];
        //remaining service time < quantum time? -> schedule departure
        if(curRequest->remainingServiceTime < QUANTUM_TIME){
            Event departure(DEPARTURE, simTime + curRequest->remainingServiceTime, curRequest->id, curEvent.coreId);
            curRequest->remainingServiceTime = 0;
            eventQueue.schedule(departure);
        }
        //sechudle preemption
        else {
            Event preemption(PREEMPTION, simTime + QUANTUM_TIME, curRequest->id, curEvent.coreId);
            curRequest->remainingServiceTime -= QUANTUM_TIME;
            eventQueue.schedule(preemption);
        }
        system.cpu[curEvent.coreId].status = BUSY;
    }

    void Simulation::onPreemption(Event curEvent) {
        Request *curRequest = &requestList[curEvent.requestId];
        //check if run queue of core running current request is empty or not
        if (system.cpu[curEvent.coreId].runQueue.empty() == 1){
            if(curRequest->remainingServiceTime <= QUANTUM_TIME){
                Event departure(DEPARTURE,simTime + curRequest->remainingServiceTime, curEvent.requestId, curEvent.coreId);
                eventQueue.schedule(departure);
                curRequest->remainingServiceTime = 0;
                //cpu already busy
            }
            else {
                Event preemption(PREEMPTION,simTime + QUANTUM_TIME,curEvent.requestId,curEvent.coreId);
                eventQueue.schedule(preemption);
                curRequest->remainingServiceTime -= QUANTUM_TIME;
                //cpu already busy
            }
        }
        //context switch next req/thread from run queue
        else {
            // enqueue, dequeue
            system.cpu[curEvent.coreId].addToRunQueue(curEvent.requestId);
            int nextRequestid = system.cpu[curEvent.coreId].nextInRunQueue();
            
            //schedule context switch
            Event contextSwitch(CONTEXT_SWITCH,simTime + CONTEXT_SWITCH_TIME,nextRequestid,curEvent.coreId);
            eventQueue.schedule(contextSwitch);
            system.cpu[curEvent.coreId].status = CS;
        }
    }

    void Simulation::onDeparture(Event curEvent) {
        //record stats
        float resonseTime = simTime - requestList[curEvent.requestId].arrivalTime;
        if(resonseTime > requestList[curEvent.requestId].timeoutTime)
            timeouts++;
        else
            successes++;
        accumulatedResponseTime += resonseTime;

        //check run queue status
        if(system.cpu[curEvent.coreId].runQueue.empty()) {
            system.cpu[curEvent.coreId].status = IDLE;
        }
        else {
            int nextRequestid = system.cpu[curEvent.coreId].nextInRunQueue();
            Event contextSwitch(CONTEXT_SWITCH,simTime + CONTEXT_SWITCH_TIME,nextRequestid,curEvent.coreId);
            eventQueue.schedule(contextSwitch);
            system.cpu[curEvent.coreId].status = CS;
        }

        //handle requests buffered in
        //If req buffer is empty, free the current thread
        if(system.requestBuffer.buffer.empty()){
            system.threadPoolContainer.releaseThread(requestList[curEvent.requestId].threadId);
        }
        //Take a new request from buffer and assign it the current thread
        else {
            int newRequest = system.requestBuffer.dequeue();
            requestList[newRequest].threadId = requestList[curEvent.requestId].threadId;
            int coreId = requestList[newRequest].threadId % NUM_OF_CORES;

            //if core is busy or cs then put new request to run queue of core
            if(system.cpu[coreId].status == BUSY || system.cpu[coreId].status == CS) {
                system.cpu[coreId].addToRunQueue(newRequest);
            }
            //If core is idle schedule cs for new request
            else {
                Event contextSwitch(CONTEXT_SWITCH, simTime + CONTEXT_SWITCH_TIME, newRequest, coreId);
                eventQueue.schedule(contextSwitch);
                system.cpu[coreId].status = CS;
            }
        }
        //re-initialize departed request
        int curRequestid = curEvent.requestId;
        requestList[curRequestid] = Request(curRequestid,-1,simTime + THINK_TIME, MEAN_SERVICE_TIME, TIMEOUT_TIME);
        //schedule arrival of departed request
        Event arrival(ARRIVAL,requestList[curRequestid].arrivalTime,curRequestid,-1);
        eventQueue.schedule(arrival);        
        numOfCompletions++;
    }