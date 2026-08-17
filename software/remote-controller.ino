#include <esp_now.h>
#include <WiFi.h>

namespace {
constexpr int DOWN_BUTTON_PIN = 16;
constexpr int UP_BUTTON_PIN = 17;
constexpr unsigned long SEND_INTERVAL_MS = 50;

// Replace with the actuator ESP32 station MAC address before uploading.
uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

struct ControlMessage {
  uint8_t downPressed;
  uint8_t upPressed;
  uint32_t sequence;
};

ControlMessage command{};
esp_now_peer_info_t peerInfo{};
unsigned long lastSendTime = 0;

bool receiverAddressIsConfigured() {
  for (const uint8_t byte : receiverAddress) {
    if (byte != 0x00) {
      return true;
    }
  }
  return false;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (!receiverAddressIsConfigured()) {
    Serial.println("Configure receiverAddress before operating the actuator.");
    return;
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to initialise ESP-NOW.");
    return;
  }

  memcpy(peerInfo.peer_addr, receiverAddress, sizeof(receiverAddress));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to register actuator peer.");
  }
}

void loop() {
  if (!receiverAddressIsConfigured() || millis() - lastSendTime < SEND_INTERVAL_MS) {
    return;
  }

  lastSendTime = millis();
  command.downPressed = digitalRead(DOWN_BUTTON_PIN) == LOW;
  command.upPressed = digitalRead(UP_BUTTON_PIN) == LOW;
  command.sequence++;

  // Conflicting inputs are converted to a stop command.
  if (command.downPressed && command.upPressed) {
    command.downPressed = 0;
    command.upPressed = 0;
  }

  const esp_err_t result =
      esp_now_send(receiverAddress, reinterpret_cast<uint8_t *>(&command), sizeof(command));

  if (result != ESP_OK) {
    Serial.println("Unable to queue ESP-NOW command.");
  }
}
