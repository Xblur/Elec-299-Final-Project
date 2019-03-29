#include <Servo.h>

int type = 1;//initial position. left-0, middle-1, right-2

int cy = 0;
int cx = 0;
int cd = 0;
int x = 0;
int y = 0;
int d = 0;
int xIn = 5;
int yIn = 1;
int dIn = 1;
int delayTLeft = 400; //500 when half bat 400 when full bat
int delayTRight = 400; //500 when half bat 400 when full bat
int delayBeforeT = 250; // 350 when half 250 when full
int lSP = 200;
int rSP = 190;
int turnMod = 90; // 80 when half 90 when full
int resetTMod = 0; // 10 when full 0 when half;
int turnADPD = 150; // 150 when full 200 when half

int leftBumper = 1;
int rightBumper = 1;

int IRpin = A5;

int forceSensor = A4;
int forceReading = 1027;
int gripThresh = 400;
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
int thresh = 880;
unsigned long lastInter = 0;
int IRVal = 0;


//---------------------------------

int panPIN = 8;
int tiltPIN = 9;
int gripPIN = 10;

Servo pan, tilt, grip;
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

  pan.write(95);
  tilt.write(200);
  grip.write(75);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("BEGINING LOOP()");
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 5;
  yIn = 2;
  dIn = 1;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 5;
  yIn = 3;
  dIn = 1;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 5;
  yIn = 4;
  dIn = 1;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 5;
  yIn = 5;
  dIn = 1;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 5;
  yIn = 5;
  dIn = 0;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 4;
  yIn = 5;
  dIn = 0;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 3;
  yIn = 5;
  dIn = 0;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 2;
  yIn = 5;
  dIn = 0;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 5;
  dIn = 0;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 5;
  dIn = 3;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 4;
  dIn = 3;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 3;
  dIn = 3;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 2;
  dIn = 3;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  xIn = 1;
  yIn = 1;
  dIn = 3;
  GoToDice(xIn, yIn, dIn);
  approachDice();
  GoToBin(type);
  approachBin();
  exit(0);
}

//Function for robot to move to a specified ball's location
void GoToDice(int xIn1, int yIn1, int dIn1)
{
  x = xIn1;
  y = yIn1;
  d = dIn1;
  cx = type + 2;
  cy = 0;
  if (cy < y) { //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    forward(y - cy);
    cy = y;

  }
  if (cx != x) { //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if (cx < x) {
      turn(0);
      forward(x - cx);
      cx = x;
    }
    else if (cx > x) {
      turn(1);
      forward(cx - x);
      cx = x;
    }

  }
  if (cd != d) { //if the robot is not facing the ball, then turn towards the ball
    if (abs(cd - d) == 2) {
      turn(1);
      turn(1);
    }
    else if (cd - d == -3) {
      turn (1);
    }
    else if (cd - d == 3) {
      turn (0);
    }
    else if (cd - d > 0)
    {
      turn(1);
    }
    else if (cd - d < 0)
    {
      turn(0);
    }
  }
  analogWrite(leftSpeed, lSP);
  analogWrite(rightSpeed, rSP);

}

void GoToBin(int typeIn)//left-0, middle-1 or right-2
{
  y = 0;
  x = typeIn + 2; //gives location of bin depending on starting position

  Serial.println(cx);
  Serial.println(cy);

  if (cd == 2)
  {
    Serial.println("direction fix");
    //      forward(1);
    if (cx < x) {
      Serial.println("Left Turn");
      analogWrite(leftSpeed, lSP);
      analogWrite(rightSpeed, rSP);
      digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, HIGH);
      delay(turnADPD);
      //      int a = 40;
      //      lVal = analogRead(lIRPin);
      //      cVal = analogRead(cIRPin);
      //      rVal = analogRead(rIRPin);
      //      digitalWrite(leftDirection, HIGH);
      //      digitalWrite(rightDirection, HIGH);
      //      if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
      //        analogWrite(leftSpeed, lSP - turnMod + a);
      //        analogWrite(rightSpeed, rSP - turnMod + a);
      //      } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      //        analogWrite(leftSpeed, lSP - turnMod - 40 + a);
      //        analogWrite(rightSpeed, rSP - turnMod + 20 + a);
      //      } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      //        analogWrite(leftSpeed, lSP - turnMod + 20 + a);
      //        analogWrite(rightSpeed, rSP - turnMod - 40 + a);
      //      }
      //      delay(turnADPD);
      //         forward(1);
      turn(1);
      //         cx = cx + 1;
    }
    else if (cx > x) {
      Serial.println("Right Turn");
      analogWrite(leftSpeed, lSP);
      analogWrite(rightSpeed, rSP);
      digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, HIGH);
      delay(turnADPD);
      //      int a = 40;
      //      lVal = analogRead(lIRPin);
      //      cVal = analogRead(cIRPin);
      //      rVal = analogRead(rIRPin);
      //      digitalWrite(leftDirection, HIGH);
      //      digitalWrite(rightDirection, HIGH);
      //      if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
      //        analogWrite(leftSpeed, lSP - turnMod + a);
      //        analogWrite(rightSpeed, rSP - turnMod + a);
      //      } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      //        analogWrite(leftSpeed, lSP - turnMod - 40 + a);
      //        analogWrite(rightSpeed, rSP - turnMod + 20 + a);
      //      } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      //        analogWrite(leftSpeed, lSP - turnMod + 20 + a);
      //        analogWrite(rightSpeed, rSP - turnMod - 40 + a);
      //      }
      //      delay(turnADPD);
      //      forward(1);
      turn(0);
      //         cx = cx - 1;
    }
    //      else if (cx == x){
    //
    //      }
    //      exit(0);
  }

  if (cx != x) { //if the objective is not directly infront of the robot then turn in the direction of the ball and go forward until the ball is in front of the robot
    if (cx < x) {
      Serial.println("Going forward");
      forward(x - cx);
      cx = x;
      turn(0);//turn right

      Serial.println("turning");

    }
    else if (cx > x) {
      Serial.println("Going forward");

      forward(cx - x);
      cx = x;
      turn(1);//turn left
    }

  }
  if (cy != y) { //if current y coordinate is less than the objective y then move forward until current y is equal to that of the objective
    if (yIn == 5 && dIn == 0) {
      cy = cy - 1;
    }
    forward(cy - y - 1);
    cy = y;
  }
  analogWrite(leftSpeed, lSP);
  analogWrite(rightSpeed, rSP);

  //drop into bin function
}




//Function to move robot forward a specified number of intersections
void forward(int numOfIntersections) {
  int intersectionCount = 0;
  digitalWrite(leftDirection, HIGH); //Set left motor direction to forward
  digitalWrite(rightDirection, HIGH); //Set right motor direction to forward

  //While not at the desired intersection, keep moving forward
  while (intersectionCount < numOfIntersections) {
    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);

    /*  Serial.print(lVal);
      Serial.print(" || ");
      Serial.print(cVal);
      Serial.print(" || ");
      Serial.println(rVal); */
    if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
      analogWrite(leftSpeed, lSP);
      analogWrite(rightSpeed, rSP);
    } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP - 40);
      analogWrite(rightSpeed, rSP + 20);
    } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP + 20);
      analogWrite(rightSpeed, rSP - 40);
    }

    if ((lVal > thresh) && (cVal > thresh) && (rVal > thresh) && (plVal > thresh) && (pcVal > thresh) && (prVal > thresh)) { //At an intersection. Increment intersection counter
      if ((millis() - lastInter) > 170) {
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
  //  analogWrite(leftSpeed, 0);
  //  analogWrite(rightSpeed, 0);
  //  delay(100);
}

//Function to turn in a specified direction until a black line is hit
void turn(int dir) {
  //drive a little past the intersection
  //delay(50);
  analogWrite(leftSpeed, lSP - turnMod);
  analogWrite(rightSpeed, rSP - turnMod);
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  delay(delayBeforeT);

  if (dir == 1) { //Begin turning counter-clockwise to ensure that center line sensor is not scaning the black tape
    analogWrite(leftSpeed, lSP - turnMod);
    analogWrite(rightSpeed, rSP - turnMod);
    digitalWrite(leftDirection, LOW);
    digitalWrite(rightDirection, HIGH);
    delay(delayTLeft);
    if (cd == 0) { //Update direction robot is facing
      cd = 3;
    } else {
      cd = cd - 1;
    }
  }
  else { //Begin turning clockwise to ensure that center line sensor is not scaning the black tape
    analogWrite(leftSpeed, lSP - turnMod);
    analogWrite(rightSpeed, rSP - turnMod);
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, LOW);
    delay(delayTRight);
    cd = (cd + 1) % 4; //Update direction robot is facing
  }
  cVal = analogRead(cIRPin);
  while (cVal < thresh) { //Continue rotating in specified direction until the center line sensor reads the black tape value
    cVal = analogRead(cIRPin);
    analogWrite(leftSpeed, lSP - turnMod);
    analogWrite(rightSpeed, rSP - turnMod);
    if (dir == 1) {
      digitalWrite(leftDirection, LOW);
      digitalWrite(rightDirection, HIGH);
    } else {
      digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, LOW);
    }

  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

//void turnB(int dir){
//  //drive a little past the intersection
//  //delay(50);
//  analogWrite(leftSpeed, lSP-turnMod);
//  analogWrite(rightSpeed, rSP-turnMod);
//  digitalWrite(leftDirection, HIGH);
//  digitalWrite(rightDirection, HIGH);
//  delay(delayBeforeT);
//
//  if(dir == 1){//Begin turning counter-clockwise to ensure that center line sensor is not scaning the black tape
//     analogWrite(leftSpeed, lSP-turnMod);
//     analogWrite(rightSpeed, rSP-turnMod);
//     digitalWrite(leftDirection, LOW);
//     digitalWrite(rightDirection, HIGH);
//     delay(delayTLeft+200);
//     if(cd ==0){ //Update direction robot is facing
//      cd =3;
//     }else{
//      cd =cd-1;
//     }
//  }
//  else{//Begin turning clockwise to ensure that center line sensor is not scaning the black tape
//     analogWrite(leftSpeed, lSP-turnMod);
//     analogWrite(rightSpeed, rSP-turnMod);
//     digitalWrite(leftDirection, HIGH);
//     digitalWrite(rightDirection, LOW);
//     delay(delayTRight+200);
//     cd = (cd+1)%4; //Update direction robot is facing
//  }
//   cVal = analogRead(cIRPin);
//   while(cVal<thresh){ //Continue rotating in specified direction until the center line sensor reads the black tape value
//      cVal = analogRead(cIRPin);
//      analogWrite(leftSpeed, lSP-turnMod);
//      analogWrite(rightSpeed, rSP-turnMod);
//      if(dir == 1){
//        digitalWrite(leftDirection, LOW);
//        digitalWrite(rightDirection, HIGH);
//      }else{
//        digitalWrite(leftDirection, HIGH);
//        digitalWrite(rightDirection, LOW);
//      }
//
//    }
//    analogWrite(leftSpeed, 0);
//    analogWrite(rightSpeed, 0);
//}

void turnWithDice()
{
  analogWrite(leftSpeed, lSP - turnMod);
  analogWrite(rightSpeed, rSP - turnMod);
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, LOW);
  delay(700);

  rVal = analogRead(rIRPin);
  while (rVal < thresh) { //Continue rotating in specified direction until the center line sensor reads the black tape value
    rVal = analogRead(rIRPin);
    analogWrite(leftSpeed, lSP - turnMod);
    analogWrite(rightSpeed, rSP - turnMod);
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, LOW);


  }

  if (cd == 0) { //Update direction robot is facing
    cd = 2;
  }
  else if (cd == 1) {
    cd = 3;
  }
  else if (cd == 2)
  {
    cd = 0;
  }
  else //cd == 3
  {
    cd = 1;
  }
  //    int a=30;
  //    digitalWrite(leftDirection, HIGH);
  //    digitalWrite(rightDirection,HIGH);
  //    if((lVal < thresh) && (cVal > thresh) && (rVal < thresh)){ //SET MOTORS TO DRIVE FORWARD
  //      analogWrite(leftSpeed, lSP-turnMod+a);
  //      analogWrite(rightSpeed, rSP-turnMod+a);
  //    }else if((lVal > thresh) && (cVal < thresh) && (rVal < thresh)){//LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
  //      analogWrite(leftSpeed, lSP-turnMod-40+a);
  //      analogWrite(rightSpeed, rSP-turnMod+20+a);
  //    }else if((lVal < thresh) && (cVal < thresh) && (rVal > thresh)){//LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
  //      analogWrite(leftSpeed, lSP-turnMod+20+a);
  //      analogWrite(rightSpeed, rSP-turnMod-40+a);
  //    }
  //    delay(600);
  if (cd == 2) {
    forward(1);
  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  delay(50);
  //  Serial.println(cd);
  //    exit(0);

}

void closeGrip() {
  int i = 75;
  int x = 200;
  while (x > 65) {
    tilt.write(x);
    x--;
    delay(10);
  }

  forceReading = analogRead(forceSensor);
  while (forceReading > gripThresh) {
    forceReading = analogRead(forceSensor);
    Serial.println(forceReading);
    i++;
    int j = i % 200;
    grip.write(j);
    delay(20);
  }
  tilt.write(200);
  Serial.println("OBJECT GRABBED");
}

void dropDice() {
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  int x = 200;
  while (x > 70) {
    tilt.write(x);
    x--;
    if (x == 80) {
      grip.write(75);
      Serial.println("Dice Dropped");
    }
  }
}


void approachDice() {
  if (cd == 3)
  {
    cx--;
  }
  else if (cd == 0)
  {
    cy++;
  }
  else if (cd == 1)
  {
    cx++;
  }

  IRVal = analogRead(IRpin);
  leftBumper = digitalRead(lBump);
  rightBumper = digitalRead(rBump);
  while (leftBumper && rightBumper)
  {
    leftBumper = digitalRead(lBump);
    rightBumper = digitalRead(rBump);
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, HIGH);
    IRVal = analogRead(IRpin);

    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);


    if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
      analogWrite(leftSpeed, lSP - turnMod);
      analogWrite(rightSpeed, rSP - turnMod);
    } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP - turnMod);
      analogWrite(rightSpeed, rSP - turnMod + 40);
    } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP - turnMod + 40);
      analogWrite(rightSpeed, rSP - turnMod);
    }

  }
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, lSP - turnMod);
  analogWrite(rightSpeed, rSP - turnMod);
  delay(300);
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  closeGrip();

  turnWithDice();

  //  if((lVal < thresh) && (cVal > thresh) && (rVal < thresh)){ //SET MOTORS TO DRIVE FORWARD
  //      analogWrite(leftSpeed, 110);
  //      analogWrite(rightSpeed, 100);
  //    }else if((lVal > thresh) && (cVal < thresh) && (rVal < thresh)){//LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
  //      analogWrite(leftSpeed, 160);
  //      analogWrite(rightSpeed, 80);
  //    }else if((lVal < thresh) && (cVal < thresh) && (rVal > thresh)){//LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
  //      analogWrite(leftSpeed, 100);
  //      analogWrite(rightSpeed, 140);
  //    }
  //  delay(1000);

}

void approachBin() {
  IRVal = analogRead(IRpin);
  leftBumper = digitalRead(lBump);
  rightBumper = digitalRead(rBump);
  while (leftBumper && rightBumper)
  {
    leftBumper = digitalRead(lBump);
    rightBumper = digitalRead(rBump);
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, HIGH);
    IRVal = analogRead(IRpin);
    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);


    if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
      analogWrite(leftSpeed, lSP);
      analogWrite(rightSpeed, rSP);
    } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP - 80);
      analogWrite(rightSpeed, rSP - 40);
    } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
      analogWrite(leftSpeed, lSP - 40);
      analogWrite(rightSpeed, rSP - 80);
    }
  }

  dropDice();
  Serial.println("dice dropped");
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, lSP - 40);
  analogWrite(rightSpeed, rSP - 40);
  delay(300);
  Serial.println("went back");
  analogWrite(leftSpeed, lSP - turnMod - resetTMod);
  analogWrite(rightSpeed, rSP - turnMod - resetTMod);
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, LOW);
  delay(1000);
  cVal = analogRead(rIRPin);
  while (cVal < thresh) { //Continue rotating in specified direction until the center line sensor reads the black tape value
    cVal = analogRead(cIRPin);
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, LOW);
    analogWrite(leftSpeed, lSP - turnMod - resetTMod);
    analogWrite(rightSpeed, rSP - turnMod - resetTMod);
  }
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, HIGH);
  analogWrite(leftSpeed, lSP);
  analogWrite(rightSpeed, rSP);
  delay(50);
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  cd = 0;
  delay(50);
  tilt.write(200);
  delay(50);
}
