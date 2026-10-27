#include <iostream>
#include <vector>
#include <iomanip>
#include "Process.h"
#include "DataLoader.h"
using namespace std;

// Khai báo các hàm từ Scheduler.cpp và DataLoader.cpp
void runFCFS(vector<Process>& processes);
void runSJF(vector<Process>& processes);
vector<Process> readInputCSV(const string& filename);
void writeOutputCSV(const string& filename, const string& algoName, const vector<Process>& processes);

void printResult(const string& algorithmName, const vector<Process>& processes) {
    cout << "\n================ " << algorithmName << " ================\n";
    cout << setw(6) << "PID" 
              << setw(8) << "AT" 
              << setw(8) << "BT" 
              << setw(8) << "ST" 
              << setw(8) << "CT" 
              << setw(8) << "WT" 
              << setw(8) << "TAT" << "\n";

    double totalWT = 0, totalTAT = 0;
    for (const auto& p : processes) {
        cout << setw(6) << p.pid 
                  << setw(8) << p.arrivalTime 
                  << setw(8) << p.burstTime 
                  << setw(8) << p.startTime 
                  << setw(8) << p.completionTime 
                  << setw(8) << p.waitingTime 
                  << setw(8) << p.turnaroundTime << "\n";
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
    }

    cout << "--------------------------------------------------\n";
    cout << "Average Waiting Time: " << totalWT / processes.size() << "\n";
    cout << "Average Turnaround Time: " << totalTAT / processes.size() << "\n";
}

int main() {
    string inputPath = "../input/test_processes.csv";  
    string outputPath = "../output/result_output.csv"; 

    vector<Process> listFCFS = readInputCSV(inputPath);

    if (listFCFS.empty()) {
        cout << "Su dung du lieu mau cung de chay thu...\n";
        listFCFS = { {1, 0, 5, 0, 0, 0, 0, 0}, {2, 1, 3, 0, 0, 0, 0, 0}, {3, 2, 8, 0, 0, 0, 0, 0} };
    }

    vector<Process> listSJF = listFCFS;

    // Chạy FCFS
    runFCFS(listFCFS);
    printResult("FCFS SCHEDULING", listFCFS);
    writeOutputCSV(outputPath, "FCFS", listFCFS);

    // Chạy SJF không độc quyền
    runSJF(listSJF);
    printResult("SJF NON-PREEMPTIVE SCHEDULING", listSJF);
    writeOutputCSV(outputPath, "SJF", listSJF);

    return 0;
}
