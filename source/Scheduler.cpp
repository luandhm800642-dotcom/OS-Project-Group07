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
// THUẬT TOÁN SJF KHÔNG ĐỘC QUYỀN 
void runSJF(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int completedCount = 0;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (completedCount < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                }
                else if (processes[i].burstTime == minBurst) {
                    if (processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    } else if (processes[i].arrivalTime == processes[idx].arrivalTime) {
                        if (processes[i].pid < processes[idx].pid) {
                            idx = i;
                        }
                    }
                }
            }
        }

        if (idx == -1) {
            currentTime++; 
        } else {
            Process& p = processes[idx];
            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            p.responseTime = p.startTime - p.arrivalTime;

            currentTime = p.completionTime;
            completed[idx] = true;
            completedCount++;
        }
    }
}
