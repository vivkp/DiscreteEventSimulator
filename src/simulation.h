#include "system.h"
#include "event.h"
#include "request.h"
#include "core.h"
#include<vector>

//System parameters
#define THREADPOOL_SIZE 50
#define REQUEST_BUFFER_SIZE 100000
#define NUM_OF_CORES 4
#define QUANTUM_TIME 10
#define CONTEXT_SWITCH_TIME 0.1
#define MAX_COMPLETIONS 100
#define NUM_USERS 100
#define THINK_TIME 1.0
#define MEAN_INTER_ARRIVAL_TIME 2
#define MEAN_SERVICE_TIME 25
#define TIMEOUT_TIME 20000

class Simulation{
    public:
    double simTime=0, lastEventTime=0;
    System system{THREADPOOL_SIZE, REQUEST_BUFFER_SIZE, NUM_OF_CORES};
    EventQueue eventQueue;
    std::vector<Request> requestList;
    
    //for metrics
    int numOfCompletions=0, successes=0, timeouts=0, drops=0;
    float accumulatedResponseTime=0;
    std::vector<float> areaServerStatus;

    // simulation controls
    Simulation();
    void initialize();
    void timing(Event curEvent);
    void updateStats();
    void printMetrics();

    //Event Handlers
    void onArrival(Event curEvent);
    void onContextSwitch(Event curEvent);
    void onPreemption(Event curEvent);
    void onDeparture(Event curEvent) ;

};