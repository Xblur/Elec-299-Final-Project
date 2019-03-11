void setup() {
  // put your setup code here, to run once:
  

}

void loop() {
  // put your main code here, to run repeatedly:

}


void GoToBall(int x, int y, int d)
{
  if (cy<y){
    forward(y-cy);
    cy = y;
 
  }
  if(cx!=x){
    if(cx<x){
      turn(0);
      forward(x-cx);
    }
    else if(cx>x){
      turn(1);
      forward(cx-x);
    }
  
  }
  if(cd!=d){
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
      
}
void forward(int numOfIntersections){
  int intersectionCount = 0;
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  while(intersectionCount < numOfIntersections){
    lVal = analogRead(lIRPin);
    cVal = analogRead(cIRPin);
    rVal = analogRead(rIRPin);

       Serial.print(lVal);
      Serial.print(" || ");
      Serial.print(cVal);
       Serial.print(" || ");
       Serial.println(rVal);
    if((lVal < thresh) && (cVal > thresh) && (rVal < thresh)){
      //GO STRAIGHT
      analogWrite(leftSpeed, 110);
      analogWrite(rightSpeed, 100);
    }else if((lVal > thresh) && (cVal < thresh) && (rVal < thresh)){
      //LEANING INTO THE RIGHT...SPEED UP RIGHT MOTOR
      analogWrite(leftSpeed, 100);
      analogWrite(rightSpeed, 140);
    }else if((lVal < thresh) && (cVal < thresh) && (rVal > thresh)){
      //LEANING LEFT...SPEED UP LEFT MOTOR
      analogWrite(leftSpeed, 160);
      analogWrite(rightSpeed, 80);
    }

    if((lVal > thresh) && (cVal > thresh) && (rVal > thresh) && (plVal > thresh) && (pcVal > thresh) && (prVal > thresh)){
      //Serial.print("INTERSECTION");
      intersectionCount++;
      delay(100);
    }
     plVal = lVal;
     pcVal = cVal;
     prVal = rVal;
  }
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

void turn(int dir){//dir = 1 is left, dir = 0 is right
  
  delay(50);
  analogWrite(leftSpeed, 128);
  analogWrite(rightSpeed, 100);
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  delay(700);
  if(dir == 1){
     analogWrite(leftSpeed, 128);
     analogWrite(rightSpeed, 100);
     digitalWrite(leftDirection, LOW);
     digitalWrite(rightDirection, HIGH);
     delay(100);
  }
  else{
    analogWrite(leftSpeed, 128);
     analogWrite(rightSpeed, 100);
     digitalWrite(leftDirection, HIGH);
     digitalWrite(rightDirection, LOW);
     delay(100);
  }
   cVal = analogRead(cIRPin);
   while(cVal<thresh){
      cVal = analogRead(cIRPin);
      analogWrite(leftSpeed, 128);
      analogWrite(rightSpeed, 100);
      if(dir = 1){
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
