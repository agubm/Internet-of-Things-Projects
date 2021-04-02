#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 12); // RX, TX
char msgBuffer[128];
int  msgLen = 0;
char commandStr[128];

///Awesome!!!

int delaytime = 2;

int pinA = 2;
int pinB = 4;
int pinC = 7;
int pinD = 6;
int pinE = 5;
int pinF = 3;
int pinG = 8;
int pinDP = A6;

int pinD1 = A1;
int pinD2 = A2;
int pinD3 = A3;
int pinD4= 13;


const int EntrySensor = 10;
const int ExitSensor = 9;
const int EmptyLED = A4;
const int FilledLED = A5;

// variables will change:
int EntrySensorState  = 0;
int ExitSensorState  = 0;
int CarEntry = 0;
int CarExit = 0;
int GarageCap = 10;
int CarState;
int check = 0;

bool Full = false;
bool Printfull = false;
bool sent = true;


void choose_digit(char num){
  switch(num){
    default:
    //0 is the default value
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;

   //1
  case 1:
    digitalWrite(pinA, HIGH );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH); 
   digitalWrite(pinDP, HIGH); // uncomment or delete
    break;

   //2
   case 2:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;

  //3
   case 3:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;

   
  //4
   case 4:
    digitalWrite(pinA, HIGH );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;


  //5
 case 5:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;
    
  //6
 case 6:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;
  
  //7
 case 7:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;

  //8
 case 8:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;
  
  //9
   case 9:
    digitalWrite(pinA, LOW );
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW); 
    digitalWrite(pinDP, HIGH); // uncomment or delete
    break;
  }
}


void setup() {

pinMode(pinD1, OUTPUT);
pinMode(pinD2, OUTPUT);
pinMode(pinD3, OUTPUT); // comment or delete
pinMode(pinD4, OUTPUT); // comment or delete

pinMode(pinA, OUTPUT);
pinMode(pinB, OUTPUT);
pinMode(pinC, OUTPUT);
pinMode(pinD, OUTPUT);
pinMode(pinE, OUTPUT);
pinMode(pinF, OUTPUT);
pinMode(pinG, OUTPUT);
pinMode(pinDP, OUTPUT); // comment or delete

  
  // intialize the Entrance and Exit LED pin as an output:
  pinMode (EmptyLED, OUTPUT);
  pinMode (FilledLED, OUTPUT);
  
  // initialize the pushbutton or IR Sensoras an input
  pinMode(EntrySensor, INPUT);
  pinMode(ExitSensor, INPUT);
  
// init serial communications
Serial.begin(9600);

esp8266.begin(19200);
esp8266.setTimeout(10000);
// reset WLAN
esp8266.println("AT+RST");
if (!esp8266.findUntil("ready", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+RST␣failed");
}
// config WLAN
esp8266.println("AT+CWMODE=1");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CWMODE=1␣failed");
}
esp8266.println("AT+CWJAP=\"AJA\",\"vxla0711\"");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CWJAP=\"Nomad_EXT\"...␣failed");
}
// get IP Address
esp8266.println("AT+CIFSR");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CIFSR");
}
// setup TCP connection
esp8266.println("AT+CIPSTART=\"TCP\",\"things.ubidots.com\",9012");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CIPSTART␣failed");
}
}

void pick_digit(int digit) {
 digitalWrite(pinD1, LOW);
 digitalWrite(pinD2, LOW);
 digitalWrite(pinD3, LOW); // uncomment or delete
 digitalWrite(pinD4, LOW); // uncomment or delete

 switch(digit) {
  case 1: digitalWrite(pinD1, HIGH); break;

  case 2: digitalWrite(pinD2, HIGH); break;

  case 3: digitalWrite(pinD3, HIGH);break;

  default: digitalWrite(pinD4, HIGH); break;
 }
}


void seven_segment (int number) {
unsigned char thousands = int ((number/1000));
unsigned char hundreds = int ((number/100)% 10);
unsigned char tens = int ((number/10)% 10);
unsigned char units = int (number%10);
  
//1234
choose_digit (thousands);
pick_digit (1);
delay (delaytime);

choose_digit (hundreds);
pick_digit (2);
delay (delaytime);

choose_digit (tens);
pick_digit (3);
delay (delaytime);

choose_digit (units);
pick_digit (4);
delay (delaytime);

}



void loop() {


check = CarState;

while (1) {

seven_segment (CarState);
delay(1);

//To read the state of the pushbutton value:
EntrySensorState = digitalRead (EntrySensor);
ExitSensorState = digitalRead (ExitSensor);


//check if the pushbutton / IR sensor is activated.
//if it is senses an obstacle, then the buttonState is HIGH:

//while (CarEntry < GarageCap) {
  
  //digitalWrite (EmptyLED, HIGH);


if (pulseIn(EntrySensor, LOW, 5000)&& (CarState < GarageCap)) 
{
  CarEntry += 1; // increments the numbers of cars in the Garage
  //delay (500);        // just a processing delay  // ***I changed from 1000 to 100, and It was a lot faster
  CarState = CarEntry - CarExit;
  if (CarState == GarageCap) {
  Full = true;
  }
    
//Serial.print("CarEntry: ");
//Serial.println(CarEntry);
Serial.print("CarState: ");
Serial.println(CarState);
}
    
  
if (pulseIn(ExitSensor, LOW, 5000) && CarExit < CarEntry) { // sensor detects car exit
  if (CarState == GarageCap) {
    Full = false;
    Printfull = false;
    }
  CarExit += 1; // decrements if a car exits the garage
  //delay (1000);        // just a processing delay // waits for 1 second // ***I changed from 1000 to 100, and It was a lot faster
  CarState = CarEntry - CarExit;
      
     
//Serial.print("CarExit: ");
//Serial.println(CarExit);
Serial.print("CarState: ");
Serial.println(CarState);

  }
 
// Leds functionality 
if (Full == true){
  analogWrite (FilledLED, 255); // Turns OFF the Green LED to signify no entrace is possible 
  analogWrite (EmptyLED, 0); // Turns ON the RED LED to signify maximum capacity
  if (Printfull == false) {
  Serial.println("Maximum Number of Car Entry Reached!!");
  Printfull = true;
  Full == false ;
  }
}
else {
  analogWrite (FilledLED, 0); // Turns ON the Green LED to signify no entrace is possible 
  analogWrite (EmptyLED, 255); // Turns OFF the RED LED to signify maximum capacity
}

if ( check != CarState) {
  sent = true;
  break;
}

}

while (sent==true && check != CarState) {
// ----------------------------build message payload----------------------------
sprintf(msgBuffer, "Parking|POST|A1E-zRK2DSraPK4O3JwD5qPXlNOtf0s75I|ParkingLot=>CarState:%d,CarEntry:%d,CarExit:%d|end", CarState, CarEntry,  CarExit);
// where BBFF-KNY.....kn is your token, arduino-pretzel is the API label of your device, 
// and test is the API label of your variable name from your device
 msgLen = strlen(msgBuffer);
// send message
sprintf(commandStr, "AT+CIPSEND=%d", msgLen);
esp8266.println(commandStr);
delay(1000);
esp8266.write(msgBuffer);
Serial.println("Sent:␣TCP");
Serial.println(msgBuffer);
delay(1000);

sent = false;
}
}
