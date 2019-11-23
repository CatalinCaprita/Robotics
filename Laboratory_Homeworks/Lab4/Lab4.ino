const int pinA = 6;
const int pinF = 7;
const int pinB = 8;
const int pinG = 9;
const int pinC = 10;
const int pinD = 11;
const int pinE = 12;
const int pinDP = 13;

const int pinD1 = 1;
const int pinD2 = 3;
const int pinD3 = 4;
const int pinD4 = 5;

const int segmentNo = 8;
int segments[segmentNo] =
{pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};

const unsigned int digitNo = 4;
const int digits[digitNo] =
{pinD1, pinD2, pinD3, pinD4};
 
byte digitCode[10][segmentNo - 1] = { 
  // a b c d e f g
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,0,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}, //9
    
    
  };

const int pinX = A1;
const int pinY = A0;
const int pinSW = 2;
int valX;
int valY;
bool movedX = false;
bool movedY = false;
bool locked = false;
const int minThreshold = 350;
const int maxThreshold = 650;

int swState = 0;
int lastSwState = 1;
int swRead = 0;
const unsigned long  debounceDelay = 100;
unsigned long lastDebounceTime = 0;
int currentDisplay = 0;
int currentDigit = 0;
int dpState = 0;
int lockState = 0;


void setup() {
  for(int i = 0; i<segmentNo; i++)
    pinMode(segments[i],OUTPUT);
  for(int i = 0; i < digitNo; i++)
    pinMode(digits[i],OUTPUT);
   Serial.begin(9600);
  pinMode(pinSW,INPUT_PULLUP);
  
}

void displayNumber(int Number, int DpState)
{
  for(int i = 0; i < segmentNo - 1; i++)
    digitalWrite(segments[i],digitCode[Number][i]);

digitalWrite(segments[segmentNo-1],DpState);
}

void onCrt()
{
  for(int i = 0; i < segmentNo - 1; i++)
    digitalWrite(segments[i],LOW);

digitalWrite(segments[segmentNo - 1],HIGH);
}


void onDigit(int num)
{
  for(int i = 0;i < digitNo; i++)
    digitalWrite(digits[i],HIGH);
   digitalWrite(digits[num],LOW);
}


void loop() {


  //if not locked, we are interested in the value of X
  if(locked == false)
  {
    valX = analogRead(pinX);
    Serial.println(valX);
  
   if( valX <minThreshold && movedX == false)
    {
           if(currentDisplay == 0)
            currentDisplay = digitNo - 1;
           else
           currentDisplay--;
           movedX = true;
           
    }
  if( valX > maxThreshold && movedX == false)
    {
           if(currentDisplay == digitNo - 1)
            currentDisplay = 0;
           else
           currentDisplay++;
           movedX = true;
        
    }
   if(valX >= minThreshold && valX <= maxThreshold)
      {
        movedX = false;
      }
   if(movedX == true)
   {  onDigit(currentDisplay);
      onCrt();
   }
  }
//we finished reading the X, or we do not care about it
  swRead = digitalRead(pinSW);
  Serial.print("swRead: ");
  Serial.print(swRead);
  Serial.print("LastSw: ");
  Serial.println(lastSwState);
   Serial.print("Sw: ");
  Serial.println(swState);
  
 if(swRead != lastSwState)
     {
       lastDebounceTime = millis();
     }
 //waited long enough since the lastDebounce was triggered to consider it an actual press
if(millis() - lastDebounceTime > debounceDelay)
{
  
  if(swRead != swState)
  {
    
    swState = swRead;
    //only if the button input was pressed
    if(swState == LOW)
    {// if no display is locked
      if(locked == false)
       {
         
          onDigit(currentDisplay);
          displayNumber(0,HIGH);
          locked = true;
          Serial.print("LOCKED ON : ");
          Serial.println(currentDisplay);
          
       }
       
      else
      {
            displayNumber(9,HIGH);
            locked = false;
            Serial.print("UNLOCKED ON :");
            Serial.println(currentDisplay);
            
        }
      }
    }
    
}
 lastSwState = swRead;

 if(locked == true)
    {     
        //If locked on a display read the valY
         valY = analogRead(pinY);
          
            if( valY < minThreshold && movedY == false)
      {
             if(currentDigit == 9)
              currentDigit = 0;
             else
             currentDigit--;
             movedY = true;
             
      }
            if( valY > maxThreshold && movedY == false)
      {
             if(currentDigit == 9)
              currentDigit= 0;
             else
             currentDigit++;
             movedY = true;
          
      }
            if(valY >= minThreshold && valY <= maxThreshold)
               movedY = false;
        displayNumber(currentDigit,HIGH);
        
    }
  
}
  
