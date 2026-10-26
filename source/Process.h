#pragma once
struct Process {
    int pid, arrival, burst;
    int start = 0, completion = 0, waiting = 0, turnaround = 0;
};
struct GanttEntry {
    int pid;        // -1 nghia la CPU idle
    int from, to;
};
