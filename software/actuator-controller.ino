#include <esp_now.h>
#include <esp_arduino_version.h>
#include <WiFi.h>

namespace {
constexpr int MOTOR_ENABLE_PIN = 25;
constexpr int MOTOR_INPUT_1_PIN = 32;
constexpr int MOTOR_INPUT_2_PIN = 33;
constexpr unsigned long COMMAND_TIMEOUT_MS = 750;

struct ControlMessage {
  uint8_t downPressed;
  uint8_t upPressed;
  uint32_t sequence;
};

volatile ControlMessage latestCommand{};
volatile unsigned long lastCommandTime = 0;

void stopMotor() {
  digitalWrite(MOTOR_ENABLE_PIN, LOW);
  digitalWrite(MOTOR_INPUT_1_PIN, LOW);
  digitalWrite(MOTOR_INPUT_2_PIN, LOW);
}

void driveDown() {
  digitalWrite(MOTOR_INPUT_1_PIN, HIGH);
  digitalWrite(MOTOR_INPUT_2_PIN, LOW);
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
}

void driveUp() {
  digitalWrite(MOTOR_INPUT_1_PIN, LOW);
  digitalWrite(MOTOR_INPUT_2_PIN, HIGH);
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
}

#if ESP_ARDUINO_VERSION_MAJOR >= 3
#define ESP_NOW_RECEIVE_ARGS \
  const esp_now_recv_info_t *, const uint8_t *incomingData, int length
#else
#define ESP_NOW_RECEIVE_ARGS \
  const uint8_t *, const uint8_t *incomingData, int length
#endif

void onDataReceived(ESP_NOW_RECEIVE_ARGS) {
  if (length != sizeof(ControlMessage)) {
    return;
  }

  memcpy((void *)&latestCommand, incomingData, sizeof(ControlMessage));
  lastCommandTime = millis();
}
}  // namespace

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_INPUT_1_PIN, OUTPUT);
  pinMode(MOTOR_INPUT_2_PIN, OUTPUT);
  stopMotor();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to initialise ESP-NOW.");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
}

void loop() {
  if (millis() - lastCommandTime > COMMAND_TIMEOUT_MS) {
    stopMotor();
    return;
  }

  const bool downPressed = latestCommand.downPressed;
  const bool upPressed = latestCommand.upPressed;

  if (downPressed == upPressed) {
    stopMotor();
  } else if (downPressed) {
    driveDown();
  } else {
    driveUp();
  }
}
