#include <NewPing.h>    //import libraries
#include <LiquidCrystal.h>

char incomingByte; // for incoming serial data
char prevByte ='\0'; // for incoming serial data

const int echo_L = 30;   //initialize pin numbers                              
const int trig_L = 31;
const int echo_M = 4;
const int trig_M = 5;
const int echo_R = 7;
const int trig_R = 8;

const int in1 =6;
const int in2 =9;
const int in3 =11;
const int in4 =10;

const int button = 12;
const int pump = 13;
bool pumpState = LOW;

int contrast = 10;

int motor_speed = 160;    
int motor_speed2 = 198;
int max_distance = 200;   
int distance_L = 0;
int distance_M = 0;
int distance_R = 0;

unsigned long previousMillis = 0;
const long intervalOn = 1500;  // LED on time (5 seconds)
const long intervalOff = 10000; // LED off time (10 seconds)

NewPing sonar_L(trig_L, echo_L, max_distance);    //initialize all the 3 sensors
NewPing sonar_M(trig_M, echo_M, max_distance);
NewPing sonar_R(trig_R, echo_R, max_distance);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);    //initialize LCD

void setup() {
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  pinMode(button, INPUT);
  pinMode(pump, OUTPUT);
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(pump, LOW);
  analogWrite(3,contrast);
  lcd.begin(16, 2);
  lcd.print("Welcome!");
  Serial.begin(9600);   //begin serial communication via bluetooth at 9600 baud rate
  delay(2000);
}

void stop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  // Serial.println("Stopped");
}
void moveForward(){
  analogWrite(in1,motor_speed2);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  analogWrite(in4,motor_speed);
  Serial.println("Moving Forward");
}

void moveBackward(){
  digitalWrite(in1,LOW);
  analogWrite(in2,motor_speed2);
  analogWrite(in3,motor_speed);
  digitalWrite(in4,LOW);
  Serial.println("Moving Backward");
}
void turnLeft(){
  digitalWrite(in1,LOW);
  analogWrite(in2,motor_speed2);
  digitalWrite(in3,LOW);
  analogWrite(in4,motor_speed);
  Serial.println("Turning Left");
}

void turnRight(){
  analogWrite(in1,motor_speed2);
  digitalWrite(in2,LOW);
  analogWrite(in3,motor_speed);
  digitalWrite(in4,LOW);
  Serial.println("Turn Right");
}

void moveVehicle(char c){
  if(c=='f'){
    moveForward();
  }else if(c == 'b'){
    moveBackward();
  }else if(c == 'r'){
    turnRight();
  }else if(c=='l'){
    turnLeft();
  }
  return ;
}



void loop() {
  
  if(digitalRead(button) == LOW)    //if button is not pressed
  {
    lcd.clear();                    //manual mode
    lcd.print("Manual Mode");
    manualMode();
    delay(100);
  }
  
  else                              //else automatic mode
  {
    lcd.clear();
    lcd.print("Automatic Mode");
    // startTime = 0;
    while(true)
    {
      automaticMode();
      if(digitalRead(button) == LOW)
      {
        stop();
        break;
      }
    }
    delay(100);
  }
}

void manualMode()
{
  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);
  }
  if(prevByte == incomingByte){
    incomingByte = 's';
    prevByte = '\0';
  }

  switch(incomingByte)    //based on received character execute respective commands
  {
    case 'f':
    moveForward();
    lcd.clear();
    lcd.print("Forward");
    prevByte = incomingByte;
    incomingByte='*';
    break;
    
    case 'b':
    moveBackward();
    lcd.clear();
    lcd.print("Backward");
    prevByte = incomingByte;
    incomingByte='*';
    break;
    
    case 'l':
    turnLeft();
    lcd.clear();
    lcd.print("Left");
    prevByte = incomingByte;
    incomingByte='*';
    break;
    
    case 'r':
    turnRight();
    lcd.clear();
    lcd.print("Right");
    prevByte = incomingByte;
    incomingByte='*';
    break;
    
    case 's':
    stop();
    lcd.clear();
    lcd.print("Stop");
    prevByte = incomingByte;
    incomingByte='*';
    break;
    
    case 'P':
    digitalWrite(pump, HIGH);
    lcd.clear();
    lcd.print("Pump ON");
    incomingByte='*';
    break;
    
    case 'p':
    digitalWrite(pump, LOW); 
    incomingByte='*';
    break;
    
    case '1':
    motor_speed = 155;
    incomingByte='*';
    break;
    
    case '2':
    motor_speed = 205;
    incomingByte='*';

    break;
    
    case '3':
    motor_speed = 255;
    incomingByte='*';
    prevByte = incomingByte;
    break;
    
    delay(5000);
  }
  
}

void automaticMode()
{
  unsigned long currentMillis = millis();
  if (pumpState == LOW && currentMillis - previousMillis >= intervalOff) {
    pumpState = HIGH;
    digitalWrite(pump, pumpState);
    previousMillis = currentMillis;
  } 
  else if (pumpState == HIGH && currentMillis - previousMillis >= intervalOn) {
    pumpState = LOW;
    digitalWrite(pump, pumpState);
    previousMillis = currentMillis;
  }
  distance_L = readSensor_L();    //read distance from all the 3 sensors
  distance_M = readSensor_M();
  distance_R = readSensor_R();
  Serial.print("distance_L: ");
  Serial.println(distance_L);
  Serial.print("distance_M: ");
  Serial.println(distance_M);
  Serial.print("distance_R: ");
  Serial.println(distance_R);
  lcd.clear();    //print distance on LCD
  lcd.print("L=");
  lcd.print(distance_L);
  lcd.print("cm ");
  lcd.print("M=");
  lcd.print(distance_M);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("R=");
  lcd.print(distance_R);
  lcd.print("cm");
  
  if(distance_M <= 20)    //if middle sensor distance is less than 20cm
  {
    if(distance_R > distance_L)   //check if there is place at right or left
    {
      if((distance_R <= 20) && (distance_L <= 20))    //if there is no place on both sides
      {
        stop();
        delay(200);
        moveBackward();   //move back
        delay(2000);
      }
      else
      {
        moveBackward();  //move back then turn right
        delay(500);
        turnRight();
        delay(2000);
      }
    }
    else 
    if(distance_R < distance_L)
    {
      if((distance_R <= 20) && (distance_L <= 20))
      {
        stop();   //move back
        delay(200);
        moveBackward();
        delay(2000);
      }
      else
      {
        moveBackward();   //move back then turn left
        delay(500);
        turnLeft();
        delay(2000);
      }
    }
  }
  
  else 
  if(distance_R <= 15)    //if right sensor distance is less than 20cm
  {
    turnLeft();   //turn left
    delay(500);
  }
  else
  if(distance_L <= 15)    //if left sensor distance is less than 20cm
  {
    turnRight();    //turn right
    delay(500);
  }
  else
  {
    moveForward();    //in all other cases keep on moving forward
  }
}

int readSensor_L()    //read distance in centimeters from left sensor
{ 
  delay(70);
  int cm_L = sonar_L.ping_cm();
  if(cm_L==0)
  {
    cm_L = 250;
  }
  return cm_L;
}

int readSensor_M()    //read distance in centimeters from left sensor
{ 
  delay(70);
  int cm_M = sonar_M.ping_cm();
  if(cm_M==0)
  {
    cm_M = 250;
  }
  return cm_M;
}

int readSensor_R()    //read distance in centimeters from left sensor
{ 
  delay(70);
  int cm_R = sonar_R.ping_cm();
  if(cm_R==0)
  {
    cm_R = 250;
  }
  return cm_R;
}