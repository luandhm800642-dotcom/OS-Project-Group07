#ifndef Process_h
#define Process_h

#include <string>

struct Process {
    int pid;             
    int arrivalTime;     
    int burstTime;       
    int startTime;       
    int completionTime;  
    int turnaroundTime;  
    int waitingTime;     
    int responseTime;   
};

#endif
