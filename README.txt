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

 