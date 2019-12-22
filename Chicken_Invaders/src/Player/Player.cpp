#include "Player.h"
#include "Arduino.h"


Player::Player(int X, int Y): posX(X),posY(Y),score(0),lives(3),blinkInterval(75),shootX(startX),shootY(startY),shooting(false),
startX(X),startY(Y)
{
//    Serial.println("Player Spawned!");
}

Player::~Player()
{

}

void Player::updatePos(int offSetX, int offSetY,const int mapSize,LedControl &lc)
{

    if(millis() - lastUpdate >= updateInterval)
    {

        lc.setLed(0,posY,posX,false);
        if(millis() - lastBlink >= blinkInterval)
        {

            lastBlink = millis();
        if(posX + offSetX < 0)
            posX = 7;
        else
            posX = (posX + offSetX) % mapSize;
        if(posY + offSetY > 7)
                posY = 7;
        else
            if(posY + offSetY >= 5)
                posY += offSetY;
        lastBlink = millis();

        }
         lc.setLed(0,posY,posX,true);
    }


}

void Player::travellingBullet(LedControl &lc)
{

        if(millis() - lastBullet >= blinkInterval * 2)
        {
             lc.setLed(0,shootY,shootX,false);
            shootY --;
            //Will check afterwards if bullet hit a target or went off
            //scrren
           lastBullet = millis();
           lc.setLed(0,shootY,shootX,true);
        }

}
void Player::respawn(LedControl &lc)
{

    lc.setLed(0, posY, posX,false);
    posX = startX;
    posY = startY;
    lc.setLed(0, posY, posX,true);
}
