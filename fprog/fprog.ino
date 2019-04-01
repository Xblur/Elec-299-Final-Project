#include <Servo.h>

int type = 2;//initial position. left-0, middle-1, right-2

int cy = 0;
int cx = 0;
int cd = 0;
int x = 0;
int y = 0;
int d = 0;
int xIn;
int yIn;
int dIn;
int delayTLeft = 600; //500 when half bat 400 when full bat
int delayTRight = 600; //500 when half bat 400 when full bat
int delayBeforeT = 200; // 350 when half 250 when full
int lSP = 210;
int rSP = 200;
int turnMod = 80; // 80 when half 90 when full
int resetTMod = 10; // subtracts from turn speed after drop dice. 10 when full -10 when half;
int turnADPD = 300; // 150 when full 200 when half
//int intersectMilli = 80; // 130 full 180 low

int leftBumper = 1;
int rightBumper = 1;

int IRpin = A5;

int forceSensor = A4;
int forceReading = 1027;
int gripThresh = 400;
int fIRThresh = 300;


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
  xIn = 4;
  yIn = 5;
  dIn = 0;
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
  xIn = 5;
  yIn = 5;
  dIn = 0;
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
  int a = 0;
  Serial.println(cx);
  Serial.println(cy);

  if (cd == 2)
  {

    Serial.println("direction fix");
    if (cx < x) {
      forward(1);
      turn(1);
      
    }
    else if (cx > x) {
      
      forward(1);
      turn(0);
    }
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
  analogWrite(leftSpeed, lSP - 60);
  analogWrite(rightSpeed, rSP - 60);

  //drop into bin function
}

void followLineF(int speedMod)//speedMod adds on to speed;
{
  lVal = analogRead(lIRPin);
  cVal = analogRead(cIRPin);
  rVal = analogRead(rIRPin);
  IRVal = analogRead(IRpin);
  digitalWrite(leftDirection, HIGH); //Set left motor direction to forward
  digitalWrite(rightDirection, HIGH); //Set right motor direction to forward

  Serial.print(lVal);
  Serial.print(" || ");
  Serial.print(cVal);
  Serial.print(" || ");
  Serial.println(rVal);
  if (IRVal > fIRThresh) {
    analogWrite(leftSpeed, 0);
    analogWrite(rightSpeed, 0);
  } else if ((lVal < thresh) && (cVal > thresh) && (rVal < thresh)) { //SET MOTORS TO DRIVE FORWARD
    analogWrite(leftSpeed, lSP + speedMod);
    analogWrite(rightSpeed, rSP + speedMod);
  } else if ((lVal > thresh) && (cVal < thresh) && (rVal < thresh)) { //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR (CALIBRATE)
    analogWrite(leftSpeed, lSP - 80 + speedMod);
    analogWrite(rightSpeed, rSP + speedMod);
  } else if ((lVal < thresh) && (cVal < thresh) && (rVal > thresh)) { //LEANING INTO THE LEFT...SPEED UP RIGHT MOTOR (CALIBRATE)
    analogWrite(leftSpeed, lSP + speedMod);
    analogWrite(rightSpeed, rSP - 80 + speedMod);
  }
  else {
    analogWrite(leftSpeed, lSP - 30 + speedMod);
    analogWrite(rightSpeed, rSP - 30 + speedMod);
  }
}


//Function to move robot forward a specified number of intersections
void forward(int numOfIntersections) {
  int intersectionCount = 0;
  digitalWrite(leftDirection, HIGH); //Set left motor direction to forward
  digitalWrite(rightDirection, HIGH); //Set right motor direction to forward

  //While not at the desired intersection, keep moving forward
  while (intersectionCount < numOfIntersections) {
    followLineF(0);
    int count = 0;
    while ( count < 4) {
      if ((lVal > thresh) && (cVal > thresh) && (rVal > thresh) && (plVal > thresh) && (pcVal > thresh) && (prVal > thresh)) { //At an intersection. Increment intersection counter
        //      if ((millis() - lastInter) > intersectMilli) {
        //        Serial.println("INTERSECTION");
        //
        //        intersectionCount++;
        //        Serial.println(intersectionCount);
        //        lastInter = millis();
        //      }
        lVal = analogRead(lIRPin);
        cVal = analogRead(cIRPin);
        rVal = analogRead(rIRPin);
        int i = 0;
        int tCVal = 0;
        int tLVal = 0;
        int tRVal = 0;
        while ( i < 10) {
          followLineF(0);
          i++;
          lVal = analogRead(lIRPin);
          cVal = analogRead(cIRPin);
          rVal = analogRead(rIRPin);
          tCVal = tCVal + cVal;
          tRVal = tRVal + rVal;
          tLVal = tLVal + lVal;
        }
        plVal = (tLVal / 10);
        pcVal = (tCVal / 10);
        prVal = (tRVal / 10);
      }
      count++;
    }
     if ((millis() - lastInter) > 200) {
        Serial.println("INTERSECTION");
        

        intersectionCount++;
        Serial.println(intersectionCount);
        lastInter = millis();
        

      }
      
//    intersectionCount++;
//    Serial.println("Intersection: " + (String)intersectionCount );
  }
  //  analogWrite(leftSpeed, 0);
  //  analogWrite(rightSpeed, 0);
  //  delay(100);
}

//Function to turn in a specified direction until a black line is hit
void turn(int dir) {
  //drive a little past the intersection
  delay(50);
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
    cVal = analogRead(cIRPin);
    pcVal = 0;
    while (cVal < thresh && pcVal < thresh) {
      pcVal = cVal;
      delay(20);
      cVal = analogRead(cIRPin);
    }
    analogWrite(leftSpeed, 0);
    analogWrite(rightSpeed, 0);
    delay(100);
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
    cVal = analogRead(cIRPin);
    pcVal = 0;
    while (cVal < thresh && pcVal < thresh) {
      pcVal = cVal;
      delay(20);
      cVal = analogRead(cIRPin);
    }
    analogWrite(leftSpeed, 0);
    analogWrite(rightSpeed, 0);
    delay(100);
    cd = (cd + 1) % 4; //Update direction robot is facing
  }

  
  Serial.println("sweep");
  lVal = analogRead(lIRPin);
  cVal = analogRead(cIRPin);
  rVal = analogRead(rIRPin);
  delay(100);
  if (cVal < thresh) {
    sweep(dir);
  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

void sweep(int lastSide) {
  if (lastSide == 0)//left
  {
    digitalWrite(leftDirection, LOW);
    digitalWrite(rightDirection, HIGH);
    analogWrite(leftSpeed, lSP - turnMod - 20);
    analogWrite(rightSpeed, rSP - turnMod - 20);
    cVal = analogRead(cIRPin);
    while (cVal < thresh) { //Continue rotating in specified direction until the center line sensor reads the black tape value
      cVal = analogRead(cIRPin);
    }
  }
  else {
    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, LOW);
    analogWrite(leftSpeed, lSP - turnMod - 20);
    analogWrite(rightSpeed, rSP - turnMod - 20);
    cVal = analogRead(cIRPin);
    while (cVal < thresh) { //Continue rotating in specified direction until the center line sensor reads the black tape value
      cVal = analogRead(cIRPin);
    }
  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  delay(50);
}

void turnWithDice()
{
  followLineF(0);
  delay(300);
  turn(1);
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
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  delay(50);
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
//    Serial.println(forceReading);
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
    fIRThresh = 600;
    followLineF(0);
  }
  fIRThresh = 300;
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, lSP - turnMod);
  analogWrite(rightSpeed, rSP - turnMod);
  delay(300);
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  closeGrip();

  turnWithDice();
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
    fIRThresh = 600;
    followLineF(0);
  }
  fIRThresh = 300;
  dropDice();
  Serial.println("dice dropped");
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, lSP);
  analogWrite(rightSpeed, rSP);
  delay(600);
  Serial.println("went back");

  
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  turn(1);
  //followLineF(0);
  delay(300);
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
  cd = 0;
  delay(100);
  tilt.write(200);
  delay(50);
}
