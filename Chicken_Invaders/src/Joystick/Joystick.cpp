#include "Joystick.h"
#include "Arduino.h"


Joystick::Joystick(byte pX = A0, byte pY = A1, byte pSw = A2):debounceDelay(100),lastDebounce(0),movedX(false),movedY(false),swState(1),lastSwState(1),maxThreshold(550),minThreshold(400),axisReadTime(150),lastReadX(0),lastReadY(0)
{
    pinX = pX;
    pinY = pY;
    pinSw = pSw;
    pinMode(pinX,INPUT);
    pinMode(pinY,INPUT);
    pinMode(pinSw,INPUT_PULLUP);
}

Joystick::~Joystick()
{
}

int Joystick::readMoveX()
{
    if(millis() - lastReadX >= axisReadTime)
    {
        valX = analogRead(pinX);
      if(valX >= maxThreshold && movedX == false)
        {
          movedX = true;
            return 1; // meaning the position must be updated to right;
        }
      if(valX <= minThreshold && movedX == false)
        {
                movedX = true;
        return -1; //meaning position must be updated to left
        }
        else
         {
            movedX = false;
            return 0; // no movement detected;
         }
    lastReadX = millis();
   }
    return 0;
}
int Joystick::readMoveY()
{
    if(millis() - lastReadY >= axisReadTime)
    {
        valY = analogRead(pinY);
      if(valY >= maxThreshold && movedY == false)
        {
          movedY = true;
            return 1; // meaning the position must be updated to right;
        }
      if(valY <= minThreshold && movedY == false)
        {
                movedY = true;
        return -1; //meaning position must be updated to left
        }
        else
         {
            movedY = false;
            return 0; // no movement detected;
         }
    lastReadY = millis();
    }
    return 0;
}

int Joystick::debounce()
{
    swRead = digitalRead(pinSw);

   // If there is any noise altering the state of the sw, then update the last
//    if(lastSwState != swRead)
//    {
//        lastDebounce = millis();
//        Serial.println("BUTTON NOISE");
//    }

    //If enough time has actually passed since the last reading, update the reading

    if(millis() - lastDebounce >= debounceDelay)
    {

        if(swState != swRead)
        {
            swState = swRead;
        }
        //if The button has actually been pressed
        if(swRead == 0)
        {
            lastSwState = swState;
            return 1;
        }
        lastDebounce = millis();
    }
    lastSwState = swRead;
    return 0;
}
