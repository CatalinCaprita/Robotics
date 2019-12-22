#include "Arduino.h"
#include "LedControl.h"
#ifndef ENEMY_H
#define ENEMY_H

class Game;
class Enemy
{
    byte posX;
    byte posY;
    byte shootX;
    byte shootY;
    byte lives;
    byte blinkState;
    bool shooting;
    bool active;
    const long blinkInterval;
    const long bulletInterval;
    long lastBlink;
    long lastBullet;
    friend class Game;
    public:
        Enemy(byte = 0,byte = 0,byte = 1);
        virtual ~Enemy();
        void updatePos(int,int,const int,LedControl &);
        byte livesLeft(){return lives;};
        byte getX(){ return posX;}
        byte getY(){ return posY;}
        byte getShootX(){ return shootX;}
        byte getShootY(){ return shootY;}
        void shoot()
        { shooting = true;
            shootX = posX;
            shootY = posY;}
        void travellingBullet(LedControl &lc);
        void doneShoot(){ shooting = false; active = false;}
        bool hasShot() { return shooting;}
        void lifeBlink(LedControl &);
};

#endif // ENEMY_H
