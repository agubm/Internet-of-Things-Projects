#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 12); // RX, TX
char msgBuffer[128];
int  msgLen = 0;
char commandStr[128];

#include <SevenSeg.h>

SevenSeg disp(2, 4, 7, 6, 5, 3, 8);

const int numOfDigits = 4;
int digitPins[numOfDigits] = {A1, A2, A3, 13};

const int EntrySensor = 10;
const int ExitSensor = 9;
const int EmptyLED = A4;
const int FilledLED = A5;

// variables will change:
int EntrySensorState  = 0;
int ExitSensorState  = 0;
int CarEntry = 0;
int CarExit = 0;
int GarageCap = 0;
int AvSpots = 0;
int CarState = 0;
int check = 0;

bool Full = false;
bool Printfull = false;
bool sent = true;

const char EOL = '\n'; // End of line
String response = ""; // Create String once, outside loop()

void setup() {

  disp.setDigitPins(numOfDigits, digitPins);

  // intialize the Entrance and Exit LED pin as an output:
  pinMode (EmptyLED, OUTPUT);
  pinMode (FilledLED, OUTPUT);

  // initialize the IR Sensoras an input
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

  // Get GarageCap
  sprintf(msgBuffer, "Parking|GET|A1E-DaxmisPrrMJiZKVdbg62PIZmBfHVy4|5d10ea76c03f9707152a2020|end");
  msgLen = strlen(msgBuffer);
  while (GarageCap == 0) {
    // send message
    sprintf(commandStr, "AT+CIPSEND=%d", msgLen);
    esp8266.println(commandStr);
    delay(1000);
    esp8266.write(msgBuffer);
    char c = '\0'; // Initialise to NUL
    while (esp8266.available() && c != EOL ) { // Wait for End of Line
      c = esp8266.read();
      response += c;
    } // while
    while (c == EOL) {
      //Serial.print("Response:"); // Don't use '+' if you can help it!!!
      if (response.substring(6, 10) == ":OK|") {
        int pch = response.indexOf('|', 10);
        String strval = response.substring(10, pch);
        int res = strval.toInt();
        GarageCap = res;
        break;
      }
      else {
        // Do other processing
        response = ""; // Clear response for next string
        break;
      }
    }
  }
  msgBuffer[128] = '\0';
  msgLen = 0;
  commandStr[128] = '\0';
}

void loop() {

  check = CarState;

  while (1) {
    if (CarState == GarageCap)
    {
      disp.write("FULL");
    }
    else {
      disp.write(GarageCap - CarState);
    }

    //To read the state of the pushbutton value:
    EntrySensorState = digitalRead (EntrySensor);
    ExitSensorState = digitalRead (ExitSensor);

    if (IR_state(EntrySensor, "IN") && (CarState < GarageCap))
    {
      CarEntry += 1; 
      CarState = CarEntry - CarExit;
      if (CarState == GarageCap) {
        Full = true;
      }
      Serial.print("CarState: ");
      Serial.println(CarState);
    }


    if (IR_state(ExitSensor, "OUT") && CarExit < CarEntry) { // sensor detects car exit
      if (CarState == GarageCap) {
        Full = false;
        Printfull = false;
      }
      CarExit += 1;
      CarState = CarEntry - CarExit;
      
      Serial.print("CarState: ");
      Serial.println(CarState);

    }

    // Leds functionality
    if (Full == true) {
      analogWrite (FilledLED, 255); // Turns ON the RED LED to signify maximum capacity
      analogWrite (EmptyLED, 0); // Turns OFF the Green LED to signify no entrace is possible
      if (Printfull == false) {
        Serial.println("Maximum Number of Car Entry Reached!!");
        Printfull = true;
        Full == false ;
      }
    }
    else {
      analogWrite (FilledLED, 0); // Turns OFF the RED LED to signify maximum capacity
      analogWrite (EmptyLED, 255); // Turns ON the Green LED to signify no entrace is possible
    }

    if ( check != CarState) {
      sent = true;
      break;
    }

  }

  AvSpots = GarageCap-CarState;
  while (sent == true && check != CarState) {
    // ----------------------------build message payload----------------------------
    sprintf(msgBuffer, "Parking|POST|A1E-DaxmisPrrMJiZKVdbg62PIZmBfHVy4|ParkingLot=>CarState:%d,CarEntry:%d,CarExit:%d,AvSpots:%d|end", CarState, CarEntry,  CarExit, AvSpots);
    // where A1E-zRK.....5I is your token, arduino-pretzel is the ParkingLot of your device,
    // and CarState is the API label of your variable name from your device
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

int IR_state(int inputpin, char msg[6])
{
  int startTime = 0;
  int endTime = 0;
  int Time = 0;
  int state = 0;

  if (digitalRead(inputpin) == LOW) {
    while (digitalRead(inputpin) == LOW) {
      Time++;
      disp.write(msg);
    }
    if (Time > 150)
    {
      state = 1;
    }
  }
  return state;
}
