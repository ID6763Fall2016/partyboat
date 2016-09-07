int pinLight = A0;
int thresholdValue= 100;
int speakerPin=8; 

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int length = 15; 
char notes[] = "ccggaagffeeddc "; 
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };  //song tune
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 }; //note frequencies

    // play tone corresponding to note
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

void setup()
{
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);

   lcd.begin(16, 2);
    
   delay(1000);
  
}


void loop()
{
  int sensorValue = analogRead(pinLight);
  if(sensorValue>thresholdValue) 
  { 
  lcd.setRGB(0, 255, 0); //LCD is green
  lcd.setCursor(0, 0);
  lcd.print("Dim the lights");
  delay (100);
  }
  
  
  else 
  {
      for (int i = 0; i < length; i++) 
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2);  
  lcd.setRGB(255, 255, 0); //LCD is yellow
  lcd.setCursor(0,0);
  lcd.print("PAAAARTY TIME!");
  delay(100);
  }

  delay(1000);
}
}

