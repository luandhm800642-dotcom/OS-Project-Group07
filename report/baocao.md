Mục Lục:
1. Giới thiệu đề tài
2. Mục tiêu và phạm vi
3. Cơ sở lý thuyết
4. Phân tích thiết kế chương trình
5. Mô tả các thuật toán
6. Dữ liệu đầu vào và đầu ra
7. Công thức tính các chỉ số
8. Kiểm thử và kết quả dự kiến
9. Hạn chế và hướng phát triển
10. Kết luận
1. Giới thiệu đề tài :
Bài tập nhóm xây dựng chương trình C++ mô phỏng hai thuật toán lập lịch CPU cơ bản là FCFS và SJF không độc quyền.Chương trình đọc thông tin tiến trình từ file CSV, tính các mốc thời gian, hiển thị bảng kết quả trên màn hình và ghi kết quả ra file CSV.
2. Mục tiêu và phạm vi :
2.1. Mục tiêu
- Tìm hiểu cách hoạt động của hai thuật toán FCFS và SJF không độc quyền.
- Thực hành sử dụng struct, vector, hàm, sắp xếp dữ liệu và đọc/ghi file CSV trong C++.
- Tính các chỉ số cơ bản của tiến trình gồm thời điểm bắt đầu, hoàn thành, thời gian chờ, thời gian quay vòng và thời gian đáp ứng.
- So sánh kết quả của hai thuật toán thông qua bảng dữ liệu.
2.2. Phạm vi
Chương trình hiện tại xử lý dữ liệu tiến trình có các trường PID, Arrival Time và Burst Time. Hai thuật toán được chạy trên hai bản sao dữ liệu riêng để kết quả của thuật toán này không làm thay đổi dữ liệu đầu vào của thuật toán còn lại. Chương trình chạy trên console và sử dụng file CSV làm đầu vào, đầu ra.
Trong phần mã nguồn được cung cấp, chương trình chưa có giao diện đồ họa. Lớp MetricsAndGantt có các hàm hỗ trợ tính chỉ số, dựng biểu đồ Gantt, hiển thị bảng và xuất CSV, nhưng main.cpp hiện chưa gọi các hàm này.
3. Cơ sở lý thuyết :
3.1. Tiến trình
Tiến trình là một chương trình đang được thực thi. Trong mô phỏng, mỗi tiến trình được mô tả bằng mã tiến trình, thời điểm đến và thời gian CPU cần sử dụng.
- PID: mã định danh của tiến trình.
- Arrival Time (AT): thời điểm tiến trình đi vào hàng đợi sẵn sàng.
- Burst Time (BT): tổng thời gian CPU mà tiến trình cần để chạy.
3.2. FCFS
FCFS là thuật toán phục vụ theo thứ tự đến trước. Tiến trình đến sớm hơn được thực hiện trước. Đây là thuật toán đơn giản, dễ triển khai; tuy nhiên, một tiến trình có thời gian chạy dài ở đầu hàng đợi có thể làm các tiến trình phía sau phải chờ lâu.
3.3. SJF không độc quyền
SJF không độc quyền chọn tiến trình có Burst Time nhỏ nhất trong số các tiến trình đã đến và đang chờ. Khi một tiến trình bắt đầu chạy, tiến trình đó tiếp tục cho đến khi hoàn thành; thuật toán không ngắt tiến trình giữa chừng để chuyển sang tiến trình khác.
Trong code, nếu hai tiến trình có cùng Burst Time, chương trình ưu tiên tiến trình có Arrival Time nhỏ hơn. Nếu Arrival Time cũng bằng nhau, chương trình ưu tiên PID nhỏ hơn.
3.4. Các chỉ số đánh giá
- Start Time (ST): thời điểm tiến trình bắt đầu chạy.
- Completion Time (CT): thời điểm tiến trình chạy xong.
- Turnaround Time (TAT): tổng thời gian từ lúc đến hệ thống đến khi hoàn thành.
- Waiting Time (WT): thời gian tiến trình phải chờ trong hàng đợi.
- Response Time (RT): thời gian từ lúc đến hệ thống đến khi được chạy lần đầu.
4. Phân tích thiết kế chương trình :
4.1. Cấu trúc các file
- main.cpp: Điều phối chương trình: đọc dữ liệu đầu vào, tạo bản sao cho FCFS và SJF, gọi thuật toán, in bảng kết quả và ghi kết quả ra CSV.
- Process.h: Khai báo struct Process để lưu dữ liệu và các chỉ số thời gian của một tiến trình.
- Scheduler.cpp: Cài đặt hai hàm runFCFS() và runSJF() để lập lịch.
- DataLoader.h: Khai báo các hàm đọc dữ liệu CSV và ghi kết quả CSV.
- DataLoader.cpp: Đọc từng dòng của file input, chuyển dữ liệu thành Process và ghi kết quả từng tiến trình ra file output.
- MetricsAndGantt.h: Khai báo GanttSegment và lớp MetricsAndGantt với các hàm tính chỉ số, dựng biểu đồ Gantt, hiển thị bảng và xuất CSV.
- MetricsAndGantt.cpp: Cài đặt các hàm MetricsAndGantt, gồm tính trung bình WT/TAT/RT và dựng các đoạn Gantt, có hỗ trợ đoạn CPU IDLE.
4.2. Cấu trúc Process
Struct Process trong Process.h gồm tám trường số nguyên:
Pid,arrivalTime,burstTime,startTime,completionTime,turnaroundTime,waitingTime,responseTime
Ba trường đầu được đọc từ file đầu vào. Các trường còn lại được khởi tạo bằng 0 và được cập nhật trong quá trình lập lịch.
4.3. Luồng hoạt động
1.Chương trình đặt đường dẫn file đầu vào là ../input/test_processes.csv và file đầu ra là ../output/result_output.csv.

2.Hàm readInputCSV() đọc dữ liệu. Nếu danh sách rỗng, main.cpp sử dụng ba tiến trình mẫu.
3.Dữ liệu được sao chép thành listFCFS và listSJF.
4.runFCFS() lập lịch trên listFCFS, sau đó chương trình in bảng và ghi kết quả.
5.runSJF() lập lịch trên listSJF, sau đó chương trình in bảng và ghi kết quả.
6.Chương trình kết thúc sau khi hoàn thành hai lượt chạy.
5. Mô tả các thuật toán :
5.1. Thuật toán FCFS
Hàm runFCFS() sắp xếp danh sách theo Arrival Time tăng dần. Nếu hai tiến trình có cùng thời điểm đến, PID nhỏ hơn được xếp trước. Biến currentTime lưu thời điểm CPU hiện tại.
Với mỗi tiến trình, nếu CPU đang ở thời điểm sớm hơn Arrival Time thì currentTime được cập nhật bằng Arrival Time để thể hiện CPU chờ tiến trình đến. Sau đó chương trình xác định Start Time, Completion Time, Turnaround Time, Waiting Time và Response Time. Cuối cùng, currentTime được cập nhật thành Completion Time của tiến trình vừa chạy.
5.2. Thuật toán SJF không độc quyền
Hàm runSJF() tạo mảng completed để đánh dấu tiến trình đã hoàn thành. Mỗi vòng lặp, chương trình duyệt các tiến trình chưa hoàn thành và có Arrival Time không lớn hơn currentTime, sau đó chọn tiến trình có Burst Time nhỏ nhất.
Nếu có nhiều tiến trình cùng Burst Time, chương trình xử lý theo quy tắc ưu tiên Arrival Time nhỏ hơn, rồi đến PID nhỏ hơn. Tiến trình được chọn sẽ chạy đến khi hoàn thành. Nếu chưa có tiến trình nào đến tại currentTime, chương trình tăng currentTime lên một đơn vị và tiếp tục tìm kiếm.
Sau khi chạy xong, tiến trình được đánh dấu completed và số tiến trình hoàn thành tăng lên. Vòng lặp kết thúc khi tất cả tiến trình đã được lập lịch.
5.3. So sánh hai thuật toán
Tiêu chí	SJF ko độc quyền	FCFS
Cách chọn tiến trình	Burst Time nhỏ nhất trong các tiến trình đã đến	Theo Arrival Time
Độ phức tạp triển khai	Phải tìm tiến trình phù hợp qua các vòng lặp	Đơn giản
Ngắt tiến trình	Không ngắt	Không ngắt
Điểm lưu ý	Cần biết Burst Time và có thể khiến tiến trình dài chờ lâu	Có thể làm tiến trình ngắn chờ lâu
6. Dữ liệu đầu vào và đầu ra :
6.1. File đầu vào CSV
Hàm readInputCSV() bỏ qua dòng đầu tiên vì xem đó là dòng tiêu đề. Các dòng tiếp theo được tách theo dấu phẩy và đọc lần lượt ba trường: PID, Arrival Time, Burst Time.
Ví dụ nội dung file input/test_processes.csv:
PID,ArrivalTime,BurstTime
1,0,5
2,1,3
3,2,8
Với dữ liệu trên, chương trình tạo ba tiến trình P1, P2 và P3. Các giá trị Start Time, Completion Time và những chỉ số khác được khởi tạo bằng 0 trước khi chạy thuật toán.
6.2. Xử lý khi không đọc được dữ liệu
Nếu không mở được file đầu vào, readInputCSV() in thông báo lỗi và trả về danh sách rỗng. main.cpp kiểm tra danh sách này; nếu rỗng, chương trình dùng bộ dữ liệu mẫu gồm ba tiến trình có thông tin {1,0,5}, {2,1,3} và {3,2,8}.
6.3. File đầu ra CSV
Hàm writeOutputCSV() mở file ở chế độ nối thêm (ios::app), sau đó ghi tên thuật toán và thông tin của từng tiến trình. Các cột được ghi theo thứ tự:
Algorithm,PID,ArrivalTime,BurstTime,StartTime,CompletionTime,WaitingTime,TurnaroundTime,ResponseTime
7. Công thức tính các chỉ số :
- Completion Time (CT) = Start Time (ST) + Burst Time (BT)
- Turnaround Time (TAT) = Completion Time (CT) − Arrival Time (AT)
- Waiting Time (WT) = Turnaround Time (TAT) − Burst Time (BT)
- Response Time (RT) = Start Time (ST) − Arrival Time (AT)
- WT trung bình = Tổng WT của các tiến trình / Số tiến trình
- TAT trung bình = Tổng TAT của các tiến trình / Số tiến trình
- RT trung bình = Tổng RT của các tiến trình / Số tiến trình
8. Kiểm thử và kết quả dự kiến :
8.1. Bộ dữ liệu mẫu
PID	Arrival TIme	Burst Time
P1	0	5
P2	1	3
P3	2	8

8.2. Kết quả FCFS
FCFS chạy P1 trước vì P1 đến tại thời điểm 0, tiếp theo là P2 và P3. P1 chạy từ 0 đến 5; P2 chạy từ 5 đến 8; P3 chạy từ 8 đến 16.
PID	ST	CT	TAT	WT	RT
P1	0	5	5	0	0
P2	5	8	7	4	4
P3	8	16	14	6	6
WT trung bình = (0 + 4 + 6) / 3 = 3,33. TAT trung bình = (5 + 7 + 14) / 3 = 8,67. RT trung bình = (0 + 4 + 6) / 3 = 3,33.
8.3. Kết quả SJF không độc quyền
Tại thời điểm 0 chỉ có P1 đã đến nên P1 chạy từ 0 đến 5. Tại thời điểm 5, P2 và P3 đều đã đến; P2 có Burst Time 3 nhỏ hơn P3 là 8 nên P2 chạy tiếp, sau đó đến P3.

PID	ST	CT	TAT	WT	RT
P1	0	5	5	0	0
P2	5	8	7	4	4
P3	8	16	14	6	6
Với bộ dữ liệu mẫu này, thứ tự chạy của FCFS và SJF trùng nhau nên các chỉ số cũng giống nhau. Điều đó không có nghĩa hai thuật toán luôn cho cùng kết quả; cần thử thêm bộ dữ liệu khác để so sánh.
8.4. Các trường hợp nên kiểm thử
- File CSV có dữ liệu hợp lệ và nhiều tiến trình đến ở các thời điểm khác nhau.
- Hai hoặc nhiều tiến trình có cùng Arrival Time.
- Hai tiến trình có cùng Burst Time để kiểm tra quy tắc ưu tiên.
- Có khoảng thời gian CPU rảnh trước khi tiến trình đầu tiên đến.
- File đầu vào không tồn tại hoặc không có dữ liệu.
- File CSV chứa giá trị không phải số, thiếu cột hoặc dòng trống.
- Không thể mở file đầu ra để ghi.
9. Hạn chế và hướng phát triển
9.1. Hạn chế hiện tại
- Chương trình chạy bằng console, chưa có giao diện đồ họa.
- Việc đọc CSV dựa trên dấu phẩy và chưa kiểm tra đầy đủ định dạng dữ liệu.
- Nếu file đầu vào không đọc được, chương trình tự chuyển sang dữ liệu mẫu nên người dùng có thể không nhận ra dữ liệu thật chưa được sử dụng.
- writeOutputCSV() nối thêm dữ liệu và không ghi tiêu đề; chạy nhiều lần có thể làm file đầu ra chứa nhiều lượt kết quả.
- main.cpp hiển thị WT trung bình và TAT trung bình nhưng chưa hiển thị RT trung bình.
- Các hàm MetricsAndGantt đã được cài đặt riêng nhưng chưa được tích hợp vào luồng chạy chính.
9.2. Hướng phát triển
- Bổ sung kiểm tra file CSV: số cột, dữ liệu số, giá trị âm, dòng trống và thông báo lỗi theo dòng.
- Tách file kết quả của từng thuật toán hoặc ghi tiêu đề rõ ràng và quản lý việc ghi nối thêm.
- Tích hợp MetricsAndGantt vào main.cpp để hiển thị RT, các chỉ số trung bình và biểu đồ Gantt.
- Bổ sung nhiều bộ dữ liệu kiểm thử để so sánh FCFS và SJF.
- Có thể phát triển giao diện trực quan để người dùng nhập dữ liệu, chọn thuật toán và xem biểu đồ Gantt.
- Bổ sung các thuật toán lập lịch khác như Round Robin hoặc Priority Scheduling nếu phạm vi môn học cho phép.
10. Kết luận
Thông qua bài tập, nhóm xây dựng được chương trình C++ mô phỏng hai thuật toán lập lịch CPU FCFS và SJF không độc quyền. Chương trình có khả năng đọc dữ liệu tiến trình từ CSV, tính các mốc thời gian cần thiết, hiển thị kết quả trên console và ghi dữ liệu ra file CSV.
FCFS có ưu điểm đơn giản và thực hiện theo thứ tự đến, trong khi SJF ưu tiên tiến trình có thời gian chạy ngắn trong số các tiến trình đã sẵn sàng. Mỗi thuật toán có cách lựa chọn khác nhau nên kết quả có thể khác nhau tùy bộ dữ liệu. Qua quá trình thực hiện, nhóm có thể hiểu rõ hơn về cơ chế lập lịch CPU, cách tính các chỉ số đánh giá và cách tổ chức chương trình C++ thành nhiều file.

