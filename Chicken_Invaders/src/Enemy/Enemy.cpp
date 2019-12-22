#include "Enemy.h"
#include "Arduino.h"
#include "LedControl.h"

Enemy::Enemy(byte startX, byte startY,byte live): posX(startX),posY(startY),lives(live),blinkInterval(20),lastBlink(0),shootX(startX),shootY(startY),shooting(false),active(false),
lastBullet(0),blinkState(1),bulletInterval(175)
{
}

Enemy::~Enemy()
{

}

void Enemy::updatePos(int offSetX, int offSetY,const int mapSize,LedControl &lc)
{


        lc.setLed(0,posY,posX,false);
        if(posX + offSetX < 0)
            posX = mapSize - 1;
        else
            posX = (posX + offSetX) % mapSize;
        if(posY + offSetY < 0)
                posY = 0;
        else
            if(posY + offSetY >= 5)
                posY = 0;
            else
                posY += offSetY;
        lc.setLed(0,posY,posX,true);



}

void Enemy::travellingBullet(LedControl &lc)
{

            if(millis() - lastBullet >= (bulletInterval))
            {
                 lc.setLed(0,shootY,shootX,false);
                shootY ++;
                //Will check afterwards if bullet hit a target or went off
                //scrren
               lastBullet = millis();
               lc.setLed(0,shootY,shootX,true);
            }


}

void Enemy::lifeBlink(LedControl &lc)
{
        if(millis() - lastBlink >= (blinkInterval))
        {
            blinkState != blinkState;
            lc.setLed(0,shootY,shootX,blinkState);
            lastBlink = millis();
        }
}
