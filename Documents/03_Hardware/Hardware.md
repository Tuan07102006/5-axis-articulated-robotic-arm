# 03. THIẾT KẾ PHẦN CỨNG (HARDWARE)

## 1. Tổng quan hệ thống

Hệ thống cánh tay robot 5 trục sử dụng vi điều khiển STM32F411 làm bộ điều khiển trung tâm, kết hợp với các động cơ servo MG90S và MG905 để thực hiện chuyển động tại các khớp.

Phần cứng được thiết kế gồm ba thành phần chính: bộ điều khiển STM32, hệ thống động cơ servo và hệ thống cung cấp nguồn điện.

## 2. Bộ điều khiển trung tâm

Vi điều khiển STM32F411 đảm nhiệm việc tiếp nhận lệnh, xử lý dữ liệu, tính toán chuyển động và tạo tín hiệu PWM điều khiển các động cơ servo.

Mỗi động cơ được điều khiển thông qua một kênh tín hiệu PWM, cho phép điều chỉnh góc quay và phối hợp chuyển động giữa các khớp robot.

## 3. Hệ thống động cơ

Hệ thống sử dụng các động cơ servo MG90S và MG905 làm cơ cấu chấp hành.

Các động cơ nhận tín hiệu PWM từ STM32 và thực hiện chuyển động đến góc đặt tương ứng.

Các servo được cấp nguồn riêng với bộ điều khiển nhằm hạn chế ảnh hưởng của dòng điện lớn và hiện tượng sụt áp trong quá trình hoạt động.

## 4. Hệ thống cung cấp nguồn

Nguồn điện chính của hệ thống sử dụng pin LiPo 3S, có điện áp danh định 11,1V và điện áp tối đa 12,6V khi sạc đầy.

Nguồn điện được phân phối qua hai bộ chuyển đổi DC-DC Buck độc lập.

Bộ Mini Buck thứ nhất hạ điện áp xuống 5V, với dòng đầu ra mục tiêu 3A, dùng để cấp nguồn cho bo điều khiển STM32 thông qua đầu vào nguồn phù hợp.

Bộ Buck thứ hai có công suất quảng cáo 300W/20A, được điều chỉnh điện áp đầu ra xuống 6V để cung cấp cho các động cơ servo. Dòng cấp mục tiêu của nhánh động cơ là 15A, tùy thuộc khả năng thực tế của bộ nguồn và nhu cầu tải.

Hai nhánh nguồn sử dụng GND chung để đảm bảo tín hiệu điều khiển PWM hoạt động ổn định.

## 5. Nguyên lý hoạt động

Khi hệ thống được cấp nguồn, pin 3S cung cấp điện cho hai bộ chuyển đổi Buck.

Mini Buck tạo điện áp 5V cấp cho bo STM32, trong khi Buck công suất lớn tạo điện áp 6V cấp cho các động cơ servo.

STM32 tiếp nhận và xử lý lệnh điều khiển, sau đó phát tín hiệu PWM đến từng servo để điều khiển chuyển động các khớp.

Việc sử dụng hai bộ nguồn riêng giúp giảm ảnh hưởng của tải động cơ đến hoạt động của vi điều khiển.

## 6. Yêu cầu an toàn

Trước khi kết nối thiết bị, cần kiểm tra điện áp đầu ra của từng bộ Buck và đảm bảo đúng cực tính.

Hệ thống cần có biện pháp bảo vệ quá dòng, hạn chế sụt áp và đảm bảo tản nhiệt cho các bộ chuyển đổi nguồn.

Không cấp điện áp 5V trực tiếp vào chân 3,3V của STM32. Điện áp cấp cho servo phải phù hợp với thông số kỹ thuật của động cơ.

Các thông số dòng điện 3A, 15A và công suất 300W cần được xác minh bằng đo kiểm thực tế trước khi vận hành toàn bộ hệ thống.

## 7. Định hướng phát triển

Trong các giai đoạn tiếp theo, hệ thống phần cứng có thể được bổ sung cảm biến phản hồi vị trí, cảm biến đo dòng điện và điện áp nhằm giám sát trạng thái hoạt động.

Thiết kế hướng đến khả năng mở rộng, thuận tiện cho việc bảo trì, thay thế linh kiện và phát triển các thuật toán điều khiển cánh tay robot.