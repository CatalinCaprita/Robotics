#include "Game.h"
#include "Arduino.h"
#include "Map.h"
#include "Enemy.h"
#include "Player.h"
#include "Joystick.h"
#include "LedControl.h"
#include "EEPROM.h"
#define PLAY 0
#define OPTIONS 8
#define SETNAME 0
#define CONTRAST 1
#define EXIT 2
#define MAXNAMESIZE 16
#define LEVELS 5
#define MAPSIZE 8
#define REPLAY 0
#define MENU 8
#define INFO 16
#define HIADDR 0
const byte Game::levels[LEVELS][MAPSIZE * MAPSIZE] =
{
  //Level 1
  {0,0,0,0,1,0,0,0,
   0,0,0,1,1,1,0,0,
   0,0,0,0,1,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0},
  //Level 2
  {0,0,0,0,0,0,0,0,
   0,0,0,1,1,0,0,0,
   0,1,1,0,0,1,1,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0},
   //Level 3
  {0,0,0,0,0,0,0,0,
   0,0,1,0,0,1,0,0,
   1,1,0,1,1,0,1,1,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0},
   //Level 4
  {0,0,0,0,0,0,0,0,
   0,1,1,1,0,1,1,1,
   0,0,1,0,0,0,1,0,
   0,0,1,0,0,0,1,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0},
  // Level 5
  {0,0,1,1,1,1,1,0,
   0,0,1,0,1,0,1,0,
   0,0,1,1,1,1,1,0,
   0,0,0,1,0,1,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0}

};

const int Game::dx[LEVELS][LEVELS] =

{
    //movement pattern for level 1 is nothing. getting accustomed to
    //gameplay
    {-1,0,0,0,0},

    //at level 2 enemies will move 2 times left, 2 times right
    {-1,-1,0,1,1},

    //Level 3 Zig-Zag
    {1,1,1,1,1},

    //level 4 will have a circular motion of range 1
    {-1,-1,1,1,0},
    //level 5 will have a square motion of offfset 1
    {-1,0,1,0,0}
};

const int Game::dy[LEVELS][LEVELS] =
{
    //movement pattern for level 1 is nothing. getting accustomed to
    //gameplay
    {0,0,0,0,0},
    //at level 2 enemies will move 2 times left, 2 times right
    {0,0,0,0,0},
    //Level 3 Zig-Zag
    {1,-1,1,-1,1},
    //level 4 will have a circular motion of range 1
    {-1,+1,+1,-1,0},
    //level 5 will have a circular motion of range 1
    {0,1,0,0,-1}
};

/*for LiquidCrysal we use
RS = 8
v0 = analog A4
enable = 7
d4 = 3;
d5 = 4;
d6 = 5;
d7  = 6;
*/

//The manu will return true if Play was not selected;
//Otherwise, return false;
int Game::menuDisplay()
{
    showCrs(cursorLine,cursorPosition);
    int valx = j.readMoveX();
    if(millis() - lastScroll >= scrollSensitivity)
    {
        if(valx != 0 )
        {
        menu.clear();
        if(valx  == -1)
            if(cursorPosition == INFO)
                cursorPosition = OPTIONS;
            else
                if(cursorPosition == PLAY)
                cursorPosition = INFO;
            else
                cursorPosition -= OPTIONS;
        if(valx == 1)
            if(cursorPosition == INFO)
                cursorPosition = PLAY;
            else
                if(cursorPosition == OPTIONS)
                cursorPosition = INFO;
            else
                cursorPosition += OPTIONS;
        }
        if(cursorPosition == INFO)
        {
            menu.clear();
            showInfo();
        }
        else
        {
            menu.clear();
            mainMenu();
        }
        if(j.debounce() == 1)
            {
                if(cursorPosition == OPTIONS)
                {
                    cursorPosition = 0;
                    cursorLine = 0;
                    menu.clear();
                    lastScroll = millis();
                    optionsMenu();
                    return 2; // value 2 is the code for Entering Options Function
                }
                if(cursorPosition == PLAY)
                {
                    lastScroll = millis();
                    menu.clear();
                    preLevel();
                    inGameMenu();
                    return 1; // value 1 is the code for Entering Game
                }
                if(cursorPosition == INFO)
                {
                    lastScroll = millis();
                    menu.clear();
                    return 3;
                }
            }
        lastScroll = millis();

    }
    return 0; // returns 0 means We are in Main Menu and nothing is selected
}
void Game::showExit()
{
    menu.setCursor(1,0);
    showCrs(cursorLine,cursorPosition);
    menu.print("EXIT");
}
void Game::optionsMenu()
{
    menu.setCursor(1,0);
    menu.print("Enter Name");
    menu.setCursor(1,1);
    menu.print("Change Contrast");
}
void Game::showInfo()
{
    menu.setCursor(1,0);
    showCrs(cursorPosition % INFO,cursorLine);
    menu.print("INFO");
}
void Game::showInfo2()
{
    menu.setCursor(0,0);
    menu.print("Matrix Heroes");
    delay(1500);
    menu.clear();
    menu.print("By: Caprita");
    menu.setCursor(0,1);
    menu.print("Nicusor -Catalin");
    delay(1500);
    menu.clear();
    menu.setCursor(0,0);
    menu.print("github.com/");
    menu.setCursor(0,1);
    menu.print("CatalinCaprita/");
    delay(1500);
    menu.clear();
    menu.setCursor(0,0);
    menu.print("@UniBuc Robotics");
    delay(1500);

}

int Game::optionsDisplay()
{
    showCrs(cursorLine,cursorPosition);
     int valY = j.readMoveY();
    if(millis() - lastScroll >= scrollSensitivity)
    {
        if(valY != 0 )
        {
        menu.clear();
        if(valY  == -1)
           if(cursorLine == SETNAME)
                cursorLine = EXIT;
                else
                    if(cursorLine == EXIT)
                cursorLine = CONTRAST;
                else
                   cursorLine--;
        if(valY == 1)
                if(cursorLine == EXIT)
                cursorLine = SETNAME;
                else
                    if(cursorLine == CONTRAST)
                    cursorLine = EXIT;
                else
                    cursorLine++;
            if(cursorLine == EXIT)
                {
                    menu.clear();
                    showExit();
                }
            else
            {
                    menu.clear();
                    optionsMenu();
            }
        }

        if(j.debounce() == 1)
            {
                if(cursorLine == SETNAME)
                {
                    menu.clear();
                    cursorPosition= 0;
                    cursorLine = 1;
                    lastScroll = millis();
                    return 1; // value 2 is the code for Entering Name
                }
                if(cursorLine == CONTRAST)
                {
                    menu.clear();
                    cursorPosition= 0;
                    cursorLine = 0;
                    lastScroll = millis();

                    return 2; // value 1 is the code for Entering Game
                }
                if(cursorLine == EXIT)
                {
                    menu.clear();
                    cursorLine = 1;
                    cursorPosition = 0;
                    lastScroll = millis();
                    return -1;

                }
            }
        lastScroll = millis();

    }
    return 0; // returns 0 means We are in Options Menu and nothing is selected


}

int Game::changeContrast()
{
    menu.setCursor(0,0);
    menu.print("Contrast: ");
    menu.print(contrastVal);
    int valY = j.readMoveY();
    if(millis() - lastScroll >= scrollSensitivity)
    {
        if(valY != 0 )
        {
            menu.clear();
            if(valY  == 1)
                contrastVal += 10;
            else
                contrastVal -= 10;
        }

        if(j.debounce() == 1)
            {
                menu.clear();
                lastScroll = millis();
                analogWrite(contrastPin,contrastVal);
                return 1;
            }
        lastScroll = millis();

    }
    return 0;
}


void Game::printName()
{

    menu.setCursor(0,0);
        if(nameSize == 0)
            menu.print("???");
        else
            for(int i = 0;i< nameSize; i++)
            {
                menu.print(playerName[i]);
            }
}
int Game::setName()
{
    menu.setCursor(cursorPosition,cursorLine);
    menu.print("^");
    int valY = j.readMoveY();
    int valX = j.readMoveX();

    if(millis() - lastScroll >= scrollSensitivity)
    {
        if(valY != 0 )
        {
            menu.clear();
            if(valY  == 1)
                if(playerName[cursorPosition] == 'Z')
                    playerName[cursorPosition ] = ' ';
                else
                    if(playerName[cursorPosition] == ' ')
                    playerName[cursorPosition ] = 'A';
                    else
                    playerName[cursorPosition ] ++;
            if(valY == -1)
                if(playerName[cursorPosition] == 'A')
                    playerName[cursorPosition ] = ' ';
                else
                if(playerName[cursorPosition] == ' ')
                    playerName[cursorPosition ] = 'Z';
                else
                    playerName[cursorPosition] --;
            printName();

        }
        if(valX != 0 )
        {

            if(valX  == 1)
                {
                    cursorPosition++;
                    if(cursorPosition >= nameSize)
                        if(nameSize + 1 >= MAXNAMESIZE)
                            cursorPosition--;
                        else
                            nameSize++;
                    if(cursorPosition > MAXNAMESIZE)
                        cursorPosition = 0;
                }
            if(valX == -1)
                {
                    cursorPosition--;
                    if(cursorPosition < 0)
                        cursorPosition = 0;
                }

            menu.clear();
            printName();
        }


        if(j.debounce() == 1)
            {
                menu.clear();
                cursorLine = 0;
                cursorPosition = 0;
                lastScroll = millis();

                return -1;
            }
        lastScroll = millis();

    }

    return 0;

}
void Game::mainMenu()
{

 menu.setCursor(0,0);
  menu.print("HiScore:");
  menu.print(EEPROM.read(HIADDR));
  menu.setCursor(0,1);
  menu.print(" Play    Options");

}

void Game::inGameMenu()
{
    printName();
    menu.print(" Level:");
    menu.print(currentLevel + 1);
    menu.setCursor(0,1);
    menu.print("Lives:");
    menu.print(player.lives);
    menu.print("Score:");
    menu.print(player.score);
}

void Game::endGame()
{
    menu.clear();
    menu.print("GAME OVER!");
    delay(1500);
    menu.clear();
    printName();
    menu.print(" Score:");
    menu.print(player.score);
    menu.setCursor(0,1);
    menu.print("GOOD GAME!");
    delay(1500);
    if(player.score > EEPROM.read(HIADDR))
        {
            menu.clear();
            menu.setCursor(0,0);
            menu.print("WOW!");
            menu.setCursor(0,1);
            menu.print("NEW BEST SCORE!");
            delay(2000);
            EEPROM.write(HIADDR,player.score);
            hiScore = player.score;
        }
   menu.clear();

}
void Game::postGame()
{
    menu.setCursor(0,0);
    menu.print("WHAT NEXT?");
    menu.setCursor(1,1);
    menu.print("REPLAY");
    menu.print("  MENU");

}
int Game::whatNext()
{
    menu.setCursor(cursorPosition,cursorLine);
    showCrs(cursorLine,cursorPosition);
    int valx = j.readMoveX();
        if(millis() - lastScroll >= scrollSensitivity)
    {
        if(valx != 0 )
        {
        menu.clear();

        if(valx  == -1)
        cursorPosition = MENU - cursorPosition;
        else
            if(cursorPosition == REPLAY)
                cursorPosition = MENU;
            else
                cursorPosition = REPLAY;
            postGame();
        }

        if(j.debounce() == 1)
            {
                if(cursorPosition == REPLAY)
                {
                    cursorPosition = 0;
                    cursorLine = 0;
                    menu.clear();
                    lastScroll = millis();
                    return 2; // value 2 is the code for Entering Options Function
                }
                if(cursorPosition == MENU)
                {
                    menu.clear();
                    lastScroll = millis();
                    player.lives = 0;
                    currentLevel = 0;
                    player.score = 0;
                    mainMenu();
                    return -1; // value 1 is the code for Entering Game
                }
            }
        lastScroll = millis();

    }
    return 0;
}
void Game::greetMsg()
{
    menu.clear();
    menu.setCursor(0,0);
    menu.print("  Matrix Heroes  ");
    menu.setCursor(0,1);
    menu.print("    Caprita TM  ");

    delay(1500);
    menu.clear();
    menu.setCursor(0,0);
    menu.print(" Welcome !");
    delay(1500);
    menu.clear();
}
void Game::showCrs(const byte crsLine, const byte crsPos)
{

  menu.setCursor(crsPos,crsLine);
  menu.print(">");
}

Game::Game(const byte nRows, const byte nCols):currentLevel(0),player(4,7),j(A0,A1,A2),lc(12, 11, 10, 1),mapSize(nRows),activeEnemies(1),maxLifeCap(1),eProjectiles(0),
fireDelay(850),lastFire(0),lastSelected(0),selectedDelay(20),currentPosition(0),movementDelay(230),lastMovement(0),menu(8,7,3,4,5,6),cursorLine(1),cursorPosition(0),scrollSensitivity(150),lastScroll(0),
contrastVal(100),contrastPin(9),nameSize(0),hiScore(0),addFactor(1)
{
    lc.shutdown(0, false); // turn off power saving, enables display
    lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    lc.setLed(0,player.posY,player.posX,true);


    analogWrite(contrastPin,contrastVal);
    menu.begin(16,2);
    menu.clear();
    playerName = new char[MAXNAMESIZE];
    for(int i = 0;i< MAXNAMESIZE; i++)
        playerName[i] = 'A';
    mainMenu();
    showCrs(cursorLine,currentPosition);
    eSize = 0 ;
    activeEnemies = 1;
    currentPosition = 0;
    minSpawn = activeEnemies;
    for(int i = 0; i < MAPSIZE; i++)
    {//    Serial.println("Passed Update Pos");
        for(int j = 0; j < MAPSIZE; j ++ )
            if(levels[currentLevel][i * MAPSIZE + j] == 1)
        {
            eArray[eSize].posY = i;
            eArray[eSize].posX = j;
            eSize++;
        }
    }
    for(int i = 0 ; i < eSize; i++)
        idx[i] = i;
    nextMove = false;
    lc.setLed(0,0,0,false);
}

Game::~Game()
{
    delete []eArray;
    delete []idx;

}
void Game::preLevel()
{
    menu.clear();
    menu.setCursor(0,0);
    menu.print("Here we Go!");
    menu.setCursor(0,1);
    if(currentLevel  == LEVELS - 1)
        menu.print("THE FINAL BOSS!");
    else
    {
        menu.print("Level:");
        menu.print(currentLevel + 1 );
    }
    menu.print("  Ready?");
    delay(2000);
    for(int i = 3 ;i > 0; i--)
    {
        menu.clear();
        menu.setCursor(MAXNAMESIZE / 2,0);
        menu.print(i);
        delay(1000);
    }
    menu.clear();
    menu.setCursor(MAXNAMESIZE / 2,0);
    menu.print("GO!");
    delay(500);
    menu.clear();
    inGameMenu();
}
void Game::progress()
{
    currentLevel ++;
        if(currentLevel == LEVELS)
        {
            return ;
        }
    //update the enemy Array
    eSize = 0;

    //allowed enemies to shoot at once is given by current level
    activeEnemies ++;
    eProjectiles = 0;

    //each enemy can have between [1, currentLevel] lives
    maxLifeCap = currentLevel + 1;
    lastFire = 0;

    //delay for selecting random enemy to shoot
    lastSelected = 0;

    for(int i = 0; i < MAPSIZE; i++)
    {
        for(int j = 0; j < MAPSIZE; j ++ )
            if(levels[currentLevel][i * MAPSIZE + j] == 1)
        {
            eArray[eSize].posY = i;
            eArray[eSize].posX = j;
            eArray[eSize].lives = (byte)random(1,maxLifeCap +1);
            eSize++;
        }
    }

    for(int i = 0 ; i < eSize; i++)
        idx[i] = i;

    player.respawn(lc);
    minSpawn = activeEnemies;
    lastMovement = 0;
    nextMove = false;
    currentPosition = 0;
    lc.setLed(0,0,0,false);
    menu.clear();
    preLevel();

}

bool Game::shootEnemy(const byte Y, const byte X)
{

    for(int i = 0; i < eSize; i++)
        if(eArray[idx[i]].posX == X && eArray[idx[i]].posY == Y)
        {
            eArray[idx[i]].lives--;
            if(eArray[idx[i]].lives <= 0)
            {
                eArray[idx[i]].active = true;
                int temp = idx[i];
                idx[i] = idx[eSize - 1];
                idx[eSize - 1] = temp;
                eSize--;
                lc.setLed(0,Y,X,false);
                lc.setLed(0,eArray[idx[i]].shootY,eArray[idx[i]].shootX,false);
            }
            return 1;
        }
    return 0;

}
int Game::onGoing()
{
    if(eSize == 0)
    {

      if(currentLevel ==  LEVELS)
      {
           menu.clear();
        endGame();
        lc.clearDisplay(0);
        cursorLine = 1;
        currentPosition = 0;
        postGame();

        return 2; //2 is the code f

      }
      progress();
    }
  else
  {
    randomSeed(millis());
    int kth;
    int x = j.readMoveX();
    int y = j.readMoveY();
    player.updatePos(x,y,MAPSIZE,lc);

      if(player.hasShot() == false)
      {
        if(j.debounce() == 1)
          {
            //Player chose to shoot
              player.shoot();
          }
      }
      else
      if(player.hasShot() == true)

      {
        //Bullet is still travelling
        player.travellingBullet(lc);
        byte y = player.getShootY();
        byte x = player.getShootX();
        if(shootEnemy(y,x) == 1)
          {
            //enemy in Array at specified position is shot
            player.score += 10;
            if((player.score % 100)== 0)
                player.lives++;
            menu.clear();
            inGameMenu();
            player.doneShoot();
            lc.setLed(0,y,x,false);
          }
        else
        if( y <= 0)
        {
          player.doneShoot();
            lc.setLed(0,y,x,false);
        }
      }

    //make random #activeEnemies enemies enemies shoot
     if(eProjectiles == 0 && millis() - lastSelected >= selectedDelay)
    {
        if(eSize > activeEnemies)
            minSpawn = activeEnemies;
        else
            minSpawn = eSize;
            for(int i = 0; i< minSpawn; i++)
        {
//            //populate the actvie aray with random enemies left alive;
                    kth = (int)random(0,eSize);
                    eArray[idx[kth]].active = true;
                    eProjectiles++;
        }
           lastSelected = millis();
    }
        for(int i = 0; i < eSize;i ++ )
        {

            if(nextMove == true)
            {
                int j = i;
                if(dx[currentLevel][currentPosition] == 1 ||
                   dy[currentLevel][currentPosition] == 1 )
                    j = eSize - i - 1;
                eArray[idx[j]].updatePos(dx[currentLevel][currentPosition]
                                     ,dy[currentLevel][currentPosition]
                                     ,mapSize,lc);
            }
            if(eArray[idx[i]].active == true)
             {
                 if(eArray[kth].hasShot() == false)
                {
                    if(millis() - lastFire >= fireDelay)
                    {
                        eArray[kth].shoot();
                        lastFire = millis();
                    }
                }
                else
                    if(eArray[kth].hasShot() == true)
                {

                    eArray[kth].travellingBullet(lc);
                    if(player.posX == eArray[kth].shootX && player.posY == eArray[kth].shootY)
                    {
                        player.respawn(lc);
                        player.lives --;
                        menu.clear();
                        inGameMenu();
                        if(player.lives == 0)
                            {
                                menu.clear();
                                endGame();
                                lc.clearDisplay(0);
                                cursorLine = 1;
                                currentPosition = 0;
                                postGame();
                                return 2; //2 is the code for WhatNext Screen
                            }
                        eArray[kth].doneShoot();
                        eProjectiles--;

                    }
                    else
                    if( eArray[kth].shootY > MAPSIZE - 1)
                      {
                          eArray[kth].doneShoot();
                          eProjectiles --;
                        }

                    }
                }

        }

        nextMove = false;

        if(millis() - lastMovement >= movementDelay)
            {
                nextMove = true;
//                if(currentLevel == LEVELS - 1 )
//                    if(currentPosition == LEVELS - 1)
//                    {
//                        addFactor *= -1;
//                    }
//                    else
//                        if(currentPosition == 0)
//                        if(addFactor == -1)
//                        addFactor *= -1;
                    currentPosition = (currentPosition + 1) % LEVELS;
                    lastMovement = millis();
            }


    }

  return 1;

}

void Game::replay()
{
    currentLevel = -1;
    player.lives = 3;
    player.score = 0;
    progress();
}
