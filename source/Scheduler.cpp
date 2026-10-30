#include "Process.h"
#include <vector>
#include <algorithm>
using namespace std;

//  THUẬT TOÁN FCFS 
void runFCFS(vector<Process>& processes) {
    
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime)
            return a.pid < b.pid;
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime; 
        }
        
        p.startTime = currentTime;
        p.completionTime = p.startTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        p.responseTime = p.startTime - p.arrivalTime;
        
        currentTime = p.completionTime;
    }
}
