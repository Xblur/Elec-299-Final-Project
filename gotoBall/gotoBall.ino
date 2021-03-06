#include <Servo.h>

int type =0;//initial position. left-0, middle-1, right-2

int cy = 0;
int cx = 2;
int cd = 0;

int IRpin = A5;

int forceSensor = A4;
int forceThresh = 300;

int lIRPin = A0;
int cIRPin = A2;
int rIRPin = A1;

int lVal = 0;
int cVal = 0;
int rVal = 0;

int plVal = 0;
int pcVal = 0;
int prVal = 0;

int leftSpeed = 5;
int leftDirection = 4;
int rightSpeed = 6;
int rightDirection = 7;
int thresh = 850;
unsigned long lastInter = 0;
int IRVal =0;


//---------------------------------

int panPIN = 8;
int tiltPIN = 9;
int gripPIN = 10;

Servo pan, tilt,grip;


void setup() {
  // put your setup code here, to run once:
  pan.attach(panPIN);
  tilt.attach(tiltPIN);
  grip.attach(gripPIN);
  
  // put your setup code here, to run once:
  pinMode(lIRPin, INPUT);
  pinMode(cIRPin, INPUT);
  pinMode(rIRPin, INPUT);

  pinMode(leftSpeed,  OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(rightDirection, OUTPUT);

  pinMode(IRpin, INPUT);
  pinMode(forceSensor, INPUT);

  pan.write(90);
  tilt.write(75);
  grip.write(140);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  /*
 forward(2);
 turn(1);
 turn(1);*/
 Serial.println(analogRead(forceSensor));



}

//Function for robot to move to a specified ball's location
void GoToDice(int x, int y, int d)
{
  if (cy<y){ //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    forward(y-cy);
    cy = y;
    Serial.println("Done with Y");
 
  }
  if(cx!=x){ //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if(cx<x){
      turn(0);
      forward(x-cx);
      cx = x;
    }
    else if(cx>x){
      Serial.println("I will turn Left");
      turn(1);
      forward(cx-x);
      cx = x;
    }
  
  }
  if(cd!=d){ //if the robot is not facing the ball, then turn towards the ball
      if(abs(cd-d) == 2){
        turn(1);
        turn(1);
      }
      else if(cd-d ==-3){
        turn (1);
      }
      else if(cd-d ==3){
        turn (0);
      }
      else if(cd-d>0)
      {
        turn(1);
      }
      else if(cd-d<0)
      {
        turn(0);
      }
    }
    
    approach();
      
}
void GoToBin(int type)//left-0, middle-1 or right-2
{
  int y = 0;
  int x =type+2;//gives location of bin depending on starting position
  
  
  if(cx!=x){ //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if(cx<x){
      forward(x-cx);
      cx = x;
      turn(0);//turn right
    }
    else if(cx>x){
      forward(cx-x);
      cx = x;
      turn(1);//turn left
    }
  
  }
  if (cy!=y){ //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    forward(cy-y);
    cy = y;
  }
  //drop into bin function
}
          



//Function to move robot forward a specified number of intersections
void forward(int numOfIntersections){
  int intersectionCount = 0;
  digitalWrite(leftDirection, HIGH); //Set left motor direction to forward
  digitalWrite(rightDirection, HIGH); //Set right motor direction to forward

  //While not at the desired intersection, keep moving forward
  while(intersectionCount < numOfIntersections){
    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);

     /*  Serial.print(lVal);
      Serial.print(" || ");
      Serial.print(cVal);
       Serial.print(" || ");
       Serial.println(rVal); */
    if((lVal < thresh) && (cVal > thresh) && (rVal < thresh)){ //SET MOTORS TO DRIVE FORWARD
      analogWrite(leftSpeed, 148);
      analogWrite(rightSpeed, 128);
    }else if((lVal > thresh) && (cVal < thresh) && (rVal < thresh)){//LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, 108);
      analogWrite(rightSpeed, 128);
    }else if((lVal < thresh) && (cVal < thresh) && (rVal > thresh)){//LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, 208);
      analogWrite(rightSpeed, 128);
    }

    if((lVal > thresh) && (cVal > thresh) && (rVal > thresh) && (plVal > thresh) && (pcVal > thresh) && (prVal > thresh)){ //At an intersection. Increment intersection counter
      if((millis()-lastInter)>200){
        Serial.println("INTERSECTION");
        
        intersectionCount++;
        Serial.println(intersectionCount);
        lastInter = millis();
        
      }
    }
     plVal = lVal;
     pcVal = cVal;
     prVal = rVal;
  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  delay(200);
}

//Function to turn in a specified direction until a black line is hit
void turn(int dir){ 
  //drive a little past the intersection 
  //delay(50);
  analogWrite(leftSpeed, 148);
  analogWrite(rightSpeed, 128);
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  delay(400);
  
  if(dir == 1){//Begin turning counter-clockwise to ensure that center line sensor is not scaning the black tape
     analogWrite(leftSpeed, 128);
     analogWrite(rightSpeed, 100);
     digitalWrite(leftDirection, LOW);
     digitalWrite(rightDirection, HIGH);
     delay(550);
     if(cd ==0){ //Update direction robot is facing
      cd =3;
     }else{
      cd =cd-1;
     }
  }
  else{//Begin turning clockwise to ensure that center line sensor is not scaning the black tape
    analogWrite(leftSpeed, 128);
     analogWrite(rightSpeed, 100);
     digitalWrite(leftDirection, HIGH);
     digitalWrite(rightDirection, LOW);
     delay(300);
     cd = (cd+1)%4; //Update direction robot is facing
  }
   cVal = analogRead(cIRPin);
   while(cVal<thresh){ //Continue rotating in specified direction until the center line sensor reads the black tape value
      cVal = analogRead(cIRPin);
      analogWrite(leftSpeed, 100);
      analogWrite(rightSpeed, 100);
      if(dir == 1){
        digitalWrite(leftDirection, LOW);
        digitalWrite(rightDirection, HIGH);
      }else{
        digitalWrite(leftDirection, HIGH);
        digitalWrite(rightDirection, LOW);
      }
    
    }
    analogWrite(leftSpeed, 0);
    analogWrite(rightSpeed, 0);
}


void approach(){
  IRVal = analogRead(IRpin);
  while(IRVal <638)
  {
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, HIGH);
    IRVal = analogRead(IRpin);

    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);

     
    if((lVal < thresh) && (cVal > thresh) && (rVal < thresh)){ //SET MOTORS TO DRIVE FORWARD
      analogWrite(leftSpeed, 110);
      analogWrite(rightSpeed, 100);
    }else if((lVal > thresh) && (cVal < thresh) && (rVal < thresh)){//LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, 100);
      analogWrite(rightSpeed, 140);
    }else if((lVal < thresh) && (cVal < thresh) && (rVal > thresh)){//LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, 160);
      analogWrite(rightSpeed, 80);
    }

  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);

  //pickup

  turn(1);
  forward(1);
 
}
