
//Awesome!

int carstate = 0000;
int delaytime = 5;

int pinA = 13;
int pinB = 2;
int pinC = 3;
int pinD = 4;
int pinE = 5;
int pinF = 6;
int pinG = 7;
int pinDP = 8;

int pinD1 = 12;
int pinD2 = 10;
int pinD3 = 11;
int pinD4= 9;


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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
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
    digitalWrite(pinDP, HIGH);
    break;
  }
}
void setup() {
  // put your setup code here, to run once:
pinMode(pinD1, OUTPUT);
pinMode(pinD2, OUTPUT);
pinMode(pinD3, OUTPUT);
pinMode(pinD4, OUTPUT);

pinMode(pinA, OUTPUT);
pinMode(pinB, OUTPUT);
pinMode(pinC, OUTPUT);
pinMode(pinD, OUTPUT);
pinMode(pinE, OUTPUT);
pinMode(pinF, OUTPUT);
pinMode(pinG, OUTPUT);
pinMode(pinDP, OUTPUT);

Serial.begin(9600);
}

void pick_digit(int digit) {
 digitalWrite(pinD1, LOW);
 digitalWrite(pinD2, LOW);
 digitalWrite(pinD3, LOW);
 digitalWrite(pinD4, LOW);

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
  // put your main code here, to run repeatedly:
seven_segment (carstate); 
//for (int i = 0; i < 10; i++) {
//choose_digit(i);
delay (1);
}
