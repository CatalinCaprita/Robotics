#include "Game.h"
#include "Arduino.h"
#include "Map.h"
#include "Enemy.h"
#include "Player.h"
#include "Joystick.h"
#include "LedControl.h"
#include "EEPROM.h"

//Constant for Various Cursor Positions on the LCD MENU
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
#define HIADDR 0 //Address at which to store the highest Score in EEPROM

// Level Configurations, where 1 represents the position of an enemy
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
    //movement pattern for level 1 is a simple left contiguous movement
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
    //movement pattern for level 1 is nothing,only movement on the X Axis
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

//The menu will return true if Play was not selected;
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

        //whenever a change in the Menu Page is detected, re-render the Menu Display
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
        //Whenever the joystick records a press of the SW button
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
                    return 3; // value 3 means INFO has been selected on the menu
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

//Printing Routine of the INFO section
//DELAY is used only in order to make a readable interval of time between messages
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
                cursorLine = SETNAME; //update to the first page
                else
                    if(cursorLine == CONTRAST)
                    cursorLine = EXIT;
                else
                    cursorLine++;
            //Update Page displayed Accordingly
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
                    return -1; //value -1 means the current Menu Page has been Exited

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
            menu.print("???"); // if no nam has been selected , the name of the player is ??? by default
                                // Gameboy-style RPGs Nostalgia
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
            if(valY  == 1) // value of 1 means scroll upwards, in descending lexicographical order
                if(playerName[cursorPosition] == 'Z')
                    playerName[cursorPosition ] = ' ';
                else
                    if(playerName[cursorPosition] == ' ')
                    playerName[cursorPosition ] = 'A';
                    else
                    playerName[cursorPosition ] ++;
            if(valY == -1) // value -1 means scrolling downard, in ascending lexigraphical oreder
                if(playerName[cursorPosition] == 'A')
                    playerName[cursorPosition ] = ' ';
                else
                if(playerName[cursorPosition] == ' ')
                    playerName[cursorPosition ] = 'Z';
                else
                    playerName[cursorPosition] --;
            printName();

        }
        if(valX != 0 ) // if scrolling on the X- axis
        {

            if(valX  == 1) // right scroll means a new charater must be added to the name
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
            if(valX == -1) // left scroll means select aprevious character in the name
                {
                    cursorPosition--;
                    if(cursorPosition < 0)
                        cursorPosition = 0;
                }

            menu.clear();
            printName();
        }


        if(j.debounce() == 1) //pressing the button will exit Name Menu
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
    //player is informed if a new HighScore has been reached
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
//Player is presented with 2 Options upon finishing a session: REPLAY or BACK TO MENU
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

    //Allow the player to have at most 16 charaters in the Name
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
    {
        for(int j = 0; j < MAPSIZE; j ++ )
            if(levels[currentLevel][i * MAPSIZE + j] == 1)
        {
            eArray[eSize].posY = i;
            eArray[eSize].posX = j;
            eSize++;
        }
    }

    //Index array is in the beginning set in order
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

//Lets the player get ready by a "3-2-1-GO!" style Message
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

//Update Level from the Levels Matrix
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

    //Update the new positions of the Enemies
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

    //Initially each Enemy is in position i which it has been detected, starting from (0,0)
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

//Checks if a any Enemy is at the (X,Y) coordinates
bool Game::shootEnemy(const byte Y, const byte X)
{

    for(int i = 0; i < eSize; i++)
        if(eArray[idx[i]].posX == X && eArray[idx[i]].posY == Y)
        {
            //if So, then Enemy loses a life
            eArray[idx[i]].lives--;


            if(eArray[idx[i]].lives <= 0)
            {
                //Delete the Enemy for the Array by Effectively moving it to the last position and
                //Decrementing the Size pointer
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

        return 2; //2 is the code for Exiting a session, in this case a WIN

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
          player.doneShoot(); //erase the bullet from the map
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
                //if the movement Delay is passed
                int j = i;
                if(dx[currentLevel][currentPosition] == 1 ||
                   dy[currentLevel][currentPosition] == 1 ) // check for possible collisions between enemies. Might "cover" one another and make some invisible
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
                    if(eArray[kth].hasShot() == true) // as for the Plyer, an Enemy has either a travelling bullet or not on the Map
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
                    currentPosition = (currentPosition + 1) % LEVELS;
                    lastMovement = millis();
            }


    }

  return 1;

}

//Resets the variables of an onGoint session, by re-entering with level 1
void Game::replay()
{
    currentLevel = -1;
    player.lives = 3;
    player.score = 0;
    progress();
}
