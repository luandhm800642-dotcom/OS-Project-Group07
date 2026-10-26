#pragma once              
struct Process {
    int pid, arrival, burst;                              // du lieu vao
    int start, completion, waiting, turnaround;           // ket qua tinh duoc
    Process(int p = 0, int a = 0, int b = 0)
        : pid(p), arrival(a), burst(b),
          start(0), completion(0), waiting(0), turnaround(0) {}
};
struct GanttEntry {  // Mot o trong Gantt chart (pid = -1 nghia la CPU ranh)
    int pid;
    int from, to;
};
