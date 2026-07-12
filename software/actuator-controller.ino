int speedVal = 255;
int downstate = 0;
int upstate = 0;
int ENA = 25;
int motor1pin1 =
32; //
int motor1pin2 = 33; //
unsigned long previouseMillis = 0;
const long interval =
500;
#include <esp_now.h>
#include <WiFi.h>
// Structure example to receive
data
// Must match the sender structure
typedef struct struct_message {
int speedval;
int downState;
int upState;
} struct_message;
// Create a struct_message called
myData
struct_message mydata;
// callback function that will be executed when data is
received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
unsigned long currentMillis = millis();
if (currentMillis - previouseMillis >=
interval){
memcpy(&mydata, incomingData, sizeof(mydata));
Serial.print("Bytes
received: ");
Serial.println(len);
Serial.print("Speedval: ");
Serial.println(mydata.speedval);
Serial.print("upState: ");
Serial.println(mydata.upState);
Serial.print("downState: ");
Serial.println(mydata.downState);
Serial.println();
}
}
void setup() {
//
Initialize Serial Monitor
Serial.begin(115200);
// Set device as a Wi-Fi Station
WiFi.mode(WIFI_STA);
// Init ESP-NOW
if (esp_now_init() != ESP_OK) {
Serial.println("Error initializing ESP-NOW");
return;
}
// Once ESPNow
is successfully Init, we will register for recv CB to
// get recv packer info
esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
pinMode (motor1pin1, OUTPUT);
pinMode (motor1pin2, OUTPUT);
pinMode (ENA, OUTPUT);
}
void loop() {
//unsigned
long currentMillis = millis();
int speedVal = mydata.speedval;
int downState =
mydata.downState;
int upState = mydata.upState;
if (downState == LOW){
//speedVal = map(speedVal,0 ,4095,0 ,255 ); // gjør verdiene fra potensiometeret fra 0-1023
til 0-255
digitalWrite(motor1pin1, HIGH);
digitalWrite(motor1pin2, LOW);
analogWrite(ENA, speedVal);
Serial.println(speedVal);
Serial.println(downState);
}
else if (upState == LOW){
//speedVal = map(speedVal,0 ,4095,0 ,255 ); // gjør verdiene
fra potensiometeret fra 0-1023 til 0-255
digitalWrite(motor1pin1, LOW);
digitalWrite(motor1pin2, HIGH);
analogWrite(ENA, speedVal);
Serial.println(speedVal);
Serial.println(upState);
}
else if (upState == HIGH && downState == HIGH) {
digitalWrite(motor1pin1, LOW);
digitalWrite(motor1pin2, LOW);
}
}
