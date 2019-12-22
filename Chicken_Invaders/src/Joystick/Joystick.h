#include "Arduino.h"
#ifndef JOYSTICK_H
#define JOYSTICK_H


class Joystick
{
    int valX;
    int valY;
    bool movedX,movedY;
    bool swState;
    bool lastSwState;
    long lastDebounce;
    long lastReadX;
    long lastReadY;
    const int maxThreshold;
    const int minThreshold;
    const long debounceDelay;
    const long axisReadTime;
    int swRead;
    byte pinX;
    byte pinY;
    byte pinSw;

    public:
        Joystick(byte, byte, byte);
        virtual ~Joystick();
        int readMoveX();
        int readMoveY();
        int debounce();
};

#endif // JOYSTICK_H
