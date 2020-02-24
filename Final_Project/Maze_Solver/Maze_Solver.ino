#include <NewPing.h>
#include <AFMotor.h>
#define WALL_RANGE 15
#define MAX_SPEED 255
#define MAX_DISTANCE 200
#define FRONT 0
#define LEFT 1
#define RIGHT 2
#define BACK 3
#define STOP 4
#define BASE_SPEED 80
#define OFFSET 6
//--MOTORS--//

AF_DCMotor leftUp(1,MOTOR12_64KHZ);
AF_DCMotor leftDown(2,MOTOR12_64KHZ);
AF_DCMotor rightUp(4,MOTOR34_64KHZ);
AF_DCMotor rightDown(3,MOTOR34_64KHZ);
//--SONARS--//
NewPing sonarR(A5,A4,MAX_DISTANCE);
NewPing sonarL(A3,A2,MAX_DISTANCE);
NewPing sonarF(A1,A0,MAX_DISTANCE);

//---
int lRead, rRead, fRead;
int lastLread = 0, lastRread = 0, lastFread = 0;
bool stillLeft = false;
bool stillRight = false;
bool frontWall = false, rightWall = false, leftWall = false;
bool turn = false;
float P = 0.7 ;
float D = 0.5 ;
float I = 0.4 ;
float oldErrorP =0  ,errorI = 0,errorP = 0,errorD = 0;
float totalError  =0 ;
float RMS, LMS; 
const long readDelay = 100;
long lastReading = 0;
void setup() {
  leftUp.run(RELEASE);
  leftDown.run(RELEASE);
 rightUp.run(RELEASE);
  rightDown.run(RELEASE);
  Serial.begin(9600);
}

void loop() {
  readSonars();
  detectWalls();
    if(turn == false){
      Serial.println("normalPID");
      normalPID();
    }
    else if(stillLeft){
      PID(LEFT);
    }
    else if(stillRight){
      PID(RIGHT);
    }
     if (leftWall == true && rightWall == false && frontWall == true ) {
    PID(RIGHT) ;
    if ( turn == false ) {
      
      //errorI = 0;
      turn = true ;
      stillRight = true ;
    }
  }
   if (leftWall == false && rightWall == true && frontWall == true ) {
    PID(LEFT) ;
    if ( turn == false ) {
     
      //errorI = 0;
      turn = true ;
      stillLeft = true ;
    }
  }
   if ( lRead == 0 || lRead > MAX_DISTANCE / 2 && rRead == 0 || rRead > MAX_DISTANCE / 2 && fRead == 0 || fRead > MAX_DISTANCE / 2) {
    moveDirection(STOP);                                                                            
  }
  Serial.print(" Left : ");
  Serial.print(lRead);
  Serial.print(" cm ");
  Serial.print(" Right : ");
  Serial.print(rRead);
  Serial.print(" cm ");
  Serial.print(" Front : ");
  Serial.print(fRead);
  Serial.println(" cm ");
  Serial.print("error=");
  Serial.println(totalError);
 }

  
void readSonars(){
  lRead = sonarL.ping_cm();
  rRead = sonarR.ping_cm();
  fRead = sonarF.ping_cm();
  lRead = (lRead + lastLread)/ 2.0;
  rRead = (rRead + lastRread)/ 2.0;
  fRead = (fRead + lastFread)/ 2.0;
  if(rRead == 0 ) rRead = 200;
  if(lRead == 0 ) lRead = 200;
  if(fRead == 0 ) fRead = 200;
  
   lastLread = lRead;
   lastRread = rRead;
   lastFread = fRead;
   Serial.println(rRead);
}

void detectWalls(){
  if(lRead < WALL_RANGE && lRead != 0){
    leftWall = true;  
  }
  else{
    leftWall = false;
  }
  if(rRead < WALL_RANGE && rRead != 0){
    rightWall = true;  
    //Serial.println("WALL IN RIGHT");
  }
  else{
    rightWall = false;
  }
  if(fRead < WALL_RANGE && fRead != 0 ){
    frontWall = true;  
   // Serial.println("WALL IN FRONT");
  }
  else{
    frontWall = false;
  }
  Serial.print(leftWall);
  Serial.print(" ");
    Serial.print(frontWall);
    Serial.print(" ");
    Serial.print(rightWall);
  Serial.println(" ");
}

void moveDirection(int dir){
  
  
  switch(dir){
    case FRONT :{
      leftUp.run(FORWARD);
      leftDown.run(FORWARD);
      rightUp.run(FORWARD);
      rightDown.run(FORWARD);
      break;
  }
    case RIGHT :{
      leftUp.run(FORWARD);
      leftDown.run(FORWARD);
      
      rightUp.run(RELEASE);
      
      rightDown.run(RELEASE);
     // Serial.println("RIGHT");
      break;
  }
    case LEFT :{
      leftUp.run(RELEASE);
      leftDown.run(RELEASE);
      rightUp.run(FORWARD);
      rightDown.run(FORWARD);
     // Serial.println("LEFT");
      break;
  }
    case BACK :{
      leftUp.run(BACKWARD);
      leftDown.run(BACKWARD);
      rightUp.run(BACKWARD);
      rightDown.run(BACKWARD);
      break;
  }
  case STOP:{
    leftUp.run(RELEASE);
      leftDown.run(RELEASE);
      rightUp.run(RELEASE);
      rightDown.run(RELEASE);
      break;
  }
    
 }
}

void normalPID(){
    errorP = lRead   - rRead   ;
    errorD = errorP - oldErrorP;
    errorI = 0.7 * errorI + errorP;
    totalError = P* errorP + I* errorI + D* errorD;
    oldErrorP = errorP ;

  RMS = BASE_SPEED + totalError ;
  LMS = BASE_SPEED - totalError ;
  if(RMS < BASE_SPEED - 20){
    //means we have to turn RIGHT
    RMS =  map(RMS , 0 , BASE_SPEED - 20, 0, 255);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(RIGHT);
  }
  else if(LMS < BASE_SPEED - 20){
    //means we have to turn LEFT
    LMS =  map(LMS , 0 , BASE_SPEED - 20, 0, 255);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(LEFT);
  }
  else
  {
    Serial.println("GOES FRONT");
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(FRONT);
  }
}
void PID(int dir){
    if(dir == LEFT)
      errorP = lRead   - rRead  - OFFSET;
    else
      errorP = lRead   - rRead   + OFFSET;
      
    errorD = errorP - oldErrorP;
    errorI = 0.7 * errorI + errorP ;
    totalError = P * errorP + D * errorD + I * errorI ;
    oldErrorP = errorP ;
    RMS = BASE_SPEED + totalError;
    LMS = BASE_SPEED - totalError ;
   if(RMS < BASE_SPEED - 20){
    //means we have to turn RIGHT
    RMS =  map(RMS , 0 , BASE_SPEED - 20, 0, 255);
    Serial.println("RIGHT TURN LMS: ");
    Serial.print(LMS);
    Serial.print(" RMS:");
    Serial.println(RMS);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(RIGHT);
  }
  else if(LMS < BASE_SPEED - 20){
    //means we have to turn LEFT
    LMS =  map(LMS , 0 , BASE_SPEED - 20, 0, 255);
    Serial.println("LEFT TURN LMS: ");
    Serial.print(LMS);
    Serial.print(" RMS:");
    Serial.println(RMS);
    
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(LEFT);
  }
  else
  {
    Serial.print("FRONT :");
    Serial.print(LMS);
    Serial.print(" RMS:");
    Serial.println(RMS);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(FRONT);
  }
  }
