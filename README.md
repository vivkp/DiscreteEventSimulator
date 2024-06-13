# Discrete-Event-Simulation-Web-Application

## Project Description
### Diagram Representing the entities to simulate
![Web-server-system-diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/Web-Server-System-new.png)
### Characteristics of the system to simulate
This project simulates the following charateristics of a Web application with requests in a closed loop:

* Multi-Core Server Machine
* Multi-threaded Web Server
* Thread-per task model - with buffering for requests
* Round-robin scheduling
* Request time-outs with retries
* Users in closed loop with think time
* Execution Time and timeout - options for distribution

<!-- **Web server system with request in a closed loop** -->
## Discrete Event Simulation
### Discrete Event Simulation General Diagram
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/Discrete-Event-Simulator-General.png)
### Features of a Discrete Event Simulator
* There are **events** which **happen discretely (one at a time)**. 
* All events are **processed sequentially** from a queue called the **event queue**. 
* The **current event** is the event which is **popped from the event queue**. (**In fig. Event - B**)
* Based upon the event **corresponding event handler is called**.(**In fig. B_Handler**)
* The **event handler** itself leads to the **creation of more events** which are **pushed** in the **event queue**.(**In fig. Event - D,A**)
* This **cycle continues** till a **specified condition or the queue becomes empty**.

In our Simulation there are 4 events with 4 corresponding event handlers:
1. Request Arrival - onArrival()
1. Thread Context Switch - onContextSwitch()
1. Thread Preemption - onPreemption()
1. Request Departure - onDeparture()

*For more details check the **[simulation.cpp](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/src/simulation.cpp)** file.*
## Build Instructions
### Linux
**Build Instructions**
```
$ mkdir build  
$ cd build  
$ cmake ..  
$ make
```
**To run** (make sure you are in **./build/src** from the root directory of this project)
```
$ ./main
```
## Performance Metrics
### Average Response Time vs number of users
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/restime-vs-users.png)
### Average core utilization vs number of users
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/util-vs-users.png)
### Throughput vs number of users
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/Tput-vs-users.png)
### Goodput vs number of users
**Goodput: Requests that complete before time-out**
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/gput-vs-users.png)
### Badput vs number of users
**Badput: Requests that complete after time-out**
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/Bput-vs-users.png)
### Drops vs number of users
![Discrete-Event-Simulation-Diagram](https://github.com/jatin-jatin/Discrete-Event-Simulation-Web-Application/blob/main/pictures/Drops-vs-users.png)