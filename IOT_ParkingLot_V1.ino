#include <WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 12); // RX, TX
#include "ThingSpeak.h"

#define SSID "RouterX"
#define PASSWORD "86023876839591174350"

#define ThingSpeakKEY "181MCAUYP7QRVJIC"

//int SENSOR=7;

#define LED_WLAN 13

#define DEBUG true

 
const int EntrySensor = 7; // Defines the Digital Ports used on the Arduino Nano as D07
const int ExitSensor = 5; // Defines the Digital Ports used on the Arduino Nano as D05

const int EmptyLED = 12;  // Defines the Digital Ports used on the Arduino Nano as D12
const int FilledLED = 10;  // Defines the Digital Ports used on the Arduino Nano as D10

// variables will change:
int EntrySensorState  = 0;
int ExitSensorState  = 0;
int CarEntry = 0;
int CarExit = 0;
int GarageCap = 10;
int CarState = 0;
bool Full = false;
bool Printfull = false;

const byte thingPost[] PROGMEM = {
  80, 79, 83, 84, 32, 42, 85, 82, 76, 42, 32, 72, 84, 84, 80, 47, 49, 46, 49, 10, 72, 111, 115, 116, 58, 32, 97, 112, 105, 46, 116, 104, 105, 110, 103, 115, 112, 101, 97, 107, 46, 99, 111, 109, 10, 67, 111, 110, 110, 101, 99, 116, 105, 111, 110, 58, 32, 99, 108, 111, 115, 101, 10, 67, 111, 110, 116, 101, 110, 116, 45, 84, 121, 112, 101, 58, 32, 97, 112, 112, 108, 105, 99, 97, 116, 105, 111, 110, 47, 120, 45, 119, 119, 119, 45, 102, 111, 114, 109, 45, 117, 114, 108, 101, 110, 99, 111, 100, 101, 100, 10, 67, 111, 110, 116, 101, 110, 116, 45, 76, 101, 110, 103, 116, 104, 58, 32, 42, 76, 69, 78, 42, 10, 10, 42, 65, 80, 80, 69, 78, 68, 42, 10
};

float s;

void setup() {
  // intialize the Entrance and Exit LED pin as an output:
  pinMode (EmptyLED, OUTPUT);
  pinMode (FilledLED, OUTPUT);
  
  // initialize the pushbutton or IR Sensors an input
  pinMode(EntrySensor, INPUT);
  pinMode(ExitSensor, INPUT);
  
  Serial.begin(19200);
  esp8266.begin(19200);
   
  if (!espConfig()) serialDebug();
  else digitalWrite(LED_WLAN, HIGH);
}
void loop() {
  
 //To read the state of the pushbutton value:
 EntrySensorState = digitalRead (EntrySensor);
 ExitSensorState = digitalRead (ExitSensor);

//check if the pushbutton / IR sensor is activated.
//if it is senses an obstacle, then the buttonState is HIGH:

//while (CarEntry < GarageCap) {
  
  //digitalWrite (EmptyLED, HIGH);

  if (pulseIn(EntrySensor, LOW, 5000)&& (CarState < GarageCap)) {
      CarEntry += 1; // increments the numbers of cars in the Garage
      delay (1000);        // just a processing delay
      CarState = CarEntry - CarExit;
      if (CarState == GarageCap) {
        Full = true;
      }
      
      //Serial.print("CarEntry: ");
      //Serial.println(CarEntry);
      Serial.print("CarState: ");
      Serial.println(CarState);
     
  }
  
 if   (pulseIn(ExitSensor, LOW, 5000) && CarExit < CarEntry) { // sensor detects car exit
  if (CarState == GarageCap) {
        Full = false;
        Printfull = false;
      }
      CarExit += 1; // decrements if a car exits the garage
      delay (1000);        // just a processing delay // waits for 1 second
      CarState = CarEntry - CarExit;
      
      //Serial.print("CarExit: ");
      //Serial.println(CarExit);
      Serial.print("CarState: ");
      Serial.println(CarState);
  }

if (Full == true){
    digitalWrite (FilledLED, HIGH); // Turns OFF the Green LED to signify no entrace is possible 
    digitalWrite (EmptyLED, LOW); // Turns ON the RED LED to signify maximum capacity
    if (Printfull == false) {
    Serial.println("Maximum Number of Car Entry Reached!!");
    Printfull = true;
    //Full == false ;
    }
}
else {
  digitalWrite (FilledLED, LOW); // Turns ON the Green LED to signify no entrace is possible 
  digitalWrite (EmptyLED, HIGH); // Turns OFF the RED LED to signify maximum capacity
}

sendThingPost(ThingSpeakKEY, CarState, "field1=");
sendThingPost(ThingSpeakKEY, CarEntry, "field2=");
sendThingPost(ThingSpeakKEY, CarExit, "field3=") ;
  delay(2000);
}

//-----------------------------------------ThingsSpeak Functions------------------------------------

boolean sendThingPost(String key, int value, String field)
{
  boolean succes = true;
  String  Host = "api.thingspeak.com";
  String msg = field + String(value);
  succes &= sendCom("AT+CIPSTART=\"TCP\",\"" + Host + "\",80", "OK");

  String postRequest = createThingPost("/update", key, msg);

  if (sendCom("AT+CIPSEND=" + String(postRequest.length()), ">"))
  {
    esp8266.print(postRequest);
    esp8266.find("SEND OK");
    if (!esp8266.find("CLOSED")) succes &= sendCom("AT+CIPCLOSE", "OK");
  }
  else
  {
    succes = false;
  }
  return succes;
}  

String createThingPost(String url, String key, String msg)
{
  String xBuffer;

  for (int i = 0; i <= sizeof(thingPost); i++)
  {
    char myChar = pgm_read_byte_near(thingPost + i);
    xBuffer += myChar;
  }

  String append = "api_key=" + key + "&" + msg;
  xBuffer.replace("*URL*", url);
  xBuffer.replace("*LEN*", String( append.length()));
  xBuffer.replace("*APPEND*", append);

  return xBuffer;
}

String createThingGet(String url, String key)
{
  String xBuffer;

  for (int i = 0; i <= sizeof(thingPost); i++)
  {
    char myChar = pgm_read_byte_near(thingPost + i);
    xBuffer += myChar;
  }

  String append = "api_key=" + key;
  xBuffer.replace("POST", "GET");
  xBuffer.replace("*URL*", url);
  xBuffer.replace("*LEN*", String( append.length()));
  xBuffer.replace("*APPEND*", append);

  return xBuffer;
}

String createThingGet(String url, String key, String msg)
{
  String xBuffer;

  for (int i = 0; i <= sizeof(thingPost); i++)
  {
    char myChar = pgm_read_byte_near(thingPost + i);
    xBuffer += myChar;
  }

  String append = "api_key=" + key + "&" + msg;

  xBuffer.replace("POST", "GET");
  xBuffer.replace("*URL*", url);
  xBuffer.replace("*LEN*", String( append.length()));
  xBuffer.replace("*APPEND*", append);

  return xBuffer;
}

//-----------------------------------------Config ESP8266------------------------------------

boolean espConfig()
{
  boolean succes = true;
  esp8266.setTimeout(5000);
  succes &= sendCom("AT+RST", "ready");
  esp8266.setTimeout(1000);

  if (configStation(SSID, PASSWORD)) {
    succes &= true;
    debug("WLAN Connected");
    debug("My IP is:");
    debug(sendCom("AT+CIFSR"));
  }
  else
  {
    succes &= false;
  }
  //shorter Timeout for faster wrong UPD-Comands handling
  succes &= sendCom("AT+CIPMUX=0", "OK");
  succes &= sendCom("AT+CIPMODE=0", "OK");

  return succes;
}

boolean configTCPServer()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMUX=1", "OK"));
  succes &= (sendCom("AT+CIPSERVER=1,80", "OK"));

  return succes;

}

boolean configTCPClient()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMUX=0", "OK"));
  //succes &= (sendCom("AT+CIPSERVER=1,80", "OK"));

  return succes;

}


boolean configStation(String vSSID, String vPASSWORT)
{
  boolean succes = true;
  succes &= (sendCom("AT+CWMODE=1", "OK"));
  esp8266.setTimeout(20000);
  succes &= (sendCom("AT+CWJAP=\"" + String(vSSID) + "\",\"" + String(vPASSWORT) + "\"", "OK"));
  esp8266.setTimeout(1000);
  return succes;
}

boolean configAP()
{
  boolean succes = true;

  succes &= (sendCom("AT+CWMODE=2", "OK"));
  succes &= (sendCom("AT+CWSAP=\"NanoESP\",\"\",5,0", "OK"));

  return succes;
}

boolean configUDP()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMODE=0", "OK"));
  succes &= (sendCom("AT+CIPMUX=0", "OK"));
  succes &= sendCom("AT+CIPSTART=\"UDP\",\"192.168.255.255\",90,91,2", "OK"); //Importand Boradcast...Reconnect IP
  return succes;
}




//-----------------------------------------------Controll ESP-----------------------------------------------------

boolean sendUDP(String Msg)
{
  boolean succes = true;

  succes &= sendCom("AT+CIPSEND=" + String(Msg.length() + 2), ">");    //+",\"192.168.4.2\",90", ">");
  if (succes)
  {
    succes &= sendCom(Msg, "OK");
  }
  return succes;
}


boolean sendCom(String command, char respond[])
{
  esp8266.println(command);
  if (esp8266.findUntil(respond, "ERROR"))
  {
    return true;
  }
  else
  {
    debug("ESP SEND ERROR: " + command);
    return false;
  }
}

String sendCom(String command)
{
  esp8266.println(command);
  return esp8266.readString();
}



//-------------------------------------------------Debug Functions------------------------------------------------------
void serialDebug() {
  while (true)
  {
    if (esp8266.available())
      Serial.write(esp8266.read());
    if (Serial.available())
      esp8266.write(Serial.read());
  }
}

void debug(String Msg)
{
  if (DEBUG)
  {
    Serial.println(Msg);
  }
}
