#include "Arduino.h"
#ifndef JOYSTICK_H
#define JOYSTICK_H

/* The Joystick Class is a simple wrapper for all the functionalities
of this hardware component as taught in the course:
 - Reading of the Movement on X-axis
 - Reading of the Movement on Y-axis
 - Reading of the state of the SW Button
*/
class Joystick
{
    int valX;
    int valY;
    bool movedX,movedY;
    bool swState; // boolean variable to be used for the debounce code
    bool lastSwState;
    long lastDebounce;
    long lastReadX;
    long lastReadY;
    const int maxThreshold; // sets the sensitivity of the movement detection on either X or Y axis
    const int minThreshold;
    const long debounceDelay;
    const long axisReadTime;
    int swRead;

    // Arduino Pins to be initialized
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
