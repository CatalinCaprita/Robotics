#include <NewPing.h>
#include <AFMotor.h>
#define WALL_RANGE 20     //The Distance for which, if a reading of a sonar is smaller, the Bot considers a wall in that direction
#define FRONT_RANGE 17    //The Distance for which, if a reading of a sonar is smaller, the Bot considers a wall in front
#define MAX_SPEED 255     //Maximum speed for a motor
#define MAX_DISTANCE 200  //Maximum distance to consider for a sonar reading. If the distance measured is greater, the function ping_cm returns 0 
#define FRONT 0           //Constants that define the direction of the Bot
#define LEFT 1
#define RIGHT 2
#define BACK 3
#define STOP 4
#define BASE_SPEED 80     //Base speed for a motor
#define THRESHOLD 25      //Threshold that measures wheter the error function calculated with PID determines a change in direction
#define OFFSET 10         //Offset for the left or right turns used in the PID calculation
//--MOTORS--//

AF_DCMotor leftUp(1,MOTOR12_64KHZ);
AF_DCMotor leftDown(2,MOTOR12_64KHZ);
AF_DCMotor rightUp(4,MOTOR34_64KHZ);
AF_DCMotor rightDown(3,MOTOR34_64KHZ);

//--SONARS--//
NewPing sonarR(A5,A4,MAX_DISTANCE);
NewPing sonarL(A3,A2,MAX_DISTANCE);
NewPing sonarF(A1,A0,MAX_DISTANCE);

//---Data variables ---//
int lRead, rRead, fRead;                                //Reading of the left, right and front sensors
int lastLread = 0, lastRread = 0, lastFread = 0;        //Last value recorderd
bool stillLeft = false;                                 //Determines wether the Bot is in a Turn-Left State
bool stillRight = false;                                //Determines wether the Bot is in a Turn-Right State
bool frontWall = false, rightWall = false, leftWall = false;  //Determines whether there is a wall in a given directtion
bool turn = false;                                           //Determines if a Bot is turning at all or just advancinf forward
float P = 0.3 ;                                             //Constant for Proportional
float D = 2.5;                                              //Constant for Derivative
float I = 0.45 ;                                            //Constant for Integaral
float oldErrorP =0  ,errorI = 0,errorP = 0,errorD = 0;      
float totalError  =0 ;
float oldError = 0;
float RMS, LMS;                                             //Right and Left Motor Speed
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
    if(turn == false ) // So long as there is no front Wall, it should calibrate only for going forward
     {Serial.println("normalPID");
      normalPID();
    }

    //If there is no calibration for forward movement, check if the Bot is in a turning phase
    else if(stillLeft){
      PID(LEFT);
    }
    else if(stillRight){
      PID(RIGHT);
    }

    //Recheck the current position of the Bot with regards to the Maze's walls 
    //If there is no wall on right, but walls in front and left side, turn right
    if (leftWall == true && rightWall == false && frontWall == true ){
    PID(RIGHT) ;
    if ( turn == false ) {
      turn = true ;
      stillRight = true ;
    }
  }
  //If there is no wall in left, but there are walls in right and front side, turn left
   if (leftWall == false && rightWall == true && frontWall == true ) {
    PID(LEFT) ;
    if ( turn == false ) {
      turn = true ;
      stillLeft = true ;
    }
  }
  
 }

 //----Reading signals and interpreting distances using the NewPing library---//
void readSonars(){
  //Reading the 3 Ultrasonic Sensor data. The function sonar.ping_cm() returns the distance in centimeters from what a sonar detected
  lRead = sonarL.ping_cm(); 
  rRead = sonarR.ping_cm(); 
  fRead = sonarF.ping_cm();

  //Reinterpreting the reads as an average between the old and new Reading for better accuracy
  lRead = (lRead + lastLread)/ 2.0;
  rRead = (rRead + lastRread)/ 2.0;
  fRead = (fRead + lastFread)/ 2.0;
  if(rRead == 0 ) rRead = 200;
  if(lRead == 0 ) lRead = 200;
  if(fRead == 0 ) fRead = 200;
  
   lastLread = lRead;
   lastRread = rRead;
   lastFread = fRead;
}

//----Wall Detection --//
//If any Distance recorder from left, right or front sensors is smaller than the WALL_RANGE, it means there is a wall in that direction
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
  if(fRead < FRONT_RANGE && fRead != 0 ){
    frontWall = true;  
   // Serial.println("WALL IN FRONT");
  }
  else{
    frontWall = false;
  }
}

//--- Deterimine the direction to set the motors to---//
void moveDirection(int dir){
  
  
  switch(dir){
    case FRONT :{ // For moing FORWARD, all motors run in the same direction
      leftUp.run(FORWARD);
      leftDown.run(FORWARD);
      rightUp.run(FORWARD);
      rightDown.run(FORWARD);
      break;
  }
    case RIGHT :{ // Only the LEFT side Motors are moving
      leftUp.run(FORWARD);
      leftDown.run(FORWARD);  
     rightUp.run(RELEASE);      
      rightDown.run(RELEASE);
      break;
  }
    case LEFT :{ // Only the RIGHT Side motors are moving
      leftUp.run(RELEASE);
      leftDown.run(RELEASE);
      rightUp.run(FORWARD);
      rightDown.run(FORWARD);
     
      break;
  }
    case BACK :{ // All the Motors are Moving Backwards
      leftUp.run(BACKWARD);
      leftDown.run(BACKWARD);
      rightUp.run(BACKWARD);
      rightDown.run(BACKWARD);
      break;
  }
  case STOP:{ // None of the Motors Runs
    leftUp.run(RELEASE);
      leftDown.run(RELEASE);
      rightUp.run(RELEASE);
      rightDown.run(RELEASE);
      break;
  }
    
 }
}

//------- Motor Control using PID ------//
//Normal PID concerns the situation where the bot only has to go striaght
void normalPID(){
    errorP = lRead   - rRead   ; //calculate the difference between the right and left wall 
    errorD = errorP - oldErrorP;
    errorI = 0.8 * errorI + errorP;
    totalError = P* errorP  + D* errorD + I * errorI;
    oldErrorP = errorP ;
  
  RMS = BASE_SPEED + totalError ; // Set the Right Motor Speed using PID
  LMS = BASE_SPEED - totalError ; // Set the Left Motor Speed using PID
  if(RMS < BASE_SPEED - THRESHOLD){ // If the RMS was smaller than a given threshold, the Bot is too far left
    //means we have to turn RIGHT
    RMS =  map(RMS ,-MAX_SPEED,MAX_SPEED, 0, MAX_SPEED); 
    Serial.print("NORMAL RIGHT : LMS: ");Serial.print(LMS);
    Serial.print(" RMS: ");Serial.println(RMS);
     leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(RIGHT);
  }
  else if(LMS < BASE_SPEED - THRESHOLD){ // If the LMS was the smaller one, the Bot is too far right
    //means we have to turn LEFT
    LMS =  map(LMS , -MAX_SPEED, MAX_SPEED,0,  MAX_SPEED);
    Serial.print("NORMAL LEFT : LMS: ");Serial.print(LMS);
    Serial.print(" RMS: ");Serial.println(RMS);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(LEFT);
  }
  else{ //If none of the Above, it means the Bot is calibrated, so we continue forward
    Serial.print("NORMAL FRONT ");
    Serial.print("LMS: ");Serial.print(LMS);
    Serial.print(" RMS: ");Serial.println(RMS);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(FRONT);
  }
}
//The same effect as normalPID, except it takes into consideration an OFFSET, i.e. the difference between the right and left walls created by the turn
void PID(int dir){
    if(dir == LEFT)
      errorP = lRead   - rRead  - OFFSET;
    else
      errorP = lRead   - rRead   + OFFSET;
      
    errorD = errorP - oldErrorP;
    errorI = 0.8 * errorI + errorP ;
    totalError = P * errorP + D * errorD + I * errorI ;
    oldErrorP = errorP ;
    RMS = BASE_SPEED + totalError;
    LMS = BASE_SPEED - totalError ;
    
    //The Same Rationale Applies as for normalPID
   if(RMS < BASE_SPEED - THRESHOLD){
    //means we have to turn RIGHT
    RMS =  map(RMS , -MAX_SPEED, MAX_SPEED, 0, MAX_SPEED)  ;
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(RIGHT);
  }
  else if(LMS < BASE_SPEED - THRESHOLD){
    
    LMS =  map(LMS , -MAX_SPEED, MAX_SPEED, 0, MAX_SPEED);
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(LEFT);
  }
  else
  {
    leftUp.setSpeed(LMS);
    leftDown.setSpeed(LMS);
    rightDown.setSpeed(RMS);
    rightUp.setSpeed(RMS);
    moveDirection(FRONT);
  }
  }
