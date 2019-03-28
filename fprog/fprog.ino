#include <Servo.h>

int type =0;//initial position. left-0, middle-1, right-2

int cy = 0;
int cx = 2;
int cd = 0;

int leftBumper=1;
int rightBumper=1;

int IRpin = A5;

int forceSensor = A4;
int forceReading = 1027;
int gripThresh = 300;
int fIRThresh = 800;


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
//--------------------------------
int lBump = 0;
int rBump = 11;

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

  pinMode(lBump, INPUT);
  pinMode(rBump, INPUT);

  pinMode(IRpin, INPUT);
  pinMode(forceSensor, INPUT);

  pan.write(90);
  tilt.write(200);
  grip.write(140);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("BEGINING LOOP()");
  GoToDice(1,5,3);
  approach();
  GoToBin(0);
  delay(10000);
}

//Function for robot to move to a specified ball's location
void GoToDice(int x, int y, int d)
{
  if (cy<y){ //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    Serial.println("Starting to in the Y direction. CY = " + (String)cy);
    forward(y-cy);
    cy = y;
    Serial.println("finished moving along y. CY = " + (String)cy);
  }
  if(cx!=x){ //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if(cx<x){
      Serial.println("Starting to in the x direction. CX = " + (String)cx);
      turn(0);
      forward(x-cx);
      cx = x;
      Serial.println("finished moving along y. CX = " + (String)cx);
    }
    else if(cx>x){
      Serial.println("Starting to in the x direction. CX = " + (String)cx);
      turn(1);
      forward(cx-x);
      cx = x;
      Serial.println("finished moving along y. CX = " + (String)cx);
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
    Serial.println("Calling approch function");
    approach();
      
}

void GoToBin(int type)//left-0, middle-1 or right-2
{
  int y = 0;
  int x =type+2;//gives location of bin depending on starting position
  
  Serial.println("Intial CX value at goToBin. CX = " + (String)cx);
  Serial.println("Intial CY value at goToBin. CY = " + (String)cy);
  
    

  if(cx!=x){ //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if(cx<x){
      Serial.println("Moving along x axis.");
      forward(x-cx);
      cx = x;
      turn(0);//turn right
      
      Serial.println("Final CX value in goToBin function. CX = " + (String)cx);
     
    }
    else if(cx>x){
      Serial.println("Moving along x axis.");

      forward(cx-x);
      cx = x;
      turn(1);//turn left

      Serial.println("Final CX value in goToBin function. CX = " + (String)cx);
    }
  
  }
  if (cy!=y){ //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    Serial.println("Moving along y axis.");
    forward(cy-y);
    cy = y;
    Serial.println("Final CY value in goToBin function. CY = " + (String)cy);
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

void turnWithDice()
{
     if(cd ==0){ //Update direction robot is facing
      cd =2;
     }
     else if(cd==1){
        cd = 3;
      }
     else if(cd ==2)
     {
      cd = 0;
     }
     else //cd == 3
     {
      cd = 1;
     }
     
     
     analogWrite(leftSpeed, 128);
     analogWrite(rightSpeed, 100);
     digitalWrite(leftDirection, HIGH);
     digitalWrite(rightDirection, LOW);
     delay(550);
     
     cVal = analogRead(cIRPin);
     while(cVal<thresh){ //Continue rotating in specified direction until the center line sensor reads the black tape value
      cVal = analogRead(cIRPin);
      analogWrite(leftSpeed, 100);
      analogWrite(rightSpeed, 100);
      digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, LOW);
    }
    
    analogWrite(leftSpeed, 0);
    analogWrite(rightSpeed, 0);
    delay(100);
    //forward(1);
 }

void closeGrip(){
  int i = 90;
  int x = 200;
  while(x>70){
    tilt.write(x);
    x--;
    delay(20);
  }
  
  forceReading = analogRead(forceSensor);
  while(forceReading > gripThresh){
    forceReading = analogRead(forceSensor);
    //Serial.println(forceReading);
    i++;
    grip.write(i);
    delay(50);
  }
  Serial.println("OBJECT GRABBED");
}


void approach(){
  Serial.println("Starting approach function.");
  if(cd == 3)
  {
    cx--;
  }
  if(cd == 0)
  {
    cy++;
  }
  if(cd == 1)
  {
    cx++;
  }

  Serial.println("CX = " + (String)cx);
  Serial.println("CY = " + (String)cy);
  
  IRVal = analogRead(IRpin);
  leftBumper = digitalRead(lBump);
  rightBumper = digitalRead(rBump);
  while(leftBumper && rightBumper)
  {
    leftBumper = digitalRead(lBump);
    rightBumper = digitalRead(rBump);
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
 
  delay(100);
  
 digitalWrite(leftDirection, LOW);
 digitalWrite(rightDirection, LOW);
  
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
  delay(400);
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
 closeGrip();
 

 turnWithDice();
 

  delay(500);
  Serial.println("Done with Approach.");
 
}
