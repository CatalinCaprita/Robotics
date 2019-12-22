#include "Arduino.h"
#include "Map.h"
#include "Enemy.h"
#include "Player.h"
#include "Joystick.h"
#include "LedControl.h"
#include "LiquidCrystal.h"
#ifndef GAME_H
#define GAME_H
#define LEVELS 5
#define MAPSIZE 8
/*The Game class is a wrapper class for an on-going game with all the functionalities of a session
*/

class Game
{
    Enemy eArray[MAPSIZE * 2]; // and array of Enemy objects. The Size is fixed, but the positions do vary according to the level
    Player player;  // a Player object, to manage the actions of the player
    Joystick j; // Joystick to manage the actions of the player
    LedControl lc; // LedControl to facilitate the 8x8 LED Matrix control
    LiquidCrystal menu; // Liquid Crystal menu to manage the Menu functionalities
    char *playerName; // The player Name can be at most 16 characters, the length of a row on the LCD Menu
    int idx[MAPSIZE * 2];
    byte currentLevel;  //Variable used to keep track of what configuration to be loaded durin a session
    int contrastVal;  //Value of the contrast Pin of the LCD menu
    int eSize;  //Pointer to the position of the last Enemy in the Enemy array
    int nameSize; // Pointer to the position of the last charater in the playerName array
    int minSpawn; // Variable used to determine how many projectiles can be fired simultaneously. The minimum between Active Enemies and Alive Enemies
    int hiScore; // Maintain the highest registered score
    int contrastPin; //pin for the contrast of the LCD
    bool nextMove; // Boolean Value to maintain wether the enemies can make a new move. The delay between 2 consecutive movements of an Enemy
    bool inGame;
    byte currentPosition; // Used to iterate throught the movement arrays defined above
    byte activeEnemies; // Maximum number of enemies that can shoot at once during a given level
    byte maxLifeCap; // Maximum amount of lives an individual Enemy can have
    byte eProjectiles; // Variable used to keep track of current projectiles are displayed on the LED Matrix
    byte cursorPosition; //Variables used for the position of the ">" cursor in the menu
    byte cursorLine;
    const byte mapSize;
    static const byte levels[LEVELS][MAPSIZE * MAPSIZE];  // Each of the 5 levels configurations

    // During each level , an Enemy has a movement pattern directed by an OffSet on the X and Y axis respectively
    static const int dx[LEVELS][LEVELS];
    static const int dy[LEVELS][LEVELS];

    //Constants and update variables to be used for certain Delays in the program
    const long movementDelay;
    const double fireDelay; // Keep track of the speed of the enemy projectiles
    const long selectedDelay; // Delay at which to select the new Active Enemies to shoot
    const long scrollSensitivity;
    long lastScroll; // Keep track of joystick activity throught the menu
    double lastFire;
    double lastSelected;
    long lastMovement;
    public:
        Game(const byte = MAPSIZE, const byte = MAPSIZE);
        virtual ~Game();
        int onGoing();
        void progress();
        bool shootEnemy(const byte, const byte);
        void showCrs(const byte, const byte);
        void greetMsg();
        void mainMenu();
        int menuDisplay();
        void optionsMenu();
        void showExit();
        int optionsDisplay();
        int changeContrast();
        int whatNext();
        int setName();
        void printName();
        void inGameMenu();
        void replay();
        void endGame();
        void postGame();
        void preLevel();
        void showInfo();
        void showInfo2();

};

#endif // GAME_H
