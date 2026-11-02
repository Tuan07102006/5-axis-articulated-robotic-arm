1. Giới thiệu dự án

5-Axis Articulated Robotic Arm là dự án thiết kế và phát triển hệ thống cánh tay robot khớp nối 5 trục, sử dụng vi điều khiển STM32 làm trung tâm xử lý và điều khiển.

Hệ thống sử dụng các động cơ servo MG90S và MG905 làm cơ cấu chấp hành, thực hiện chuyển động tại các khớp của cánh tay robot thông qua tín hiệu điều khiển được tạo bởi vi điều khiển.

Dự án hướng đến việc xây dựng một nền tảng robot có cấu trúc phần cứng và phần mềm theo hướng module hóa, cho phép phát triển, thử nghiệm và tích hợp các thuật toán điều khiển chuyển động, động học robot và các chức năng tự động hóa.

2. Mục tiêu dự án

Mục tiêu tổng quát

Thiết kế và phát triển hệ thống cánh tay robot 5 trục có khả năng điều khiển chuyển động các khớp bằng vi điều khiển STM32, tạo nền tảng cho việc nghiên cứu và triển khai các thuật toán điều khiển robot.

Mục tiêu cụ thể

Xây dựng hệ thống cơ khí cánh tay robot với 5 trục chuyển động.

Sử dụng động cơ servo MG90S và MG905 để thực hiện chuyển động tại các khớp.

Xây dựng hệ thống điều khiển trung tâm dựa trên vi điều khiển STM32.

Phát triển firmware theo kiến trúc module, thuận tiện cho bảo trì và mở rộng.

Thiết kế các chức năng điều khiển vị trí, giới hạn góc và phối hợp chuyển động giữa các khớp.

Triển khai các thuật toán động học thuận, động học nghịch và lập kế hoạch quỹ đạo.

Xây dựng quy trình kiểm thử, đánh giá độ ổn định và độ chính xác của hệ thống.

3. Kiến trúc tổng thể hệ thống

Hệ thống được tổ chức thành các thành phần chính:

3.1. Bộ điều khiển trung tâm (Central Control Unit)

Vi điều khiển STM32 đảm nhiệm chức năng xử lý trung tâm, bao gồm:

Tiếp nhận và xử lý các lệnh điều khiển.

Tính toán thông số chuyển động của các khớp.

Tạo tín hiệu PWM điều khiển động cơ servo.

Quản lý trạng thái vận hành và giới hạn chuyển động.

Phối hợp hoạt động giữa các cơ cấu chấp hành.

3.2. Hệ thống cơ cấu chấp hành 

Hệ thống sử dụng các động cơ servo MG90S và MG905 nhằm tạo chuyển động quay tại các khớp robot.

Các động cơ nhận tín hiệu điều khiển từ STM32 và thực hiện chuyển động đến vị trí được chỉ định trong phạm vi cơ khí cho phép.

3.3. Hệ thống cơ khí 

Cánh tay robot được thiết kế theo cấu trúc khớp nối gồm 5 trục chuyển động. Các bộ phận cơ khí có nhiệm vụ truyền lực, liên kết các khớp và xác định không gian làm việc của robot.

3.4. Hệ thống nguồn 

Hệ thống nguồn cung cấp năng lượng cho bộ điều khiển và các động cơ servo, bảo đảm điện áp phù hợp và khả năng cấp dòng đáp ứng các trạng thái hoạt động của robot.

Nguồn động lực cho servo và nguồn logic điều khiển cần được thiết kế phù hợp nhằm hạn chế ảnh hưởng của nhiễu và sụt áp trong quá trình vận hành.

3.5. Hệ thống phần mềm 

Firmware được phát triển bằng ngôn ngữ C/C++ trên nền tảng STM32, tổ chức thành các module chức năng độc lập để thuận tiện cho phát triển nhóm và mở rộng hệ thống.

4. Nguyên lý hoạt động

Nguyên lý điều khiển tổng quát của hệ thống được mô tả qua các bước:

Hệ thống tiếp nhận lệnh điều khiển hoặc thông số vị trí mong muốn.

Vi điều khiển STM32 xử lý lệnh và xác định giá trị điều khiển tương ứng cho từng khớp.

Bộ điều khiển tạo tín hiệu PWM phù hợp với yêu cầu của từng động cơ servo.

Các động cơ servo thực hiện chuyển động của các khớp.

Hệ thống cập nhật trạng thái điều khiển và tiếp tục thực hiện các lệnh tiếp theo.

Trong các giai đoạn phát triển tiếp theo, hệ thống có thể được bổ sung các chức năng phản hồi vị trí, điều khiển quỹ đạo và giám sát trạng thái vận hành.