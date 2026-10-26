#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "Process.h"
using namespace std;
vector<GanttEntry> fcfs(vector<Process>& procs) {
    // Tie-breaking: Arrival nho hon chay truoc, bang nhau thi PID nho hon chay truoc
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    vector<GanttEntry> gantt;
    int time = 0;
    for (size_t i = 0; i < procs.size(); i++) {
        Process& p = procs[i];
        if (time < p.arrival) {                  // CPU ranh, cho tien trinh den
            GanttEntry idle = {-1, time, p.arrival};
            gantt.push_back(idle);
            time = p.arrival;
        }
        p.start = time;
        time += p.burst;                         // FCFS: chay mot mach den het
        p.completion = time;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        GanttEntry run = {p.pid, p.start, p.completion};
        gantt.push_back(run);
    }
    return gantt;
}
