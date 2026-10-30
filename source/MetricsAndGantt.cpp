#include "MetricsAndGantt.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

void MetricsAndGantt::calculateMetrics(vector<Process>& processes,
                                       double& avgWT, double& avgTAT, double& avgRT) {
    if (processes.empty()) {
        avgWT = avgTAT = avgRT = 0.0;
        return;
    }

    long long totalWT = 0, totalTAT = 0, totalRT = 0;

    for (auto& p : processes) {
        // Công thức chuẩn Hệ điều hành
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime    = p.turnaroundTime - p.burstTime;
        p.responseTime   = p.startTime - p.arrivalTime;

        totalWT  += p.waitingTime;
        totalTAT += p.turnaroundTime;
        totalRT  += p.responseTime;
    }

    int n = static_cast<int>(processes.size());
    avgWT  = static_cast<double>(totalWT) / n;
    avgTAT = static_cast<double>(totalTAT) / n;
    avgRT  = static_cast<double>(totalRT) / n;
}

vector<GanttSegment> MetricsAndGantt::buildGanttSegments(const vector<Process>& scheduledProcesses) {
    vector<GanttSegment> segments;
    if (scheduledProcesses.empty()) return segments;

    // Sắp xếp bản sao theo startTime tăng dần để vẽ trục thời gian chuẩn xác
    vector<Process> sortedProcs = scheduledProcesses;
    sort(sortedProcs.begin(), sortedProcs.end(), [](const Process& a, const Process& b) {
        if (a.startTime != b.startTime)
            return a.startTime < b.startTime;
        return a.pid < b.pid;
    });

    int currentTime = 0;
    for (const auto& p : sortedProcs) {
        // Nếu thời điểm tiến trình bắt đầu lớn hơn mốc hiện tại -> CPU đang rảnh (IDLE)
        if (p.startTime > currentTime) {
            segments.push_back({"IDLE", currentTime, p.startTime});
        }
        // Chuyển pid kiểu int thành chuỗi "P" + pid (VD: P1, P2)
        string procLabel = "P" + to_string(p.pid);
        segments.push_back({procLabel, p.startTime, p.completionTime});
        currentTime = p.completionTime;
    }

    return segments;
}

void MetricsAndGantt::displayResultTable(const vector<Process>& processes,
                                         double avgWT, double avgTAT, double avgRT) {
    cout << "\n==================== BANG KET QUA CHI TIET ====================\n";
    cout << left
         << setw(8)  << "PID"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(10) << "Start"
         << setw(12) << "Completion"
         << setw(12) << "Turnaround"
         << setw(10) << "Waiting"
         << setw(10) << "Response" << "\n";
    cout << string(82, '-') << "\n";

    for (const auto& p : processes) {
        string pidStr = "P" + to_string(p.pid);
        cout << left
             << setw(8)  << pidStr
             << setw(10) << p.arrivalTime
             << setw(10) << p.burstTime
             << setw(10) << p.startTime
             << setw(12) << p.completionTime
             << setw(12) << p.turnaroundTime
             << setw(10) << p.waitingTime
             << setw(10) << p.responseTime << "\n";
    }

    cout << string(82, '-') << "\n";
    cout << fixed << setprecision(2);
    cout << "-> Average Waiting Time (WT)     : " << avgWT << "\n";
    cout << "-> Average Turnaround Time (TAT) : " << avgTAT << "\n";
    cout << "-> Average Response Time (RT)    : " << avgRT << "\n";
}

void MetricsAndGantt::renderGanttChart(const vector<GanttSegment>& ganttSegments) {
    if (ganttSegments.empty()) {
        cout << "\n[!] Khong co du lieu de ve bieu do Gantt.\n";
        return;
    }

    cout << "\n========================= BIEU DO GANTT =========================\n";

    // Tính độ rộng hiển thị của từng ô dựa trên thời gian chạy
    vector<int> widths;
    for (const auto& seg : ganttSegments) {
        int duration = seg.endTime - seg.startTime;
        int width = max(6, duration * 2);
        if (static_cast<int>(seg.label.length()) + 2 > width) {
            width = static_cast<int>(seg.label.length()) + 2;
        }
        widths.push_back(width);
    }

    // 1. Vẽ viền trên
    cout << " ";
    for (int w : widths) {
        cout << string(w, '-') << " ";
    }
    cout << "\n|";

    // 2. In nhãn tiến trình căn giữa trong từng ô
    for (size_t i = 0; i < ganttSegments.size(); ++i) {
        int w = widths[i];
        string label = ganttSegments[i].label;
        int padLeft = (w - static_cast<int>(label.length())) / 2;
        int padRight = w - static_cast<int>(label.length()) - padLeft;
        cout << string(padLeft, ' ') << label << string(padRight, ' ') << "|";
    }
    cout << "\n ";

    // 3. Vẽ viền dưới
    for (int w : widths) {
        cout << string(w, '-') << " ";
    }
    cout << "\n";

    // 4. In dòng thời gian (Timeline) thẳng hàng với vách ngăn '|'
    cout << ganttSegments[0].startTime;
    for (size_t i = 0; i < ganttSegments.size(); ++i) {
        string prevTimeStr = to_string(ganttSegments[i].startTime);
        string nextTimeStr = to_string(ganttSegments[i].endTime);

        int spaces = widths[i] + 1 - static_cast<int>(prevTimeStr.length());
        if (spaces < 1) spaces = 1;
        cout << string(spaces, ' ') << nextTimeStr;
    }
    cout << "\n=================================================================\n";
}

bool MetricsAndGantt::exportToCSV(const string& filename,
                                  const vector<Process>& processes,
                                  double avgWT, double avgTAT, double avgRT) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "PID,ArrivalTime,BurstTime,StartTime,CompletionTime,TurnaroundTime,WaitingTime,ResponseTime\n";
    for (const auto& p : processes) {
        file << p.pid << ","
             << p.arrivalTime << ","
             << p.burstTime << ","
             << p.startTime << ","
             << p.completionTime << ","
             << p.turnaroundTime << ","
             << p.waitingTime << ","
             << p.responseTime << "\n";
    }
    file << fixed << setprecision(2);
    file << "Average,,,,," << avgTAT << "," << avgWT << "," << avgRT << "\n";

    file.close();
    return true;
}
