#include "Arduino.h"
#include "LedControl.h"
#ifndef PLAYER_H
#define PLAYER_H

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
    const long blinkInterval;
    const long updateInterval;
    long lastBlink;
    long lastBullet;
    long lastOn;
    long lastUpdate;
    friend class Game;
    public:
        Player(int = 4,int = 7);
        virtual ~Player();
        void updatePos(int,int,const int,LedControl &);
        bool hasShield();
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
