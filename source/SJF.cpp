#include "SJF.h"
#include <vector>

void runSJF(std::vector<Process>& processes) {
    int n = processes.size();
    std::vector<bool> completed(n, false);

    int finished = 0;
    int currentTime = 0;

    while (finished < n) {
        int chosen = -1;

        for (int i = 0; i < n; i++) {
            if (completed[i] ||
                processes[i].arrivalTime > currentTime) {
                continue;
            }

            if (chosen == -1 ||
                processes[i].burstTime < processes[chosen].burstTime ||
                (processes[i].burstTime == processes[chosen].burstTime &&
                    processes[i].arrivalTime < processes[chosen].arrivalTime) ||
                (processes[i].burstTime == processes[chosen].burstTime &&
                    processes[i].arrivalTime == processes[chosen].arrivalTime &&
                    processes[i].pid < processes[chosen].pid)) {
                chosen = i;
            }
        }

        if (chosen == -1) {
            int nextArrival = -1;

            for (int i = 0; i < n; i++) {
                if (!completed[i] &&
                    (nextArrival == -1 ||
                        processes[i].arrivalTime < nextArrival)) {
                    nextArrival = processes[i].arrivalTime;
                }
            }

            currentTime = nextArrival;
            continue;
        }

        Process& p = processes[chosen];

        p.startTime = currentTime;
        p.completionTime = p.startTime + p.burstTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.responseTime = p.startTime - p.arrivalTime;

        currentTime = p.completionTime;
        completed[chosen] = true;
        finished++;
    }
}
