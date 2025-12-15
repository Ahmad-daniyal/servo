#define BLYNK_TEMPLATE_ID "TMPL6gB3c0_RQ"
#define BLYNK_TEMPLATE_NAME "penggerak servo"
#define BLYNK_AUTH_TOKEN "LSPKUZ_qlj_Xe8sNLsDHf2kPmEyuAF6c"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <BlynkSimpleEsp32.h>

// ================= PCA9685 =================
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// ======== KALIBRASI MG995 (WAJIB DISESUAIKAN) ========
#define SERVO_MIN 120   // posisi 0°
#define SERVO_MAX 520   // posisi 180°
#define SERVO_STOP 320  // posisi DIAM (penting!)

int servo1 = 0;
int servo2 = 1;

// ================= WIFI =================
char ssid[] = "LAB_HARDWARE";
char pass[] = "labhardware2024";

// ================= LOGIC =================
bool posisiServo1 = false;
bool posisiServo2 = false;

// ================= FUNGSI SERVO =================
void setServoAngle(int ch, int angle) {
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(ch, 0, pulse);
}

void stopServo(int ch) {
  pwm.setPWM(ch, 0, SERVO_STOP);
}

// ================= BLYNK BUTTON =================

// Tombol V1 → Servo 1
BLYNK_WRITE(V1) {
  if (param.asInt() == 1) {
    posisiServo1 = !posisiServo1;

    if (posisiServo1) {
      setServoAngle(servo1, 180);
    } else {
      setServoAngle(servo1, 0);
    }

    delay(600);
    stopServo(servo1);
  }
}

// Tombol V2 → Servo 2 (arah BERLAWANAN)
BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    posisiServo2 = !posisiServo2;

    if (posisiServo2) {
      setServoAngle(servo2, 0);
    } else {
      setServoAngle(servo2, 180);
    }

    delay(600);
    stopServo(servo2);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  Wire.begin();               // SDA = GPIO21, SCL = GPIO22
  pwm.begin();
  pwm.setPWMFreq(50);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Posisi awal
  setServoAngle(servo1, 0);
  setServoAngle(servo2, 180);
  delay(600);
  stopServo(servo1);
  stopServo(servo2);

  Serial.println("SYSTEM READY");
}

// ================= LOOP =================
void loop() {
  Blynk.run();
}

