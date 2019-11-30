#include <LiquidCrystal.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);

/*Joystick variables*/
const int pinX = A0;
const int pinY = A1;
const int pinSW = 7;
int valX = 0;
int valY = 0;
boolean movedX = 0;
boolean movedY = 0;
boolean changed = 0;
const int minThreshold = 400;
const int maxThreshold = 600;

const double long debounceDelay = 200;
int lastDebounce = 0;
int swRead;
int swState = 0;
int lastSwState = 0;

//Menu Variables
int cursorPos = 0;
int cursorLine = 1;
int highScore = 0;
boolean playing = 0;
boolean options = 0;
boolean selected = 0;

//Options Variables
boolean optSelected = false;
int currentStartLevel = 0;
int startLevel = 0;

//inGame Variables
int currentLevel;
int inGameTime = 0;
double lastUpdate;
double lastLevelModified = 0;
const int maxInGame = 10;
const double  levelInterval = 5000;
const double  updateInterval = 1000;


void setup() {
  lcd.begin(16,2);
  lcd.clear();
  
    pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  pinMode(pinSW,INPUT_PULLUP);
}

int readSW()
{
  //Reading the button Press

    swRead = digitalRead(pinSW);

  //whenever state changes, either during sound or actual press, change the laast time since debounce
    if(swRead != lastSwState)
    {
      lastDebounce = millis();
    }

  //if waited long enough to call it an actual press
    if(millis() - lastDebounce >= debounceDelay)
    {
      if(swRead != swState)
        swState = swRead;
    //onlly if button has actually been pressed
      if(swState == LOW)
          {
            lastSwState  = swRead;
            return 1;
          }
    }
    lastSwState = swRead;
   return 0;
}
void mainMenu()
{
 
  lcd.setCursor(0,0);
  lcd.print("HiScore:");
  lcd.print(highScore);
  lcd.setCursor(0,1);
  lcd.print(" Play    Options");
  
}

void showCrt(const int cursorPos,const int cursorLine)
{
  lcd.setCursor(cursorPos,cursorLine);
  lcd.print(">");
  
}
void endGame()
{
  lcd.clear();
  lcd.print("Congratulations!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You Finished");
  lcd.setCursor(0,1);
  lcd.print( "The game!");
  delay(1000);
  lcd.clear();
  
}

void inGame()
{
  lcd.setCursor(0,0);
  lcd.print("Lives:3");
  lcd.setCursor(7,0);
  lcd.print(" Score:");
  lcd.print(currentLevel * 3);  
  lcd.setCursor(0,1);
  lcd.print("Level: ");
  lcd.print(currentLevel);
  //increment level every levelInterval == 5 seconds
  if(millis() - lastLevelModified >= levelInterval)
  {
    currentLevel ++;
    lastLevelModified = millis();
  }
  if(millis() - lastUpdate >= updateInterval)
  {
    inGameTime ++;
    if(inGameTime >= maxInGame)
    {
      playing = false;
      selected = false;
      if(currentLevel * 3 > highScore)
       {
        highScore = currentLevel * 3;
        lcd.clear();        
        lcd.print("WOW! New ");
        lcd.setCursor(0,1);
        lcd.print("Highscore!: ");
        lcd.print(currentLevel * 3);
        delay(3000);
       }
      endGame();
     }
    lastUpdate = millis();
  }
  
}

void opts()
{
  lcd.setCursor(1,0);
  lcd.print("Set Start Level");
  showCrt(cursorPos,cursorLine);
  lcd.setCursor(1,1);
  lcd.print("Exit");
  
}

void setLevel()
{
  lcd.setCursor(0,0);
  lcd.print("Start Level:");
  lcd.print(currentStartLevel);
  valX = analogRead(pinX);
  if(valX >= maxThreshold && movedX == false)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Start Level:");
      movedX = true;
      currentStartLevel ++;
      lcd.print(currentStartLevel);
    }
  if(valX <= minThreshold && movedX == false)
    {
      lcd.setCursor(0,0);
      lcd.print("Start Level:");
      movedX = true;
      currentStartLevel --;
      if(currentStartLevel <= 0)
        lcd.print("0");
      else
      lcd.print(currentStartLevel);
     }
    else
     {
        movedX = false;
     }
  //if button is pressed, go back to options menu
  if(readSW() == 1)
  {
    lcd.clear();
    optSelected = false;
    startLevel = currentStartLevel;
  }
}
void loop() {

  if(selected == false)
  {
      mainMenu();
    showCrt(cursorPos,cursorLine);
    valX = analogRead(pinX);
    if((valX >= maxThreshold  ||valX <= minThreshold ) && movedX == false)
    {
      lcd.clear();
      mainMenu();
      if(cursorPos == 0)
      cursorPos = 8;
      else
      cursorPos = 0;
      movedX = true;
      changed = true;
    }
    else
     {
        movedX = false;
     }
   if(readSW() == 1)
        //if locked on Play , select play
     {
            if(cursorPos == 0)
          {
            selected = true;
            playing = true;
             lastUpdate = millis();
             lastLevelModified = millis();
             currentLevel = startLevel;
             inGameTime = 0;
            lcd.clear();
          }
          else
          {
            selected = true;
            cursorPos = 0;
            cursorLine = 0;
            options = true;
            currentStartLevel = startLevel;
            lcd.clear();
        }
      }
   }
 else
   if(playing == true)
   {
    inGame();
   }
 else
     if(options == true)
     {
      if(optSelected == false)
      {
        opts();
        valX = analogRead(pinX);
        if( (valX >= maxThreshold ||valX <= minThreshold) && movedX == false)
        {
          lcd.clear();
          if(cursorLine == 0)
          cursorLine = 1;
          else
          cursorLine = 0;
          opts();
          movedX = true;
        }
        else
         {
            movedX = false;
         }
  
      if(readSW() == 1)
        {
          //if on the SET LEVEL STAGE
          if(cursorLine == 0)
          {
            optSelected = true;
            cursorPos = 0;
            cursorLine = 0;
            lcd.clear();
          }
          //means Exit option has been selected
          else
          {
            lcd.clear();
            options = false;
            selected = false;
          }
        }
      }
      //if entered Set level Stage
      else
      {
        setLevel();
      }
  }
 delay(200);
}
