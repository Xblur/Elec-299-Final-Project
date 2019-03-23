//StartFunction
// librarys
#include <QSerial.h>
#include <Servo.h>

//initialization
QSerial myIRserial;
Servo pan, tilt, grip;

// pin assignment
int txpin = -1;
int rxpin = 12;
int panPin = 8;
int tiltPin = 9;
int gripPin = 10;

//array assignment
int array1[5][3] = {{0,1,3},
                    {1,6,0},
                    {6,5,1},
                    {0,2,3},
                    {0,4,3}}; 
int array2[5][3] = {{0,3,3},
                    {3,6,0},
                    {6,3,1},
                    {2,6,0},
                    {4,6,0}}; 
int array3[5][3] = {{0,5,3},
                    {5,6,0},
                    {6,1,1},
                    {6,2,1},
                    {6,4,1}};  
int home1[3]= {2,-1, 0};
int home2[3]= {3,-1, 0};
int home3[3]= {4,-1, 0};

//transmitted characters
char Start1 = '0';
char Start2 = '1';
char Start3 = '2';

//robots variables for locations
//still need to change to allow for direction variable
int botsPickUpLOC[5][3];
int botsHome[3];
int botsLoc[3];



void GetIR(){
    myIRserial.attach(rxpin, txpin);
    Serial.begin(9600);
    char val = -1;

    while(true){
      val = myIRserial.receive(200);
      Serial.println(val);
    
         if(val == Start1) {
                Serial.println("Start Location 1");
                for (int i = 0; i < 5; i++){
                      for (int j = 0; j <3; j++){
                             botsPickUpLOC[i][j] = array1[i][j];
                              Serial.println(botsPickUpLOC[i][j]);
                       }
                  }
                  
                for(int i = 0; i < 3; i++){
                      botsHome[i] = home1[i];
                      }   
                for(int i = 0; i < 3; i++){
                      botsLoc[i] = botsHome[i];
                      Serial.println(botsLoc[i]);
                  }
          break;
        }
    
        else if(val == Start2) {
                Serial.println("Start Location 2");
                for (int i = 0; i < 5; i++){
                      for (int j = 0; j <3; j++){
                            botsPickUpLOC[i][j] = array2[i][j];
                            Serial.println(botsPickUpLOC[i][j]);
                        }
                  }
                for(int i = 0; i < 3; i++){
                      botsHome[i] = home2[i];
                     }   
                for(int i = 0; i < 3; i++){
                      botsLoc[i] = botsHome[i];
                      Serial.println(botsLoc[i]);
                  }
          break;
        }
      
        else if(val == Start3) {
                Serial.println("Start Location 3");
                for (int i = 0; i < 5; i++){
                      for (int j = 0; j <3; j++){
                            botsPickUpLOC[i][j] = array3[i][j];
                            Serial.println(botsPickUpLOC[i][j]);
                      }
                }
                for(int i = 0; i < 3; i++){
                     botsHome[i] = home3[i];
                     }   
                for(int i = 0; i < 3; i++){
                      botsLoc[i] = botsHome[i];
                      Serial.println(botsLoc[i]);
                  }
          break;
      }
      
      else {
      Serial.println("Invalid transmitted value.");
      }  
    } 
}

void  SetArm(){
    pan.attach(panPin);
    tilt.attach(tiltPin);
    grip.attach(gripPin);

    pan.write(90);    
    tilt.write(160);  
    grip.write(40);  
  }


void setup(){
  GetIR();
  SetArm();
}

void loop(){
  
}
