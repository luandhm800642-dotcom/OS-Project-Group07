// ===== FCFS (Nhiem vu 3) =====
#include <vector>
#include <algorithm>
#include "Process.h"
using namespace std;
vector<GanttEntry> fcfs(vector<Process>& procs) {
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {    // Tie-breaking: Arrival nho hon chay truoc, bang nhau thi PID nho hon chay truoc
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    vector<GanttEntry> gantt;
    int time = 0;
    for (auto& p : procs) {
        if (time < p.arrival) {                  // CPU ranh, cho tien trinh den
            gantt.push_back({-1, time, p.arrival});
            time = p.arrival;
        }
        p.start = time;
        time += p.burst;                         // FCFS: chay mot mach den het
        p.completion = time;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        gantt.push_back({p.pid, p.start, p.completion});
    }
    return gantt;
}
// ===== END FCFS =====
