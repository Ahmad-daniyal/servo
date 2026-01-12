#define BLYNK_TEMPLATE_ID "TMPL6gB3c0_RQ"
#define BLYNK_TEMPLATE_NAME "penggerak servo"
#define BLYNK_AUTH_TOKEN "LSPKUZ_qlj_Xe8sNLsDHf2kPmEyuAF6c"

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_PWMServoDriver.h>

// ================= WIFI =================
char ssid[] = "LAB_HARDWARE";
char pass[] = "labhardware2024";

// ================= TERMINAL =================
WidgetTerminal terminal(V10);

// ================= PCA9685 ===========  ======
Adafruit_PWMServoDriver pwm(0x40);

// ================= SERVO =================
#define TOTAL_SERVO 16
constexpr uint8_t servo[TOTAL_SERVO] = {
  0,1,2,3,4,5,6,7,
  8,9,10,11,12,13,14,15
};

// ================= CONTINUOUS SERVO =================
#define SERVO_CW    395
#define SERVO_CCW  235
#define SERVO_STOP 315

#define MOVE_TIME_180 1200
#define MOVE_TIME_90   600
#define GAP_TIME       200

// ================= STATUS ARAH =================
bool dir1 = false;
bool dir2 = false;
bool dir3 = false;

// ================= FUNGSI =================
void stopAllServo() {
  for (int i = 0; i < TOTAL_SERVO; i++) {
    pwm.setPWM(servo[i], 0, SERVO_STOP);
  }
}

void moveGroupSequential(int start, int end, int duration, bool direction) {
  int pwmValue = direction ? SERVO_CCW : SERVO_CW;

  for (int i = start; i <= end; i++) {
    pwm.setPWM(servo[i], 0, pwmValue);
    delay(duration);

    pwm.setPWM(servo[i], 0, SERVO_STOP);
    delay(GAP_TIME);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // PCA9685
  pwm.begin();
  pwm.setPWMFreq(50);

  stopAllServo();

  terminal.println("🚀 SYSTEM READY");
  terminal.println("🛑 MODE : CONTINUOUS SERVO SEQUENTIAL");
  terminal.println("⚙️ SERVO : 16 CHANNEL");
  terminal.flush();
}

// ================= LOOP =================
void loop() {
  Blynk.run();
}

// ================= TOMBOL BLYNK =================

// Tombol 1 → Servo 1–4 (±180°)
BLYNK_WRITE(V0) {
  if (param.asInt()) {
    dir1 = !dir1;
    terminal.printf(
      "🔘 Tombol 1 → Servo 1–4 BERURUTAN (%s)\n",
      dir1 ? "CCW" : "CW"
    );
    terminal.flush();

    moveGroupSequential(0, 3, MOVE_TIME_180, dir1);
  }
}

// Tombol 2 → Servo 5–8 (±180°)
BLYNK_WRITE(V1) {
  if (param.asInt()) {
    dir2 = !dir2;
    terminal.printf(
      "🔘 Tombol 2 → Servo 5–8 BERURUTAN (%s)\n",
      dir2 ? "CCW" : "CW"
    );
    terminal.flush();

    moveGroupSequential(4, 7, MOVE_TIME_180, dir2);
  }
}

// Tombol 3 → Servo 9–16 (±90°)
BLYNK_WRITE(V2) {
  if (param.asInt()) {
    dir3 = !dir3;
    terminal.printf(
      "🔘 Tombol 3 → Servo 9–16 BERURUTAN (%s)\n",
      dir3 ? "CCW" : "CW"
    );
    terminal.flush();

    moveGroupSequential(8, 15, MOVE_TIME_90, dir3);
  }
}
