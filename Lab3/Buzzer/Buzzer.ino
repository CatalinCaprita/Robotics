#include "pitches.h"
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
const int speakerPin = A0;
const int buttonPin = 10;
const int buzzerPin = 9;
const int limit = 1;
int currentTime;
int lastTime;
int interval = 5000;
int speakerValue;
int buttonState;
void setup() {
 Serial.begin(9600);
 pinMode(speakerPin,INPUT);
 pinMode(buttonPin,INPUT);
 pinMode(buzzerPin,OUTPUT); 
}

void loop() {
    //Read the Knock
    speakerValue = 5;//analogRead(speakerPin);
    currentTime = millis();
    if(speakerValue != 0)
    Serial.println(speakerValue);
    Serial.print(" ");Serial.println(buttonState);;
    
    
    //We detected a Knock
    if(speakerValue >= limit)
    {
      Serial.println("KNOCK");
      //if we waited 5s
      if(currentTime - lastTime>= 5000) 
      {
       for (int thisNote = 0; thisNote < 8; thisNote++) {

               buttonState = digitalRead(buttonPin);
                if(buttonState == HIGH)
                break;
             // to calculate the note duration, take one second divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(buzzerPin, melody[thisNote], noteDuration);
      
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
            int pauseBetweenNotes = noteDuration * 1.30;
            delay(pauseBetweenNotes);
          // stop the tone playing:
            noTone(buzzerPin);
            }
          lastTime = currentTime;
      }
      
      }
      
}
