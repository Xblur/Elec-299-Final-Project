#define GRIPTHRESHOLD 300
#include <Servo.h>


int panPIN = 8;
int tiltPIN = 9;
int gripPIN = 10;

int forceReading = 1000;

int forceSensor = A4;

int leftSpeed = 5;
int leftDirection = 4;
int rightSpeed = 6;
int rightDirection = 7;

Servo pan, tilt, grip;

void setup(){

  Serial.begin(9600);
  pan.attach(panPIN);
  tilt.attach(tiltPIN);
  grip.attach(gripPIN);

  pinMode(leftSpeed,  OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(forceSensor,INPUT);


  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);

  pan.write(90);
  tilt.write(75);
  grip.write(90);
  

}

void loop(){
  delay(2500);
  closeGrip();
  delay(100);
  exit(0);
}

void closeGrip(){
  int i = 75;
  
  forceReading = analogRead(forceSensor);
  while(forceReading > GRIPTHRESHOLD){
    forceReading = analogRead(forceSensor);
    Serial.println(forceReading);
    i++;
    int j = i%200;
    Serial.println(j);
    grip.write(j);
    delay(10);
  }
  
  Serial.println("OBJECT GRABBED");
}
