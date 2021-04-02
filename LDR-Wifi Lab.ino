#include<SoftwareSerial.h>
SoftwareSerial esp8266(11, 12);//RX,TX
int count = 0;
char msgBuffer[64];
int msgLen = 0;
char commandStr[64];

//A0pintowhichPhotoResistorisconnected
int lightPin = 0;
//VariabletostorethePhotoResistorreading
int lightValue = 0;

void setup() {
//initserialcommunications
Serial.begin(9600);
esp8266.begin(19200);
esp8266.setTimeout(10000);

//reset WLAN
esp8266.println("AT+RST");
if (!esp8266.findUntil("ready","ERROR")) {
Serial.println("ESP ERROR: AT+RST failed");
}
//config WLAN
esp8266.println("AT+CWMODE=1");
if (!esp8266.findUntil("OK","ERROR")) {
Serial.println("ESP ERROR: AT+CWMODE=1 failed");
}
esp8266.println("AT+CWJAP=\"CN-Students\",\"CnStudentsWiFiAccess123\"");
if (!esp8266.findUntil("OK","ERROR")) {
Serial.println("ESP ERROR: AT+CWJAP=\"Nomad_EXT\"... failed");
}
//getIPAddress
esp8266.println("AT+CIFSR");
if (!esp8266.findUntil("OK","ERROR")) {
Serial.println("ESP ERROR: AT+CIFSR");
}
//setupUDPconnection
esp8266.println("AT+CIPSTART=\"UDP\",\"192.168.1.126\",9999,8888,0");
if (!esp8266.findUntil("OK","ERROR")) {
Serial.println("ESP ERROR: AT+CIPSTART failed");
  }
}
void loop() {
  lightValue = analogRead(lightPin);
  //buildmessagepayload
  sprintf(msgBuffer,"3206166 : Seq %d : %d", ++count, lightValue);
  msgLen = strlen(msgBuffer);
  //sendmessage
  sprintf(commandStr,"AT+CIPSEND=%d", msgLen);
  esp8266.println(commandStr);
  delay(1000);
  esp8266.write(msgBuffer);
  Serial.println("Sent: UDP");
  Serial.println(msgBuffer);
  delay(500);
}
