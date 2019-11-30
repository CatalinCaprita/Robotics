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

/*------------------------------*/
Laboratory No.5 file: Lab5/Lab5.ino
	There are a number of function which characterizes every state of the menu :
I. The "void mainMenu()" displays the main menu options on the lcd using lcd.print() and lcd.clear()
The "showCrt(const int cursorLine,const int cursorPos)" function is used to set the cursor on the line and
column indicated by the parameters given and it updated according to it. 
Each time a movement of the X-axis joystick is detected , i.e. if the valX read from pinX(A0) is greater than
maxThreshold or smaller than minThreshold, the cursor position is updated via the showCrt() function. This happens
for every menu section, either mainMenu or options Menu;

	The boolean "selected" is used to keep track of an option being selected either Play or Options form the LCD.
 A detection is recognized via the readSW() function which is essentailly a debounce paradigm to record whenever 
change of the state of the SW button from the joystick happens. If the button is pressed, the function returns 1,
or 0 otherwise.
	If "selected" is false, it means no option has been chosen , thus the mainMenu configurations is displayed.
	Otherwise, either "Play" or "Options" has been selected, in which case, selected becomes true, and so
either one of the booleans "playing" or "options" is set to true.

II. The "void opts()" function displays the Options menu, in which the starting level can be selected.
The same mechanism of choice is used with the showCrt() function diplaying the current position of the selective
cursor and the readSW() is used to record a press of a button.The "opts()" function is called in loop() only if selected has been 
set to true.
	The global boolean optSelected becomes true if and only if the button has been pressed, i.e. readSW() returns 1,
and the cursor was positioned on the "Set Starting Level" option, i.e. line 0 row 0 on the LCD.
	II.a) The "void setLevel()" function displays the currentStartingLevel variable and , by means of the same mechanisms using
the analogRead() of pinX , if the joystick is moved upwards, i.e, valX >= maxThreshold, then the currentStartingLevel variable is incremented
and displayed on the screen. Otherwise, if the joystick is moved downwards, i.e valX <= minThreshold,the currentStartingLevel variable is decremented
Usign the readSW() function, if the button has been pressed, the startLevel global variable is set to the value of currentStartingLevel.
This funciton is only called when the optSelected variable is true.Once the button is pressed, optSelected becomes false, which on the LCD 
means that we are returning to the configuration rendered by the opts() function.
	II.b) If the button has been pressed and Exit on the LCD has been selected, meaning the cursor is on line 1 column 0 of the matrix,
then selected = false,meaning by the next iteration of the loop() function, the configuration rendered on the LCD will be the one of mainMenu();

III.The "inGame()" function renders as requested, the number of Lives, the Score of the player, memorised using the variable currentLevel multiplyed by 3
as well as the current level. This function is called only if the selected variable is true, and when the button was pressed, the cursorLine was 1 
and cursorPos was 0. Each time a new game starts, the lastUpdate variable is used to keep track of how many seconds have been spent in the game.
if the differnece between current time ( millis() ) and the lastUpdate is greater than a second, it means that inGameTime variable is incremented.
Once inGameTime is greater than the const int maxGameTime, it means the game must end.Thus, the variable "playing" is set to false and so is the
variable "selected" so the LCD can display again the mainMenu configuration.Before "exiting: the game we check if the currentScore is greater than
the highest score memorised int the global variable highScore. If so, a message is shown and highScore is updated.
finally, the endGame() function is called which renders a congrationating message.