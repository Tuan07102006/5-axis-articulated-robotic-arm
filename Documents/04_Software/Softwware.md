
## 1. Tổng quan hệ thống phần mềm

### 1.1. Giới thiệu

Hệ thống phần mềm của dự án **5-Axis Articulated Robotic Arm** được xây dựng trên nền tảng vi điều khiển STM32F411, sử dụng ngôn ngữ lập trình C/C++ kết hợp với thư viện STM32 HAL (Hardware Abstraction Layer).

STM32 đóng vai trò là bộ điều khiển trung tâm, chịu trách nhiệm xử lý các lệnh điều khiển, thực hiện tính toán chuyển động, quản lý trạng thái hệ thống và tạo tín hiệu PWM điều khiển các động cơ servo MG90S và MG905.

Firmware được định hướng phát triển theo kiến trúc module hóa (Modular Architecture), trong đó các chức năng được chia thành những thành phần độc lập, có giao diện lập trình rõ ràng.

Kiến trúc này nhằm đáp ứng các yêu cầu về khả năng mở rộng, bảo trì, kiểm thử và phát triển phần mềm theo nhóm.

### 1.2. Mục tiêu thiết kế

Hệ thống phần mềm cần đáp ứng các mục tiêu:

- Điều khiển độc lập và phối hợp các khớp của cánh tay robot.
- Sử dụng bộ định thời phần cứng của STM32 để tạo tín hiệu PWM.
- Hỗ trợ cấu hình và hiệu chuẩn riêng cho từng động cơ servo.
- Tách biệt thuật toán điều khiển khỏi các thao tác truy cập phần cứng.
- Hỗ trợ mở rộng các thuật toán động học và lập kế hoạch quỹ đạo.
- Có cơ chế kiểm tra và xử lý lệnh không hợp lệ.
- Tổ chức mã nguồn dễ bảo trì và thuận tiện cho phát triển nhóm.
- Hỗ trợ biên dịch, kiểm thử và quản lý phiên bản bằng các công cụ tiêu chuẩn.

---

## 2. Công nghệ và môi trường phát triển

### 2.1. Nền tảng phần cứng

Vi điều khiển STM32F411 sử dụng lõi xử lý ARM Cortex-M4, hỗ trợ các bộ định thời, giao tiếp ngoại vi và khả năng tính toán số thực cần thiết cho các thuật toán điều khiển nhúng.

Các ngoại vi chính được sử dụng hoặc dự kiến sử dụng gồm:

**GPIO:** Quản lý tín hiệu vào/ra và các chức năng điều khiển cơ bản.

**TIM:** Tạo tín hiệu PWM để điều khiển vị trí đặt của các động cơ servo.

**UART:** Phục vụ giao tiếp với máy tính hoặc các thiết bị điều khiển bên ngoài khi được triển khai.

**I2C và SPI:** Dự phòng cho việc kết nối cảm biến và các thiết bị ngoại vi.

**DMA:** Có thể được sử dụng để tối ưu hoạt động truyền nhận dữ liệu và giảm tải xử lý cho CPU.

**SWD:** Hỗ trợ nạp chương trình và gỡ lỗi trực tiếp trên vi điều khiển.

Việc sử dụng các ngoại vi phải dựa trên cấu hình chân thực tế trong STM32CubeMX, tránh xung đột giữa các chức năng.

### 2.2. Ngôn ngữ lập trình

Ngôn ngữ C được sử dụng cho các thành phần điều khiển phần cứng, xử lý ngắt và xây dựng driver.

Ngôn ngữ C++ có thể được sử dụng cho các module thuật toán, cấu trúc dữ liệu và quản lý đối tượng điều khiển.

Các tiêu chuẩn dự kiến áp dụng:

- C11 cho mã nguồn C.
- C++17 cho mã nguồn C++.

Khi sử dụng đồng thời C và C++, cần đảm bảo khả năng liên kết giữa hai ngôn ngữ, đặc biệt tại các giao diện gọi thư viện HAL hoặc module C từ chương trình C++.

### 2.3. Công cụ phát triển

**Visual Studio Code:** Môi trường soạn thảo, quản lý mã nguồn và tích hợp các công cụ phát triển.

**STM32CubeMX:** Cấu hình vi điều khiển, hệ thống xung nhịp, chân GPIO, bộ định thời và ngoại vi.

**STM32 HAL:** Cung cấp các API hỗ trợ truy cập và điều khiển phần cứng.

**ARM GNU Toolchain:** Biên dịch mã nguồn thành chương trình thực thi dành cho STM32.

**CMake:** Quản lý cấu hình dự án, danh sách mã nguồn, thư viện và các tùy chọn biên dịch.

**Ninja:** Thực thi quá trình biên dịch và liên kết chương trình.

**Git:** Theo dõi các thay đổi của mã nguồn.

**GitHub:** Lưu trữ repository, quản lý nhánh và hỗ trợ phát triển nhóm.

---

## 3. Kiến trúc tổng thể phần mềm

### 3.1. Nguyên tắc thiết kế

Firmware được định hướng phân chia thành nhiều tầng chức năng.

Mỗi tầng có trách nhiệm riêng và cung cấp các giao diện để các tầng khác sử dụng.

Việc phân chia này giúp giảm phụ thuộc giữa thuật toán và phần cứng, từ đó đơn giản hóa quá trình kiểm thử, sửa lỗi và nâng cấp hệ thống.

### 3.2. Các tầng phần mềm

**Tầng 1 — Application Layer**

Là tầng quản lý hoạt động tổng thể của cánh tay robot.

Các chức năng bao gồm tiếp nhận yêu cầu chuyển động, điều phối hoạt động giữa các khớp, quản lý trạng thái vận hành và xử lý các lệnh từ bên ngoài.

Tầng Application không trực tiếp thao tác các thanh ghi phần cứng mà thông qua những module chức năng.

**Tầng 2 — Motion & Control Layer**

Phụ trách xử lý các thuật toán điều khiển chuyển động.

Các nhiệm vụ dự kiến bao gồm tính toán góc đặt, nội suy chuyển động, lập kế hoạch quỹ đạo và xử lý mô hình động học.

Tầng này cần được thiết kế độc lập với phần cứng ở mức tối đa để thuận tiện cho việc kiểm thử thuật toán trên máy tính.

**Tầng 3 — Device Management Layer**

Quản lý các thiết bị chấp hành và cảm biến.

Trong giai đoạn đầu, thành phần chính là module Servo, thực hiện chuyển đổi lệnh góc đặt thành thông số PWM phù hợp.

Tầng này cũng đảm nhiệm quản lý giới hạn thiết bị và các thông số hiệu chuẩn.

**Tầng 4 — Hardware Abstraction Layer**

Cung cấp giao diện truy cập phần cứng thông qua STM32 HAL.

Các chức năng bao gồm điều khiển TIM, GPIO, UART và những ngoại vi khác.

**Tầng 5 — Hardware Layer**

Bao gồm vi điều khiển STM32F411, hệ thống cấp nguồn, động cơ servo và các thiết bị ngoại vi.

### 3.3. Luồng điều khiển

Luồng xử lý tổng quát được thiết kế như sau:

```text
User Command / Control Interface
                |
                v
       Application Layer
                |
                v
       Command Validation
                |
                v
      Motion / Kinematics
                |
                v
      Joint Target Manager
                |
                v
         Servo Module
                |
                v
        STM32 HAL / TIM
                |
                v
          PWM Outputs
                |
                v
          Servo Motors
```

Các lệnh điều khiển phải được kiểm tra tính hợp lệ và giới hạn chuyển động trước khi tác động đến cơ cấu chấp hành.

Sơ đồ trên mô tả kiến trúc mục tiêu, không đồng nghĩa tất cả thành phần đã được triển khai.

---

## 4. Cấu trúc mã nguồn dự án

### 4.1. Cấu trúc hiện tại

Dự án được tổ chức trong thư mục `Firmware/`, với các thành phần chính:

```text
Firmware/
|
+-- .settings/
+-- .vscode/
+-- build/
+-- cmake/
+-- Core/
|   +-- Inc/
|   +-- Src/
|
+-- Drivers/
|   +-- CMSIS/
|   +-- STM32F4xx_HAL_Driver/
|
+-- Modules/
|   +-- Servo/
|       +-- Inc/
|       |   +-- servo.h
|       +-- Src/
|           +-- servo.c
|
+-- .clangd
+-- .mxproject
+-- 5-axis_articulated_robotic_arm.ioc
+-- CMakeLists.txt
+-- CMakePresets.json
+-- STM32F411xx_FLASH.ld
+-- startup_stm32f411xe.s
```

### 4.2. Vai trò của các thư mục

**Core/**

Chứa chương trình chính, các hàm khởi tạo ngoại vi, xử lý ngắt và cấu hình hệ thống được tạo bởi STM32CubeMX.

Các tệp tiêu biểu gồm `main.c`, `gpio.c`, `tim.c` và các tệp khai báo tương ứng.

Đây là nơi kết nối các module chức năng với chương trình chính.

**Drivers/**

Chứa thư viện CMSIS và STM32 HAL.

Các thư viện này cung cấp giao diện lập trình cho CPU và các ngoại vi của STM32.

Nên hạn chế sửa trực tiếp thư viện do nhà sản xuất cung cấp để thuận tiện cho việc nâng cấp phiên bản.

**Modules/**

Chứa các module chức năng do nhóm trực tiếp phát triển.

Mỗi module được tổ chức độc lập với phần khai báo trong `Inc/` và phần hiện thực trong `Src/`.

**cmake/**

Chứa các tệp hỗ trợ cấu hình công cụ biên dịch và các thành phần CMake do STM32CubeMX tạo ra.

**build/**

Chứa các tệp trung gian và kết quả biên dịch.

Thư mục này được tạo tự động và không nên đưa lên GitHub.

### 4.3. Cấu trúc module định hướng

Trong quá trình phát triển, thư mục `Modules/` có thể được mở rộng như sau:

```text
Modules/
|
+-- Servo/
|   +-- Inc/
|   +-- Src/
|
+-- Joint/
|   +-- Inc/
|   +-- Src/
|
+-- Motion/
|   +-- Inc/
|   +-- Src/
|
+-- Kinematics/
|   +-- Inc/
|   +-- Src/
|
+-- Trajectory/
|   +-- Inc/
|   +-- Src/
|
+-- Communication/
|   +-- Inc/
|   +-- Src/
|
+-- Safety/
    +-- Inc/
    +-- Src/
```

Đây là kiến trúc đề xuất cho những giai đoạn tiếp theo. Chỉ module Servo đã được xác nhận là có tệp mã nguồn trong cấu trúc dự án hiện tại.

---

## 5. Thiết kế module Servo

### 5.1. Mục đích

Module Servo chịu trách nhiệm tạo và quản lý các tín hiệu điều khiển vị trí động cơ.

Module là lớp trung gian giữa hệ thống điều khiển chuyển động và bộ định thời phần cứng của STM32.

Việc tách module này giúp các thuật toán ở tầng trên không cần trực tiếp xử lý thanh ghi TIM hoặc các chi tiết của thư viện HAL.

### 5.2. Chức năng

Module Servo được thiết kế để đáp ứng những chức năng sau:

- Khởi tạo thông số điều khiển cho từng servo.
- Ánh xạ mỗi servo đến một kênh PWM.
- Thiết lập góc điều khiển.
- Giới hạn góc đặt theo cấu hình cơ khí.
- Chuyển đổi góc đặt thành độ rộng xung.
- Cập nhật giá trị thanh ghi so sánh của bộ định thời.
- Hỗ trợ hiệu chuẩn vị trí trung tính.
- Quản lý các tham số riêng của từng servo.

### 5.3. Nguyên lý tạo tín hiệu PWM

STM32 sử dụng bộ định thời phần cứng để tạo xung PWM điều khiển servo.

Tần số PWM được xác định từ tần số bộ định thời, hệ số chia trước và giá trị chu kỳ:

```text
f_PWM = f_TIM / ((PSC + 1) * (ARR + 1))
```

Trong đó:

- `f_PWM`: Tần số PWM đầu ra.
- `f_TIM`: Tần số đầu vào của bộ định thời.
- `PSC`: Giá trị Prescaler.
- `ARR`: Giá trị Auto Reload Register.

Nhiều servo điều khiển vị trí thông thường sử dụng chu kỳ xung khoảng 20 ms, tương đương 50 Hz. Tuy nhiên, tần số và khoảng độ rộng xung hợp lệ phải được xác định theo từng loại servo cụ thể.

Góc đặt của servo được chuyển đổi thành độ rộng xung theo thông số hiệu chuẩn.

Mô hình ánh xạ tuyến tính tổng quát:

```text
Pulse = PulseMin
      + (Angle - AngleMin)
      * (PulseMax - PulseMin)
      / (AngleMax - AngleMin)
```

Công thức chỉ áp dụng trong phạm vi được hiệu chuẩn và khi đặc tính điều khiển có thể xấp xỉ tuyến tính.

Giá trị độ rộng xung được chuyển thành giá trị thanh ghi CCR tương ứng với cấu hình TIM.

### 5.4. Cấu hình riêng cho từng servo

Mỗi servo cần có một bộ tham số riêng, bao gồm:

- Mã nhận diện servo.
- Bộ định thời và kênh PWM sử dụng.
- Giới hạn góc nhỏ nhất.
- Giới hạn góc lớn nhất.
- Độ rộng xung nhỏ nhất.
- Độ rộng xung lớn nhất.
- Góc trung tính.
- Hướng quay quy ước.
- Trạng thái kích hoạt.

Không nên giả định toàn bộ servo MG90S và MG905 có cùng giới hạn góc hoặc cùng dải độ rộng xung.

### 5.5. Giao diện lập trình

Module Servo cần cung cấp các API đơn giản cho tầng điều khiển bên trên.

Các chức năng dự kiến gồm:

**Khởi tạo:** Thiết lập thông số servo và liên kết với bộ định thời.

**Điều khiển góc:** Nhận giá trị góc đặt và cập nhật PWM.

**Đọc giá trị đặt:** Truy xuất góc mục tiêu đang được quản lý trong phần mềm.

**Hiệu chuẩn:** Thay đổi các tham số liên quan đến vị trí trung tính và giới hạn hoạt động.

**Kiểm tra lỗi:** Phát hiện dữ liệu đầu vào ngoài phạm vi hợp lệ.

Các API cụ thể sẽ được định nghĩa trong `servo.h` và hiện thực trong `servo.c`.

### 5.6. Giới hạn kỹ thuật

Đối với servo thông thường, mạch điều khiển bên trong động cơ tự thực hiện điều khiển vị trí dựa trên tín hiệu PWM.

STM32 không trực tiếp biết chính xác vị trí trục servo nếu không được trang bị tín hiệu phản hồi vị trí độc lập.

Vì vậy, cần phân biệt:

**Góc đặt:** Giá trị STM32 yêu cầu servo thực hiện.

**Góc thực tế:** Vị trí trục servo đo được bằng cảm biến hoặc thiết bị đo.

Trong giai đoạn chưa bổ sung cảm biến phản hồi, hệ thống chỉ có thể quản lý góc đặt và các thông số điều khiển tương ứng.

---

## 6. Thiết kế module quản lý khớp (Joint Management)

### 6.1. Mục đích

Module Joint được định hướng phát triển nhằm quản lý các khớp chuyển động của robot ở mức hệ thống.

Một khớp không chỉ bao gồm servo mà còn có các đặc tính cơ khí như chiều quay, tỉ số truyền, góc gốc và giới hạn chuyển động.

Vì vậy, module Joint cần được thiết kế tách biệt với module Servo.

### 6.2. Chức năng

- Quản lý danh sách các khớp robot.
- Liên kết từng khớp với cơ cấu chấp hành tương ứng.
- Lưu giới hạn góc khớp.
- Chuyển đổi góc khớp sang góc điều khiển servo.
- Quản lý góc đặt hiện tại.
- Kiểm tra trạng thái và tính hợp lệ của lệnh điều khiển.

### 6.3. Nguyên tắc chuyển đổi

Góc khớp trong mô hình động học và góc điều khiển servo có thể khác nhau do vị trí lắp đặt cơ khí.

Quan hệ chuyển đổi cần xét đến:

- Góc lệch lắp đặt.
- Hướng quay của servo.
- Tỉ số truyền của cơ cấu.
- Giới hạn vật lý của khớp.

Các tham số này phải được hiệu chuẩn độc lập cho từng trục.

---

## 7. Thiết kế hệ thống điều khiển chuyển động

### 7.1. Mục đích

Hệ thống điều khiển chuyển động có nhiệm vụ tạo ra các góc đặt phù hợp cho từng khớp theo yêu cầu của chương trình.

Module Motion là tầng trung gian giữa lệnh điều khiển của người dùng và module quản lý khớp.

### 7.2. Điều khiển độc lập

Ở chế độ cơ bản, hệ thống cho phép điều khiển từng khớp đến một giá trị góc mục tiêu.

Lệnh điều khiển cần được kiểm tra để đảm bảo góc mục tiêu không vượt quá giới hạn cho phép.

### 7.3. Điều khiển phối hợp

Đối với các thao tác phức tạp, nhiều khớp cần hoạt động đồng thời.

Module Motion phải có khả năng quản lý tập hợp góc mục tiêu của toàn bộ cánh tay robot và cập nhật chúng theo cùng một mốc thời gian điều khiển.

### 7.4. Nội suy chuyển động

Việc thay đổi trực tiếp góc đặt từ giá trị ban đầu đến giá trị mục tiêu có thể gây chuyển động đột ngột.

Do đó, hệ thống được định hướng sử dụng các phương pháp nội suy để tạo chuỗi góc đặt trung gian.

Các phương pháp nghiên cứu gồm:

- Nội suy tuyến tính.
- Nội suy đa thức bậc ba.
- Nội suy đa thức bậc năm.
- Lập kế hoạch chuyển động theo giới hạn vận tốc và gia tốc.

Cần lưu ý rằng việc làm mượt giá trị góc đặt không đồng nghĩa bảo đảm servo thực tế sẽ chuyển động đúng theo vận tốc hoặc gia tốc đã tính toán.

---

## 8. Thiết kế module động học (Kinematics)

### 8.1. Mục đích

Module Kinematics được định hướng xây dựng để mô hình hóa mối quan hệ giữa chuyển động các khớp và vị trí của cơ cấu chấp hành cuối.

Mô hình động học được xây dựng dựa trên kích thước thực tế, cấu trúc liên kết và hệ tọa độ của cánh tay robot.

### 8.2. Động học thuận

Động học thuận (Forward Kinematics) xác định vị trí và hướng của cơ cấu cuối từ tập hợp góc khớp đã biết.

Vector biến khớp được biểu diễn:

```text
q = [θ1, θ2, θ3, θ4, θ5]^T
```

Phép biến đổi tổng quát:

```text
T05 = T01 * T12 * T23 * T34 * T45
```

Trong đó các ma trận biến đổi mô tả quan hệ giữa những hệ tọa độ liên tiếp.

### 8.3. Động học nghịch

Động học nghịch (Inverse Kinematics) xác định các giá trị góc khớp phù hợp với vị trí và hướng mong muốn của cơ cấu cuối.

Việc giải động học nghịch cần xem xét:

- Khả năng tồn tại nghiệm.
- Giới hạn các khớp.
- Nhiều nghiệm có thể xảy ra.
- Tính liên tục của chuyển động.
- Các cấu hình kỳ dị.
- Khả năng va chạm cơ khí.

Do robot có 5 trục, không phải mọi vị trí và hướng 6 bậc tự do đều có thể thực hiện độc lập.

### 8.4. Kiểm thử thuật toán

Các thuật toán động học cần được kiểm thử bằng các bộ dữ liệu đầu vào đã xác định.

Kết quả có thể được đối chiếu với mô phỏng, mô hình hình học hoặc phép đo thực nghiệm.

Việc kiểm thử trên máy tính nên được thực hiện trước khi tích hợp trực tiếp với cơ cấu chấp hành.

---

## 9. Thiết kế module lập kế hoạch quỹ đạo

### 9.1. Chức năng

Module Trajectory được định hướng phát triển nhằm tạo quỹ đạo góc đặt theo thời gian cho các khớp.

Đầu vào bao gồm vị trí ban đầu, vị trí mục tiêu, thời gian chuyển động và các giới hạn đã được cấu hình.

Đầu ra là chuỗi giá trị góc đặt theo từng thời điểm.

### 9.2. Các phương pháp dự kiến

**Point-to-Point (PTP):**

Chuyển robot từ một cấu hình khớp đến cấu hình khác.

**Linear Joint Interpolation:**

Nội suy tuyến tính giữa các giá trị góc trong không gian khớp.

**Polynomial Interpolation:**

Sử dụng hàm đa thức nhằm tạo quỹ đạo liên tục và có khả năng kiểm soát các điều kiện biên.

**Trajectory Optimization:**

Nghiên cứu lựa chọn quỹ đạo và thời gian chuyển động phù hợp với những ràng buộc kỹ thuật của hệ thống.

### 9.3. Nguyên tắc triển khai

Bộ lập kế hoạch phải cung cấp dữ liệu ở định dạng thống nhất để tầng Motion có thể sử dụng.

Các giới hạn vị trí, vận tốc và gia tốc đặt cần được kiểm tra trước khi phát lệnh.

Những thuật toán yêu cầu tính toán phức tạp cần được đánh giá về thời gian thực thi và bộ nhớ trước khi triển khai trên STM32F411.

---

## 10. Thiết kế hệ thống giao tiếp (Communication)

### 10.1. Mục đích

Module Communication được định hướng phát triển để kết nối bộ điều khiển STM32 với máy tính hoặc các thiết bị điều khiển bên ngoài.

### 10.2. Các chức năng

- Tiếp nhận lệnh điều khiển.
- Phân tích và xác thực dữ liệu đầu vào.
- Truyền phản hồi trạng thái.
- Truyền dữ liệu phục vụ gỡ lỗi.
- Hỗ trợ mở rộng giao thức trong tương lai.

### 10.3. Giao thức truyền thông

UART là phương án giao tiếp có thể được triển khai trong giai đoạn đầu nhờ tính đơn giản và khả năng hỗ trợ trên STM32.

Giao thức điều khiển cần quy định rõ:

- Cấu trúc bản tin.
- Mã lệnh.
- Định danh trục.
- Dữ liệu vị trí mục tiêu.
- Cơ chế kiểm tra lỗi.
- Thông báo phản hồi.
- Xử lý thời gian chờ.

Trong trường hợp sử dụng ngắt hoặc DMA để nhận dữ liệu, cần có bộ đệm và cơ chế xử lý phù hợp để tránh mất dữ liệu hoặc làm gián đoạn luồng điều khiển.

---

## 11. Quản lý trạng thái và an toàn phần mềm

### 11.1. Quản lý trạng thái

Hệ thống được đề xuất quản lý theo các trạng thái:

```text
SYSTEM_INIT
SYSTEM_IDLE
SYSTEM_MOVING
SYSTEM_STOPPING
SYSTEM_FAULT
```

**SYSTEM_INIT:** Khởi tạo thiết bị và xác định cấu hình điều khiển.

**SYSTEM_IDLE:** Hệ thống sẵn sàng tiếp nhận lệnh.

**SYSTEM_MOVING:** Đang thực hiện chuỗi chuyển động.

**SYSTEM_STOPPING:** Đang xử lý yêu cầu dừng có kiểm soát.

**SYSTEM_FAULT:** Hệ thống phát hiện lỗi và chuyển sang chế độ xử lý an toàn.

### 11.2. Xử lý lỗi

Phần mềm cần kiểm tra các điều kiện như:

- Lệnh điều khiển không hợp lệ.
- Góc đặt vượt giới hạn.
- Tham số servo chưa được cấu hình.
- Yêu cầu chuyển động trong trạng thái không cho phép.
- Lỗi giao tiếp.
- Lỗi khởi tạo ngoại vi.

Khi phát hiện lỗi, hệ thống cần từ chối hoặc dừng thực hiện lệnh theo mức độ nguy hiểm.

### 11.3. Dừng khẩn cấp

Chức năng dừng khẩn cấp phải được xem xét ở cả phần mềm và phần cứng.

Không được giả định rằng ngừng phát PWM sẽ khiến servo dừng an toàn trong mọi trường hợp.

Phương án ngắt nguồn động lực bằng phần cứng cần được nghiên cứu và kiểm tra độc lập.

---

## 12. Tổ chức chương trình chính

### 12.1. Giai đoạn khởi tạo

Khi STM32 khởi động, chương trình cần thực hiện lần lượt:

1. Khởi tạo HAL.
2. Cấu hình xung nhịp hệ thống.
3. Khởi tạo GPIO và các ngoại vi.
4. Thiết lập các bộ định thời.
5. Khởi tạo cấu hình servo.
6. Kiểm tra giới hạn và thông số hệ thống.
7. Kích hoạt đầu ra điều khiển theo quy trình an toàn.
8. Chuyển sang trạng thái sẵn sàng.

### 12.2. Giai đoạn vận hành

Trong quá trình vận hành, chương trình thực hiện các công việc:

- Tiếp nhận và xử lý lệnh.
- Cập nhật trạng thái hệ thống.
- Tính toán góc đặt mục tiêu.
- Cập nhật quỹ đạo chuyển động.
- Gửi giá trị điều khiển đến module Servo.
- Kiểm tra lỗi và các điều kiện an toàn.

### 12.3. Nguyên tắc xử lý thời gian

Các chức năng điều khiển nên được thiết kế theo hướng không chặn (Non-blocking).

Không nên sử dụng các khoảng `HAL_Delay()` kéo dài trong luồng điều khiển chính vì có thể khiến hệ thống không phản hồi kịp thời với những lệnh mới.

Các công việc định kỳ có thể được tổ chức bằng bộ định thời hoặc bộ lập lịch đơn giản.

Tần số cập nhật thuật toán điều khiển cần được xác định độc lập với tần số PWM của servo.

Các hàm xử lý ngắt cần ngắn gọn, hạn chế tính toán phức tạp và không thực hiện các thao tác chờ kéo dài.

---

## 13. Hệ thống biên dịch CMake

### 13.1. Tổng quan

Dự án sử dụng CMake để quản lý quá trình biên dịch và Ninja làm công cụ thực thi build.

Các tệp cấu hình chính bao gồm:

- `CMakeLists.txt`
- `CMakePresets.json`
- `cmake/gcc-arm-none-eabi.cmake`
- `cmake/stm32cubemx/CMakeLists.txt`

### 13.2. Quản lý module

Dự án được định hướng sử dụng cơ chế phát hiện tệp nguồn tự động trong thư mục `Modules/`.

Thông qua `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`, CMake có thể kiểm tra sự xuất hiện của các tệp `.c` và `.cpp` mới trong quá trình build.

Các thư mục chứa header cũng được thu thập để khai báo đường dẫn include.

Cách làm này giúp các thành viên bổ sung module mới mà không phải liên tục cập nhật danh sách tệp nguồn thủ công.

Tuy nhiên, các tệp thử nghiệm không thuộc firmware chính thức không nên đặt trong thư mục được quét tự động.

### 13.3. Biên dịch Debug

Từ thư mục `Firmware`, thực hiện:

```powershell
cmake --preset Debug
cmake --build build/Debug
```

Chế độ Debug được sử dụng chủ yếu trong quá trình phát triển và kiểm tra chương trình.

### 13.4. Biên dịch Release

```powershell
cmake --preset Release
cmake --build build/Release
```

Chế độ Release sử dụng cho quá trình xây dựng firmware với các tùy chọn tối ưu tương ứng của dự án.

### 13.5. Kết quả biên dịch

Sau khi biên dịch thành công, hệ thống tạo tệp thực thi ELF và các tệp trung gian.

Tệp ELF chứa mã chương trình, dữ liệu và có thể bao gồm thông tin phục vụ gỡ lỗi.

Dung lượng Flash và RAM sử dụng phải được kiểm tra bằng công cụ phân tích firmware, không xác định trực tiếp từ kích thước toàn bộ tệp ELF.

---

## 14. Kiểm thử và gỡ lỗi phần mềm

### 14.1. Kiểm thử module Servo

Cần thực hiện các thử nghiệm:

- Khởi tạo servo.
- Kiểm tra độ rộng xung PWM.
- Kiểm tra giá trị góc nhỏ nhất và lớn nhất.
- Kiểm tra vị trí trung tính.
- Kiểm tra chiều quay.
- Kiểm tra nhiều servo hoạt động đồng thời.
- Kiểm tra cách xử lý dữ liệu không hợp lệ.

### 14.2. Kiểm thử thuật toán

Các thuật toán không phụ thuộc trực tiếp vào phần cứng cần được kiểm thử riêng bằng bộ dữ liệu mẫu.

Đối với động học thuận và nghịch, cần kiểm tra tính nhất quán giữa góc khớp và vị trí đầu công tác.

Đối với nội suy quỹ đạo, cần kiểm tra điều kiện đầu và cuối, tính liên tục và các giới hạn chuyển động.

### 14.3. Gỡ lỗi trên STM32

Hệ thống hỗ trợ gỡ lỗi thông qua giao diện SWD khi có thiết bị nạp và debug tương thích.

Các hoạt động gỡ lỗi bao gồm:

- Đặt điểm dừng (Breakpoint).
- Theo dõi giá trị biến.
- Kiểm tra trạng thái chương trình.
- Theo dõi thanh ghi ngoại vi.
- Phân tích luồng thực thi.

Có thể bổ sung cơ chế ghi log thông qua UART để phục vụ kiểm tra khi hệ thống đang hoạt động.

### 14.4. Kiểm thử tích hợp

Sau khi từng module được kiểm tra riêng, cần tiến hành kiểm thử tích hợp.

Quy trình cần xác nhận sự tương thích giữa:

- Bộ điều khiển STM32 và hệ thống PWM.
- Module Servo và cơ cấu chấp hành.
- Module Motion và quản lý các khớp.
- Module Kinematics và Motion.
- Module Communication và tầng Application.

Các lỗi được ghi nhận trong tài liệu kiểm thử để theo dõi và khắc phục.

---

## 15. Quản lý phiên bản và phát triển nhóm

### 15.1. Quản lý mã nguồn

Toàn bộ mã nguồn được quản lý bằng Git và lưu trữ trên GitHub.

Nhánh `main` được sử dụng để lưu phiên bản đã được hợp nhất và kiểm tra theo quy trình phát triển nhóm.

Các thành viên không nên trực tiếp đưa những thay đổi chưa được kiểm tra vào nhánh chính.

### 15.2. Phân chia nhiệm vụ

Mỗi thành viên có thể đảm nhiệm một module hoặc nhóm chức năng.

Các nhóm công việc đề xuất:

**Servo:** Điều khiển PWM, cấu hình và hiệu chuẩn servo.

**Motion:** Điều khiển phối hợp các khớp và tạo chuyển động.

**Kinematics:** Xây dựng mô hình động học thuận và nghịch.

**Trajectory:** Nghiên cứu nội suy và lập kế hoạch quỹ đạo.

**Communication:** Phát triển giao tiếp với thiết bị điều khiển bên ngoài.

**Testing:** Kiểm thử tích hợp và đánh giá kết quả.

### 15.3. Quy trình phát triển

Mỗi tính năng được phát triển trên một nhánh riêng.

Sau khi hoàn thành, thành viên cần kiểm tra mã nguồn, bảo đảm dự án biên dịch thành công và tạo Pull Request.

Các thay đổi được kiểm tra trước khi hợp nhất vào `main`.

### 15.4. Quy ước viết mã

- Đặt tên hàm và biến rõ ràng.
- Sử dụng tiền tố module khi cần tránh trùng tên.
- Hạn chế sử dụng biến toàn cục.
- Không tự ý sửa giao diện API của module khác.
- Ghi chú các tham số và đơn vị đo.
- Kiểm tra dữ liệu đầu vào.
- Tách mã xử lý phần cứng khỏi thuật toán khi có thể.
- Không đưa dữ liệu biên dịch hoặc tệp cấu hình riêng của máy lên GitHub nếu không cần thiết.

---

## 16. Lộ trình phát triển phần mềm

### Giai đoạn 1 — Firmware cơ bản

Hoàn thiện cấu hình STM32F411, tạo tín hiệu PWM, phát triển và kiểm thử module Servo.

Kết quả mong muốn là điều khiển độc lập các động cơ servo và quản lý được giới hạn chuyển động cơ bản.

### Giai đoạn 2 — Quản lý và phối hợp khớp

Xây dựng module Joint và Motion.

Cho phép quản lý nhiều trục, chuyển đổi góc khớp sang góc servo và thực hiện các chuỗi chuyển động cơ bản.

### Giai đoạn 3 — Mô hình động học

Xây dựng mô hình hình học và các thuật toán động học thuận, động học nghịch.

Kiểm tra các thuật toán bằng mô phỏng trước khi tích hợp vào firmware.

### Giai đoạn 4 — Lập kế hoạch quỹ đạo

Triển khai nội suy chuyển động và điều phối nhiều khớp theo thời gian.

Đánh giá các giới hạn về tính toán, bộ nhớ và khả năng đáp ứng của servo.

### Giai đoạn 5 — Giao tiếp và tự động hóa

Phát triển giao diện điều khiển, giao thức truyền thông và chức năng lưu các chuỗi chuyển động.

Nghiên cứu tích hợp nhận diện giọng nói, phản hồi vị trí và các tính năng điều khiển thông minh.

---

## 17. Kết luận

Hệ thống phần mềm của dự án 5-Axis Articulated Robotic Arm được định hướng phát triển theo kiến trúc module hóa, sử dụng STM32F411 làm bộ điều khiển trung tâm.

Trong giai đoạn đầu, trọng tâm là xây dựng hệ thống điều khiển servo ổn định, quản lý các khớp và hoàn thiện cấu trúc firmware.

Trên nền tảng đó, các module động học, điều khiển chuyển động, lập kế hoạch quỹ đạo và giao tiếp có thể được phát triển độc lập và tích hợp theo từng giai đoạn.

Kiến trúc này nhằm xây dựng một hệ thống phần mềm có tính tổ chức, khả năng mở rộng và thuận tiện cho nghiên cứu, kiểm thử cũng như hợp tác phát triển trong nhóm.