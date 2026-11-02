## 1. Giới thiệu

### 1.1. Mục đích tài liệu
Tài liệu này xác định các yêu cầu kỹ thuật và tiêu chí kiểm thử đối với hệ thống cánh tay robot khớp nối 5 trục. Đây là căn cứ để thiết kế phần cứng, phát triển firmware, nghiên cứu thuật toán, phân công công việc và đánh giá kết quả triển khai.
### 1.2. Phạm vi hệ thống
Cánh tay robot sử dụng các động cơ servo làm cơ cấu chấp hành và vi điều khiển STM32F411 làm trung tâm xử lý. STM32 tiếp nhận lệnh, tính toán thông số chuyển động, quản lý giới hạn khớp và tạo tín hiệu điều khiển servo. Hệ thống được phát triển theo kiến trúc module, hướng đến điều khiển độc lập, phối hợp các trục, lập kế hoạch quỹ đạo và mở rộng giao tiếp.

## 2. Yêu cầu chức năng

### 2.1. Điều khiển servo
| FR-001 | Điều khiển giá trị góc đặt của từng servo độc lập | MUST |
| FR-002 | Tạo tín hiệu PWM tương thích với từng loại servo | MUST |
| FR-003 | Cài đặt vị trí trung tính và hiệu chuẩn từng trục | MUST |
| FR-004 | Giới hạn góc đặt theo giới hạn cơ khí | MUST |
| FR-005 | Gửi lệnh điều khiển phối hợp nhiều trục | MUST |
| FR-006 | Điều chỉnh tốc độ thay đổi góc đặt bằng phần mềm | SHOULD |
| FR-007 | Di chuyển theo các vị trí đặt trước | SHOULD |
| FR-008 | Lưu thông số hiệu chuẩn và giới hạn khớp | SHOULD |

**Tiêu chí nghiệm thu:** Từng servo có thể nhận lệnh riêng; nhiều servo có thể được cập nhật cùng chu kỳ; chương trình từ chối góc đặt không hợp lệ; kiểm tra góc thực tế bằng phép đo khi nghiệm thu.
### 2.2. Quản lý trạng thái hệ thống

| FR-009 | Quản lý các trạng thái khởi tạo, sẵn sàng và chuyển động | MUST |
| FR-010 | Xử lý lệnh sai định dạng hoặc vượt giới hạn | MUST |
| FR-011 | Hỗ trợ yêu cầu dừng chuyển động có kiểm soát | MUST |
| FR-012 | Ghi nhận và thông báo lỗi vận hành | SHOULD |
| FR-013 | Giám sát dòng điện, điện áp hoặc nhiệt độ qua cảm biến bổ sung | FUTURE |
Các trạng thái phần mềm dự kiến: `INIT`, `IDLE`, `MOVING`, `STOPPING`, `FAULT`.
### 2.3. Điều khiển và tái hiện chuyển động
| FR-014 | Điều khiển theo góc khớp | MUST |
| FR-015 | Chuyển động điểm đến điểm (PTP) | SHOULD |
| FR-016 | Nội suy góc đặt theo thời gian | SHOULD |
| FR-017 | Phối hợp các khớp theo quỹ đạo | SHOULD |
| FR-018 | Lưu và tái hiện chuỗi lệnh chuyển động | FUTURE |
| FR-019 | Điều khiển bằng giao diện máy tính hoặc lệnh giọng nói | FUTURE |
Servo điều khiển bằng PWM thường có vòng phản hồi bên trong, nhưng STM32 không tự biết chính xác góc trục thực tế nếu không có tín hiệu phản hồi độc lập. Vì vậy cần phân biệt **góc đặt** với **góc đo được** trong tài liệu và phần mềm.
## 3. Yêu cầu phần cứng

### 3.1. Bộ điều khiển trung tâm
| HW-001 | Vi điều khiển | STM32F411, ARM Cortex-M4 |
| HW-002 | PWM | Số kênh đáp ứng số servo thực tế |
| HW-003 | Bộ định thời | Tạo PWM bằng timer phần cứng |
| HW-004 | Giao tiếp | Có khả năng mở rộng UART, I2C hoặc SPI theo phân bổ chân |
| HW-005 | Lập trình/gỡ lỗi | SWD |
| HW-006 | Tài nguyên | Flash, RAM và chân I/O đáp ứng cấu hình chip cụ thể |
Việc chọn chân, timer, kênh PWM và tần số cập nhật phải được xác nhận trong tệp STM32CubeMX `.ioc`.

### 3.2. Động cơ servo
Hệ thống sử dụng servo **MG90S** và servo đang được ghi nhận là **MG905**. Trước khi chốt bảng thiết bị, cần kiểm tra lại nhãn động cơ và datasheet vì mã này có thể bị nhầm với MG995.

| HW-007 | Điện áp hoạt động | Đối chiếu datasheet và nhãn thiết bị |
| HW-008 | Chu kỳ và độ rộng xung điều khiển | Datasheet và đo PWM |
| HW-009 | Dải góc làm việc an toàn | Hiệu chuẩn thực nghiệm |
| HW-010 | Mô-men và tải tại từng khớp | Tính toán cơ khí, thử có tải |
| HW-011 | Dòng không tải, có tải và gần kẹt trục | Đo thực tế có giới hạn dòng và biện pháp bảo vệ |
| HW-012 | Độ lặp lại góc | Đo nhiều chu kỳ |

### 3.3. Nguồn điện
| HW-013 | Nguồn logic STM32 ổn định | MUST |
| HW-014 | Nguồn servo phù hợp điện áp và dòng tải | MUST |
| HW-015 | GND chung giữa mạch điều khiển và tín hiệu servo | MUST |
| HW-016 | Bảo vệ quá dòng và ngắt nguồn động lực khi khẩn cấp | MUST |
| HW-017 | Giảm sụt áp và nhiễu do servo khởi động/đảo chiều | MUST |
| HW-018 | Đo dòng và điện áp nguồn | SHOULD |

Không cấp dòng nuôi nhiều servo trực tiếp từ chân nguồn STM32. Bộ Buck phải được chọn dựa trên dòng tiêu thụ đo được và khả năng tản nhiệt thực tế, không chỉ dựa trên dòng quảng cáo.

### 3.4. Kết cấu cơ khí
| ME-001 | Cấu trúc có 5 trục chuyển động | MUST |
| ME-002 | Liên kết cơ khí chắc chắn, hạn chế rơ | MUST |
| ME-003 | Hạn chế va chạm giữa các khâu trong vùng làm việc | MUST |
| ME-004 | Bố trí dây điện không bị kéo căng khi chuyển động | MUST |
| ME-005 | Thuận tiện tháo lắp, bảo trì, thay servo | SHOULD |
| ME-006 | Tải yêu cầu ở mỗi trục không vượt khả năng thiết kế | MUST |

Cần đo và lưu: chiều dài khâu, khối lượng, tọa độ trục quay, giới hạn góc, tầm với và tải trọng đầu công tác.
## 4. Yêu cầu phần mềm
### 4.1. Công cụ phát triển

| Thành phần | Công nghệ |
|---|---|
| Ngôn ngữ | C11, C++17 nếu cần |
| Nền tảng | STM32 HAL |
| Cấu hình ngoại vi | STM32CubeMX |
| IDE | Visual Studio Code |
| Hệ thống build | CMake, Ninja |
| Compiler | ARM GNU Toolchain |
| Quản lý phiên bản | Git, GitHub |
### 4.2. Kiến trúc firmware

Các thành phần chính được tổ chức theo module, có API rõ ràng và hạn chế phụ thuộc trực tiếp giữa thuật toán với lớp HAL.

```text
Firmware/
├── Core/
├── Drivers/
├── Modules/
│   ├── Servo/          
│   ├── Motion/         
│   ├── Kinematics/     
│   ├── Trajectory/     
│   ├── Safety/         
│   └── Communication/ 
├── cmake/
└── CMakeLists.txt
```

| SW-001 | Mỗi module có giao diện API rõ ràng | MUST |
| SW-002 | Tránh sử dụng biến toàn cục không cần thiết | SHOULD |
| SW-003 | Hạn chế thao tác chờ chặn trong vòng điều khiển | SHOULD |
| SW-004 | Quản lý cấu hình riêng từng servo | MUST |
| SW-005 | Có cơ chế kiểm tra tham số đầu vào | MUST |
| SW-006 | Có quy tắc xử lý và báo lỗi | MUST |
| SW-007 | Dự án có thể build lại từ mã nguồn trên GitHub | MUST |
| SW-008 | Tách mã ứng dụng khỏi mã được STM32CubeMX sinh tự động | SHOULD |

## 5. Yêu cầu thuật toán
### 5.1. Động học robot

Các bước phát triển định hướng:

1. Xác định hệ tọa độ cơ sở và hệ tọa độ gắn với từng khâu.
2. Đo kích thước và xác định chiều dương của các trục.
3. Thiết lập bảng Denavit–Hartenberg (DH) phù hợp với mô hình thực.
4. Xây dựng động học thuận để tính vị trí, hướng đầu công tác từ góc khớp.
5. Nghiên cứu động học nghịch và kiểm tra nghiệm trong giới hạn khớp.
6. Kiểm chứng kết quả bằng mô phỏng hoặc đo đạc.
### 5.2. Lập kế hoạch quỹ đạo

| ID | Yêu cầu | Ưu tiên |
|---|---|---|
| ALG-001 | Động học thuận | SHOULD |
| ALG-002 | Động học nghịch | SHOULD |
| ALG-003 | Giới hạn nghiệm theo vùng chuyển động | SHOULD |
| ALG-004 | Nội suy vị trí đặt giữa các điểm | SHOULD |
| ALG-005 | Kiểm tra giới hạn vận tốc, gia tốc lệnh đặt | SHOULD |
| ALG-006 | Tìm hiểu nội suy đa thức bậc ba/bậc năm | FUTURE |
| ALG-007 | Tối ưu thời gian chuyển động theo giới hạn cơ cấu chấp hành | FUTURE |

Khi lập kế hoạch quỹ đạo, phải phân biệt quỹ đạo **lệnh đặt** với chuyển động **thực tế** do giới hạn cơ khí, tốc độ và tải của servo.
## 6. Yêu cầu phi chức năng

| NFR-001 | Độ ổn định | Firmware không treo khi nhận dữ liệu sai |
| NFR-002 | Khả năng bảo trì | Module có mô tả API và quy ước mã nguồn |
| NFR-003 | Khả năng mở rộng | Bổ sung module mà không phá vỡ cấu trúc đang có |
| NFR-004 | Khả năng kiểm thử | Thuật toán có thể thử riêng khi phù hợp |
| NFR-005 | Độ chính xác | Có phép đo và tiêu chí sai số rõ ràng |
| NFR-006 | Hiệu năng | Đo thời gian xử lý và chu kỳ cập nhật |
| NFR-007 | Khả năng tái lập | Thành viên khác clone và build được dự án |

Giá trị định lượng cho độ chính xác, độ trễ và chu kỳ cập nhật sẽ được chốt sau khi thử nghiệm.
## 7. Yêu cầu an toàn

| SAF-001 | Kiểm tra cực tính, điện áp trước khi cấp điện | MUST |
| SAF-002 | Có bảo vệ quá dòng phù hợp | MUST |
| SAF-003 | Có cách ngắt nguồn động lực servo khi khẩn cấp | MUST |
| SAF-004 | Giới hạn góc đặt để giảm nguy cơ va chạm và kẹt trục | MUST |
| SAF-005 | Không tạo chuyển động đột ngột ngoài dự kiến khi khởi động | MUST |
| SAF-006 | Có quy trình đưa robot về trạng thái an toàn trước khi sửa cơ khí | MUST |
| SAF-007 | Kiểm tra nhiệt độ, dòng và tải trong thử nghiệm | SHOULD |

**Quan trọng:** Ngừng gửi PWM không được xem là chức năng dừng khẩn cấp bảo đảm an toàn. Các phép thử kẹt trục phải được thực hiện có kiểm soát và có khả năng ngắt nguồn ngay.
## 8. Yêu cầu giao tiếp và mở rộng

| COM-001 | Nhận lệnh điều khiển từ thiết bị ngoài | SHOULD |
| COM-002 | Phản hồi kết quả xử lý lệnh | SHOULD |
| COM-003 | Kiểm tra dữ liệu nhận trước khi thực thi | MUST khi có giao tiếp ngoài |
| COM-004 | Hỗ trợ log/debug qua giao tiếp nối tiếp | SHOULD |
| COM-005 | Giao diện điều khiển trên máy tính | FUTURE |
| COM-006 | Ghi và phát lại chương trình chuyển động | FUTURE |
| COM-007 | Nhận diện giọng nói qua thiết bị xử lý phù hợp | FUTURE |

Giao thức, định dạng gói tin và mức điện áp giao tiếp sẽ được quy định trong tài liệu thiết kế phần mềm.
## 9. Kiểm thử và nghiệm thu
### 9.1. Kiểm thử phần cứng

- [ ] Xác nhận chính xác mã servo và số lượng từng loại.
- [ ] Kiểm tra điện áp cấp cho STM32 và servo.
- [ ] Đo dòng tiêu thụ khi không tải và khi vận hành có tải.
- [ ] Kiểm tra PWM bằng thiết bị đo thích hợp.
- [ ] Hiệu chuẩn góc trung tính và giới hạn từng khớp.
- [ ] Kiểm tra nguồn, nhiệt độ và tình trạng cơ khí trong vận hành.
### 9.2. Kiểm thử firmware

- [ ] Build thành công cấu hình Debug.
- [ ] Kiểm tra khởi tạo timer và PWM.
- [ ] Kiểm tra điều khiển từng servo.
- [ ] Kiểm tra điều khiển đồng thời nhiều servo.
- [ ] Kiểm tra lệnh sai hoặc vượt giới hạn góc.
- [ ] Kiểm tra quá trình dừng và xử lý lỗi.
### 9.3. Kiểm thử thuật toán

- [ ] Đối chiếu động học thuận với mô hình hoặc phép đo.
- [ ] Kiểm tra nghiệm động học nghịch trong vùng khả thi.
- [ ] Kiểm tra tính liên tục của quỹ đạo lệnh đặt.
- [ ] Đo sai số và độ lặp lại khi chạy cùng một chuỗi lệnh.
### 9.4. Điều kiện nghiệm thu phiên bản cơ bản

Phiên bản cơ bản được nghiệm thu khi firmware biên dịch được, các servo đáp ứng lệnh đặt trong giới hạn đã hiệu chuẩn, hệ thống thực hiện được chuỗi chuyển động cơ bản và có kết quả thử nghiệm nguồn, an toàn, chuyển động được lưu ở `Documents/06_Testing/`.
## 10. Danh sách thông số cần xác minh (TBD)

| Số lượng servo MG90S và MG905 | Chờ xác nhận |
| Mã chính xác của servo MG905/MG995 | Chờ kiểm tra nhãn |
| Chiều dài và khối lượng từng khâu | Chờ đo |
| Giới hạn góc của từng khớp | Chờ hiệu chuẩn |
| Tải trọng đầu công tác | Chờ tính toán và thử nghiệm |
| Dòng điện cực đại và công suất nguồn Buck | Chờ đo |
| Sai số và độ lặp lại vị trí | Chờ thử nghiệm |
| Chu kỳ cập nhật điều khiển | Chờ lựa chọn và đo |
| Chân PWM/timer tương ứng từng servo | Chờ đối chiếu `.ioc` |
| Giao thức lệnh điều khiển | Chưa chốt |

**Nguyên tắc:** Không điền số liệu giả định vào bảng thông số chính thức khi chưa đo, kiểm chứng hoặc đối chiếu tài liệu kỹ thuật.
## 11. Lộ trình phát triển

**Giai đoạn 1 – Điều khiển cơ bản:** hoàn thiện nguồn, đấu nối, tạo PWM, hiệu chuẩn servo, thiết lập giới hạn góc và kiểm tra từng trục.

**Giai đoạn 2 – Phối hợp chuyển động:** quản lý nhiều trục, thực hiện chuỗi lệnh, điều khiển điểm đến điểm và dừng có kiểm soát.

**Giai đoạn 3 – Thuật toán:** xây dựng mô hình động học, kiểm chứng động học thuận/nghịch và nội suy quỹ đạo.

**Giai đoạn 4 – Mở rộng:** giao diện điều khiển, ghi/tái hiện chuyển động, tích hợp cảm biến và nghiên cứu nhận diện giọng nói.