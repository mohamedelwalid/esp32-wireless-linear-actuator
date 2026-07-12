#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x4C,
0x11, 0xAE, 0xB3, 0xA0, 0x20};
int speedpin = 18; // pinnen som leser av potensiometer
int
downpin = 16; // knapp som spinner motor en vei
int uppin = 17; // knapp som spinner motor
annen vei
/*int lastbuttonstate = LOW;
unsigned long lastDebounceTime = 0; // the last time
the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase
if the output flickers*/ //brukes ikke enda
typedef struct struct_message{
int
speedval;
int downState;
int upState;
} struct_message;
// Lage en struct_message
som heter mydata
struct_message mydata;
esp_now_peer_info_t peerInfo;
//callback
when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
Serial.print ("\r\nLast Packet Send Status:\t");
Serial.println(status ==
ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void setup() {
Serial.begin(115200);
pinMode (speedpin, INPUT);
pinMode (downpin,
INPUT_PULLUP);
pinMode (uppin, INPUT_PULLUP);
// Set device as a Wi-Fi Station
WiFi.mode(WIFI_STA);
// Init ESP-NOW
if (esp_now_init() != ESP_OK) {
Serial.println("Error initializing ESP-NOW");
return;
}
// Once ESPNow
is successfully Init, we will register for Send CB to
// get the status of Trasnmitted
packet
esp_now_register_send_cb(OnDataSent);
// Register peer
memcpy(peerInfo.peer_addr, broadcastAddress, 6);
peerInfo.channel = 0;
peerInfo.encrypt
= false;
// Add peer
if (esp_now_add_peer(&peerInfo) != ESP_OK){
Serial.println("Failed to add peer");
return;
}
}
void loop()
{
// Set values to send
if (analogRead(speedpin) == 0){
mydata.speedval =
255;
Serial.println("speedval er 255");
}
if(analogRead(speedpin
!= 0)) {
mydata.speedval = analogRead(speedpin);
}
mydata.downState =
digitalRead(downpin);
mydata.upState = digitalRead(uppin);
Serial.println(mydata.downState);
Serial.println(mydata.upState);
Serial.println(mydata.speedval);
// Send message via ESP-NOW
esp_err_t result =
esp_now_send(broadcastAddress, (uint8_t *) &mydata, sizeof(mydata));
if (result ==
ESP_OK) {
Serial.println("Sent with success");
}
else {
Serial.println("Error sending the data");
}
delay(500);
}
