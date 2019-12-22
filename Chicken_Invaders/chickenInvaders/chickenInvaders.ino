#include <EEPROM.h>
#include <Player.h>
#include <Joystick.h>
#include <LedControl.h>
#include <LiquidCrystal.h>
#include <Map.h>
#include <Enemy.h>
#include <Game.h>
#define PLAY 1
#define OPTIONS 2
#define SETNAME 1
#define CONTRAST 2
#define EXIT -1
#define REPLAY 2
#define INFO 3
#define LOST 2
#define WON 0
#define HIADDR 0 
const byte mapSizeX = 8;
const byte mapSizeY = 8;

//MENU VARIABLES
int hiScoreAddr = 0;
bool run = false;
int state = 0;
int optionState = 0;
int setName = 0;
int changeContrast = 0;
int gameOver = 1;
int whatNext = 0;
bool printedOnce = false;
Game session(mapSizeX,mapSizeY);

void setup() {
  Serial.begin(9600);
}

void loop() {

  if(printedOnce == false)
  {
    session.greetMsg();
    printedOnce = true;
  }
  if(state == 0)
    state = session.menuDisplay();
    else
    if(state == OPTIONS)
      {
  
        if(optionState == 0)
         optionState = session.optionsDisplay();
        else
          if(optionState == SETNAME)
            {
              if(setName == 0)
              setName = session.setName();
             else
             if(setName == -1)
             {
              session.optionsMenu();
              setName = 0;
              optionState = 0;
             }
            }
         else
         if(optionState == CONTRAST)
         {
          if(changeContrast == 0)
            changeContrast = session.changeContrast();
          else
            {
              session.optionsMenu();
              optionState = 0;
              changeContrast = 0;
             }
         }
         else
         if(optionState == EXIT)
          {
            state = 0;
            optionState = 0;
            changeContrast = 0;
            session.mainMenu();
          }
      }
      else
      if(state == INFO)
      {
        state = 0;
        optionState = 0;
        session.showInfo2();
      }
    else
    if(state == PLAY)
      {
        if(gameOver == 1)
        gameOver = session.onGoing();
        else
        if(gameOver == LOST || gameOver == WON) //signals the whatNext screen
         {
            if(whatNext == 0)
              whatNext = session.whatNext();
             else
              if(whatNext == REPLAY)
            {
              session.replay(); 
              whatNext = 0;
              state = PLAY;
              gameOver = 1;
            }
            else
            {
              state = 0;
              whatNext = 0;
              optionState = 0;
              gameOver = 1;
            }
         }
      }
  /*if(run == true)
  if(session.onGoing() != 1)
  {
    Serial.println("Game FINISHED");
    run = false;
  }*/
}
