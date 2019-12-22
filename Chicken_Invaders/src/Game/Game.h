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
//The Game class is a wrapper class for an on-going game
class Game
{
    //Map m;
    Enemy eArray[MAPSIZE * 2];
    Player player;
    Joystick j;
    LedControl lc;
    LiquidCrystal menu;
    char *playerName;
    int idx[MAPSIZE * 2];
    byte currentLevel;
    int contrastVal;
    int eSize;
    int nameSize;
    int minSpawn;
    int hiScore;
    int contrastPin;
    char addFactor;
    bool nextMove;
    bool inGame;
    byte currentPosition;
    byte activeEnemies;
    byte maxLifeCap;
    byte eProjectiles;
    byte cursorPosition;
    byte cursorLine;
    const byte mapSize;
    static const byte levels[LEVELS][MAPSIZE * MAPSIZE];
    static const int dx[LEVELS][LEVELS];
    static const int dy[LEVELS][LEVELS];
    const long movementDelay;
    const double fireDelay;
    const long selectedDelay;
    const long scrollSensitivity;
    long lastScroll;
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
        //void showCrsName(const byte, const byte);
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
