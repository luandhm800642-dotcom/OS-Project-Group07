#include "MetricsAndGantt.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

void MetricsAndGantt::calculateMetrics(std::vector<Process>& processes, 
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

std::vector<GanttSegment> MetricsAndGantt::buildGanttSegments(const std::vector<Process>& scheduledProcesses) {
    std::vector<GanttSegment> segments;
    if (scheduledProcesses.empty()) return segments;

    // Sắp xếp bản sao theo startTime tăng dần để vẽ trục thời gian chuẩn xác
    std::vector<Process> sortedProcs = scheduledProcesses;
    std::sort(sortedProcs.begin(), sortedProcs.end(), [](const Process& a, const Process& b) {
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
        std::string procLabel = "P" + std::to_string(p.pid);
        segments.push_back({procLabel, p.startTime, p.completionTime});
        currentTime = p.completionTime;
    }

    return segments;
}

void MetricsAndGantt::displayResultTable(const std::vector<Process>& processes, 
                                         double avgWT, double avgTAT, double avgRT) {
    std::cout << "\n==================== BANG KET QUA CHI TIET ====================\n";
    std::cout << std::left 
              << std::setw(8)  << "PID"
              << std::setw(10) << "Arrival"
              << std::setw(10) << "Burst"
              << std::setw(10) << "Start"
              << std::setw(12) << "Completion"
              << std::setw(12) << "Turnaround"
              << std::setw(10) << "Waiting"
              << std::setw(10) << "Response" << "\n";
    std::cout << std::string(82, '-') << "\n";

    for (const auto& p : processes) {
        std::string pidStr = "P" + std::to_string(p.pid);
        std::cout << std::left 
                  << std::setw(8)  << pidStr
                  << std::setw(10) << p.arrivalTime
                  << std::setw(10) << p.burstTime
                  << std::setw(10) << p.startTime
                  << std::setw(12) << p.completionTime
                  << std::setw(12) << p.turnaroundTime
                  << std::setw(10) << p.waitingTime
                  << std::setw(10) << p.responseTime << "\n";
    }

    std::cout << std::string(82, '-') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "-> Average Waiting Time (WT)     : " << avgWT << "\n";
    std::cout << "-> Average Turnaround Time (TAT) : " << avgTAT << "\n";
    std::cout << "-> Average Response Time (RT)    : " << avgRT << "\n";
}

void MetricsAndGantt::renderGanttChart(const std::vector<GanttSegment>& ganttSegments) {
    if (ganttSegments.empty()) {
        std::cout << "\n[!] Khong co du lieu de ve bieu do Gantt.\n";
        return;
    }

    std::cout << "\n========================= BIEU DO GANTT =========================\n";

    // Tính độ rộng hiển thị của từng ô dựa trên thời gian chạy
    std::vector<int> widths;
    for (const auto& seg : ganttSegments) {
        int duration = seg.endTime - seg.startTime;
        int width = std::max(6, duration * 2);
        if (static_cast<int>(seg.label.length()) + 2 > width) {
            width = static_cast<int>(seg.label.length()) + 2;
        }
        widths.push_back(width);
    }

    // 1. Vẽ viền trên
    std::cout << " ";
    for (int w : widths) {
        std::cout << std::string(w, '-') << " ";
    }
    std::cout << "\n|";

    // 2. In nhãn tiến trình căn giữa trong từng ô
    for (size_t i = 0; i < ganttSegments.size(); ++i) {
        int w = widths[i];
        std::string label = ganttSegments[i].label;
        int padLeft = (w - static_cast<int>(label.length())) / 2;
        int padRight = w - static_cast<int>(label.length()) - padLeft;
        std::cout << std::string(padLeft, ' ') << label << std::string(padRight, ' ') << "|";
    }
    std::cout << "\n ";

    // 3. Vẽ viền dưới
    for (int w : widths) {
        std::cout << std::string(w, '-') << " ";
    }
    std::cout << "\n";

    // 4. In dòng thời gian (Timeline) thẳng hàng với vách ngăn '|'
    std::cout << ganttSegments[0].startTime;
    for (size_t i = 0; i < ganttSegments.size(); ++i) {
        std::string prevTimeStr = std::to_string(ganttSegments[i].startTime);
        std::string nextTimeStr = std::to_string(ganttSegments[i].endTime);
        
        int spaces = widths[i] + 1 - static_cast<int>(prevTimeStr.length());
        if (spaces < 1) spaces = 1;
        std::cout << std::string(spaces, ' ') << nextTimeStr;
    }
    std::cout << "\n=================================================================\n";
}

bool MetricsAndGantt::exportToCSV(const std::string& filename, 
                                  const std::vector<Process>& processes, 
                                  double avgWT, double avgTAT, double avgRT) {
    std::ofstream file(filename);
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
    file << std::fixed << std::setprecision(2);
    file << "Average,,,,," << avgTAT << "," << avgWT << "," << avgRT << "\n";

    file.close();
    return true;
}
