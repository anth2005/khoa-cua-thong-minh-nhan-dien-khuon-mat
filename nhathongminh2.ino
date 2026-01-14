#define BLYNK_AUTH_TOKEN  "YOUR_BLYNK_AUTH_TOKEN"
#define BLYNK_TEMPLATE_ID "TMPL65efz2UZE"
#define BLYNK_TEMPLATE_NAME "nhan"

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Flask AI Server
#define FLASK_SERVER "http://YOUR_FLASK_SERVER_IP:5000"
/*************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>

char auth[] = BLYNK_AUTH_TOKEN;

WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin LED, buzzer, servo
const int LED_GREEN = 5;
const int LED_RED   = 4;
const int BUZZER    = 14;
const int SERVO_PIN = 12;

// Ultrasonic sensor
const int TRIG_PIN = 26;
const int ECHO_PIN = 25;

Servo doorServo;
BlynkTimer timer;

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);

  lcd.init();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Door closed by default

  lcd.setCursor(0, 0);
  lcd.print("Dang ket noi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi: OK");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  server.on("/open", HTTP_POST, handleOpen);
  server.on("/distance", HTTP_GET, handleDistance);
  server.on("/fail", HTTP_POST, handleFail);
  server.begin();
}

// ================= DISTANCE =================
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

void handleDistance() {
  float distance = getDistance();
  StaticJsonDocument<100> doc;
  doc["distance"] = distance;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// ================= FACE RESULT =================
void handleOpen() {
  String body = server.arg("plain");
  StaticJsonDocument<200> doc;

  if (deserializeJson(doc, body) == DeserializationError::Ok && doc.containsKey("name")) {
    String name = doc["name"].as<String>();
    Serial.println("Welcome: " + name);

    if (name == "unknown") {
      showMismatch();
    } else {
      showSuccess(name);
    }
  } else {
    showFailure();
  }

  server.send(200, "text/plain", "OK");
}

// ================= UI FUNCTIONS =================
void showSuccess(String name) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Xin chao:");
  lcd.setCursor(0, 1);
  lcd.print(name);

  Blynk.virtualWrite(V7, name);

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  tone(BUZZER, 1000, 200);

  doorServo.write(90);
  delay(3000);
  doorServo.write(0);

  digitalWrite(LED_GREEN, LOW);
  lcd.clear();
}

void showFailure() {
  lcd.clear();
  lcd.print("KHONG XAC DINH");

  digitalWrite(LED_RED, HIGH);
  tone(BUZZER, 500, 500);
  delay(1000);
  noTone(BUZZER);
  digitalWrite(LED_RED, LOW);

  Blynk.virtualWrite(V7, "Unknown");
  lcd.clear();
}

void showMismatch() {
  lcd.clear();
  lcd.print("KHUON MAT SAI!");

  digitalWrite(LED_RED, HIGH);
  tone(BUZZER, 300, 500);
  delay(1000);
  noTone(BUZZER);
  digitalWrite(LED_RED, LOW);

  Blynk.virtualWrite(V7, "Khong khop");
  lcd.clear();
}

// ================= BLYNK =================
BLYNK_WRITE(V8) {
  if (param.asInt() == 1) {
    showSuccess("App");
    Blynk.virtualWrite(V8, 0);
  }
}

// ================= LOOP =================
bool faceRecognitionActive = false;
unsigned long lastTriggerTime = 0;
const unsigned long triggerCooldown = 20000;

void loop() {
  Blynk.run();
  server.handleClient();

  float distance = getDistance();
  unsigned long now = millis();

  if (distance > 0 && distance < 40 && !faceRecognitionActive && now - lastTriggerTime > triggerCooldown) {
    faceRecognitionActive = true;
    lastTriggerTime = now;

    HTTPClient http;
    http.begin(String(FLASK_SERVER) + "/start");
    http.GET();
    http.end();
  }

  else if (distance >= 40 && faceRecognitionActive && now - lastTriggerTime > triggerCooldown) {
    faceRecognitionActive = false;
    lastTriggerTime = now;

    HTTPClient http;
    http.begin(String(FLASK_SERVER) + "/stop");
    http.GET();
    http.end();

    lcd.clear();
  }
}

// ================= FAIL API =================
void handleFail() {
  lcd.clear();
  lcd.print("KHUON MAT SAI!");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  tone(BUZZER, 1000, 500);
  server.send(200, "text/plain", "Fail OK");
}
