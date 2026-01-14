# 🔐 Khóa Cửa Thông Minh Nhận Diện Khuôn Mặt (ESP32 + AI)

## 📌 Giới thiệu
Dự án **Khóa cửa thông minh nhận diện khuôn mặt** là một hệ thống IoT kết hợp AI, cho phép kiểm soát ra vào tự động thông qua công nghệ nhận diện khuôn mặt.  
Hệ thống sử dụng **ESP32** làm bộ điều khiển trung tâm, kết hợp với **Flask (Python + OpenCV)** để xử lý nhận diện khuôn mặt và **Blynk** để điều khiển, giám sát từ xa.

---

## 🎯 Mục tiêu
- Tự động phát hiện người đứng trước cửa
- Nhận diện khuôn mặt để quyết định mở cửa
- Cảnh báo khi khuôn mặt không hợp lệ
- Cho phép mở cửa thủ công từ xa qua ứng dụng
- Hiển thị trạng thái hệ thống trực quan

---

## 🧠 Chức năng chính
- 👤 **Nhận diện khuôn mặt bằng AI**
- 🚪 **Tự động mở/đóng cửa bằng Servo**
- 📏 **Phát hiện người đến gần bằng cảm biến siêu âm**
- 📱 **Điều khiển & giám sát từ xa qua Blynk**
- 🔔 **Cảnh báo bằng LED & Buzzer**
- 📺 **Hiển thị trạng thái trên LCD I2C**
- 🌐 **Giao tiếp ESP32 ↔ Flask bằng HTTP**

---

## 🔧 Phần cứng sử dụng
- ESP32
- Cảm biến siêu âm HC-SR04
- Servo SG90
- LCD I2C 16x2
- LED đỏ, LED xanh
- Buzzer
- Nguồn & dây kết nối

---

## 🛠 Phần mềm & công nghệ
- Arduino IDE
- ESP32 Core
- Flask (Python)
- OpenCV (Face Recognition)
- Blynk IoT Platform
- HTTP REST API
- ArduinoJson

---

## ⚙️ Cấu hình (trước khi chạy)
Trong file `.ino`, thay các giá trị sau bằng thông tin của bạn:

```cpp
#define BLYNK_AUTH_TOKEN  "YOUR_BLYNK_AUTH_TOKEN"
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define FLASK_SERVER "http://YOUR_FLASK_SERVER_IP:5000"

🚀 Quy trình hoạt động
Cảm biến siêu âm phát hiện có người đứng gần cửa
ESP32 gửi yêu cầu bật nhận diện đến Flask Server
Flask xử lý hình ảnh và nhận diện khuôn mặt
Kết quả được gửi về ESP32:
✅ Nhận diện đúng → mở cửa
❌ Không hợp lệ → cảnh báo
Trạng thái được hiển thị trên LCD và ứng dụng Blynk

---

📱 Điều khiển từ xa (Blynk)
Nút trên Blynk cho phép mở cửa thủ công
Hiển thị tên người được nhận diện
Giám sát trạng thái hệ thống theo thời gian thực

---

🔒 Bảo mật
Không public thông tin WiFi, Token Blynk
Giao tiếp nội bộ qua mạng LAN
Có cơ chế chống kích hoạt liên tục (cooldown)

---

📚 Ứng dụng thực tế
Nhà ở thông minh
Phòng trọ / ký túc xá
Văn phòng nhỏ
Đồ án IoT – AI – Nhúng

---

👨‍🎓 Ghi chú
Dự án phục vụ mục đích học tập và nghiên cứu
Có thể mở rộng thêm:
Lưu log ra vào
Nhận diện nhiều người
Kết hợp cloud / database
