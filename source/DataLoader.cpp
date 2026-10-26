#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Process.h"
#include "DataLoader"
using namespace std;

vector<Process> readInputCSV(const string& filename) {
    vector<Process> processes;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Khong the mo file input: " << filename << "\n";
        return processes;
    }

    string line;
    
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        Process p;

        getline(ss, segment, ',');
        p.pid = stoi(segment);

        getline(ss, segment, ',');
        p.arrivalTime = stoi(segment);
      
        getline(ss, segment, ',');
        p.burstTime = stoi(segment);

        p.startTime = 0;
        p.completionTime = 0;
        p.turnaroundTime = 0;
        p.waitingTime = 0;
        p.responseTime = 0;

        processes.push_back(p);
    }

    file.close();
    return processes;
}

void writeOutputCSV(const string& filename, const string& algoName, const vector<Process>& processes) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Khong the mo file output de ghi ket qua!\n";
        return;
    }

    for (const auto& p : processes) {
        file << algoName << ","
             << p.pid << ","
             << p.arrivalTime << ","
             << p.burstTime << ","
             << p.startTime << ","
             << p.completionTime << ","
             << p.waitingTime << ","
             << p.turnaroundTime << ","
             << p.responseTime << "\n";
    }

    file.close();
}
