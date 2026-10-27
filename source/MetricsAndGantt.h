#ifndef METRICS_AND_GANTT_H
#define METRICS_AND_GANTT_H

#include <vector>
#include <string>
#include "Process.h"

// Cấu trúc lưu trữ từng đoạn trên biểu đồ Gantt (hỗ trợ cả khoảng trống CPU Idle)
struct GanttSegment {
    std::string label; // Hiển thị "P1", "P2"... hoặc "IDLE"
    int startTime;     // Thời điểm bắt đầu đoạn
    int endTime;       // Thời điểm kết thúc đoạn
};

class MetricsAndGantt {
public:
    // 1. Tính toán TAT, WT, RT cho từng tiến trình và tính trung bình
    static void calculateMetrics(std::vector<Process>& processes, 
                                 double& avgWT, double& avgTAT, double& avgRT);

    // 2. Tạo danh sách các đoạn Gantt từ danh sách tiến trình đã lập lịch
    static std::vector<GanttSegment> buildGanttSegments(const std::vector<Process>& scheduledProcesses);

    // 3. Hiển thị bảng kết quả chi tiết ra màn hình Console
    static void displayResultTable(const std::vector<Process>& processes, 
                                   double avgWT, double avgTAT, double avgRT);

    // 4. Vẽ biểu đồ Gantt trực quan trên màn hình Console
    static void renderGanttChart(const std::vector<GanttSegment>& ganttSegments);

    // 5. Xuất bảng kết quả ra file CSV (lưu vào thư mục output)
    static bool exportToCSV(const std::string& filename, 
                            const std::vector<Process>& processes, 
                            double avgWT, double avgTAT, double avgRT);
};

#endif // METRICS_AND_GANTT_H
