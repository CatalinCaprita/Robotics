Laboratory Homeworks
/*--------------------*/
Laboratory No1. file /Lab2/RGBPot/RGBPot.ino
	We did set up 3 digital pins, for Red, green and blue colors.
potPinR (A0) for red

potPinG (A1) for green

potPinB (A2) for blue
Read each of the analogvlaues provided by the the potPins using analogRead .Then I used the digitalWrite
Function threefold for each of the 3 pins read.

/*--------------------*/
Laboratory No3. file : Lab3/Buzzer/Buzzer.ino
	First Off I Created an additional file with all the values for the frequency of all the pithces using #DEFINE-s
Then I created an array of int called melody[] which I populated with the notes i wanted the tone to play
After having establighed the porper setup on the Arduino, the 
speakerPin  = A0 is the pin from which to read the value of the Passive Buzzer
buttonPin = 10 is D10 the pin from which to read the value provided by the pushButton
buzzerPin = D9 the pin to provide digital output for the active buzzer

currentTime - records the moment when a knock was detected using millis()
lastTime - keeps track of the last time a knock was detected
Thus the delay() function is simulated via the condition (currentTime - lastTime >= interval)
A KNOCK is detected when speakerValue is an analog value greater than the "limit". If this "limit" has been
surpassed then we check if the last time since the last reading of a knock is greater than the value of 
"interval".
If so, the for loop iterates over the melody[] array and sets the tone to be played on the active buzzer
by means of the function tone().After each tone is played, we set a pause between notes that is equal
to the duration *1.30. After the set delay, we use noTone() for the buzzerPin to stop the input.
Also , we keep reading digitalInput from the buttonPin to check if it is pressed. If so, we break the loop.
Finally , record this time as lastTime of a KNOCK being detected.

/*--------------------------*/
Laboratory No.4 file : Lab4/Lab4.ino

	The idea on the code is to maintain a boolean "locked" value in order t check for every iteration if
a certain display must display a number via the Y-axis movement of the joystick.
	A 7-segment dispaly is memorized in an array of 8 elements  segments[]
	Each of the 4 displays is memorised in an array of 4 elements digits[]
	Each digit's configuration for a 7-segment is memorisez in a 2D array digitcode[10][]

During each iteration of the loop():
First the locked state is checked.
	If the locked value is TRUE, it means that in previous iterations we locked on the currentDisplay
thus we are not intersted in analogRead()-ing the valX from the joystick so it is ignored.Instead, 
"valY" is of interestet because if the "valY" indicates that the joystick has moved via the "movedY" variable
we decrease or increase the variable "currentDigit" accordingly and call the " displayNumber()" function 
to display the "currentDigit " on the current 7-segment;
	If the "locked" value is FALSE, it means that the currentDisplay to be locked is still being chosen.
Therefore, each iteration reads the "valX" via analogRead() and similarly to the "valY" checks whether the 
value indicates a movement of the joystick using the "movedX" variable. If so, the currentDisplay value increases
or decreases accordingly and the DP of that display is lit.
	The "locked" state is triggered via the press of the swButton. A debounce method is used to read
the sw signal via "swRead" and if the "swState" is "LOW", i.e the button is pressed, we either lock on the 
currentDisplay, meaning via the functinon onDigit() we turn off the other displays and turn on the selected one.
	This way, we check continuously for the locked state and read what value we are interested in.  
