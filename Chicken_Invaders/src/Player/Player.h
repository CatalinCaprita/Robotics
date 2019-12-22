#include "Arduino.h"
#include "LedControl.h"
#ifndef PLAYER_H
#define PLAYER_H
#define SPAWNX 4
#define SPAWNY 7
/*The Player class is used to represent all that the player could do
 - Shooting a projectile towards enemies
 - Moving left-right, up-down
 - Die
 - Respawn
 */

class Game;
class Player
{
    byte posX;
    byte posY;
    byte startX;
    byte startY;
    byte shootX;
    byte shootY;
    int score;
    int lives;
    bool shooting;
    const long blinkInterval;  //the rate at which the bullet travels
    const long updateInterval; // the speed at which the player changes positon on the map
    long lastBlink;
    long lastBullet;  //keep track of the last time a bullet was shot or its position has been updated
    long lastOn;
    long lastUpdate;  //keep track of the last time the player positon has been updated
    friend class Game;
    public:
        Player(int = SPAWNX,int = SPAWNY);
        virtual ~Player();
        void updatePos(int,int,const int,LedControl &);
        byte livesLeft(){return lives;};
        byte getX(){ return posX;}
        byte getY(){ return posY;}
        byte getShootX(){ return shootX;}
        byte getShootY(){ return shootY;}
        void shoot(){ shooting = true;
                    shootX = posX;
                    shootY = posY;}
        void travellingBullet(LedControl &lc);
        void doneShoot(){ shooting = false;}
        bool hasShot() { return shooting;}
        void respawn(LedControl &);
};

#endif // PLAYER_H
