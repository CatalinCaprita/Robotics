# Introduction
This is the final project of the Introduction To Robotics course.The project represents an interactive applpication that consolidates what the Introduction To Robotics course aims at : understanding of basic robotics notions such as the relation between differnet physical microcontrollers and modules and the Arduino IDE .
## Table of Contents
---
* [General Info](#general-info)
* [Algorithms and Concepts](#algorithms-and-techniques)
* [Demonstrative Video](#demonstrative-video)
* [Technologies](#technologies)
* [Hardware Components](#hardwarware-components)

## General Info
---
The Robot uses a simple maze-solving algorithm to escape from a non-self-looping Maze. 
### Video
[Link]Demo(https://www.youtube.com/watch?v=U3ynA-OqjHk)
## Algorithms and Concepts
---
The left-hand algorithm implmented into the robot's board follows a simple rule, an anaolgy of "keeping one's hand on the left wall of a maze :
* if there is no left wall, then it should turn left;
* if there is a left wall, then:
	* if there is no front wall, then it should keep moving forward
	* if however there is a front wall , but no right wall, then it should turn right
	* otherwise, turn around 180 degrees
The implementation also uses PID control for the motors that control the wheels such that, the maze-solver can "predict" with reasonable accuracy when to begin turning in a certain direction, so as to avoid collisions.
## Technologies
---
* Arduino IDE 1.8.13
## Hardware Components
---
1. Arduino UNO Board
2. L293d Motor Shield
3. 3x Ultrasonic Sensors
4. 4x DC Motors
5. Wires
6. Breadboard
7. Wheels
![Front](https://imgur.com/a/haay68e)
![Sides](https://imgur.com/a/tLnTqJQ)
![Back](https://imgur.com/a/Lr6cX5n)