//ONG LIT YIT 2013-07-20
//This following code plays happy birthday melody on Arduino
//Put Piezo Buzzer on GDN and 9 (Positive and negative are reversible)
//this project requires a Piezo Buzzer and

// an Arduino board and

//jumper wires to connect Buzzer's (+) to ~9 and (-) to GND (any GND)

//HAVE FUN

#include <Adafruit_NeoPixel.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_NeoMatrix.h>
#include <Adafruit_DotStar.h>
#include <SPI.h> 
#include <Servo.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS 9
#define DATAPIN    12
#define CLOCKPIN   2

 int speakerPin = 11;
 int LEDG = 6;
 int LEDY = 5;
// int neopixelPin = 6;
 int candlePin = 3;
 int photocellPin = 3;
 int photocellReading;
 int flexposition;
 int flexPin = 1;
 int FSR_PIN = 0;
int servoPin = 10;
int fsrReading;
int angle;
Servo servo;
int servoPosition;
int flagS=0;
 
 int flag = 0;
 int flagTone = 1;
 
 int j=0;
  
 int length = 28; // the number of notes
  
 char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
  
 int beats[] = { 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8,8, 16, 1, 2,2,8,8,8,8,16, 1,2,2,8,8,8,16 };
  
 int tempo = 150;
 
 Adafruit_DotStar strip = Adafruit_DotStar(
  9, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
 
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, neopixelPin, NEO_GRB + NEO_KHZ800);
//   Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, neopixelPin,
//  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
//  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
//  NEO_GRB            + NEO_KHZ800);
  
//  const uint16_t colors[] = {
//  matrix.Color(255, 0, 0), matrix.Color(0, 0, 0), matrix.Color(0, 0, 255) };
  
 void playTone(int tone, int duration) {
   digitalWrite(LEDG,HIGH);
   digitalWrite(LEDY,LOW);
//   matrix.setTextColor(colors[0]);
//      strip.setPixelColor(j%9,200,0,255);  
   for (long i = 0; i < duration * 1000L; i += tone * 2) {
  
     digitalWrite(speakerPin, HIGH);
  
     delayMicroseconds(tone);
     
//     strip.setPixelColor(j%9,255,165,0);
//     j++;
    
     digitalWrite(speakerPin, LOW);
  
     delayMicroseconds(tone);
    
   }
//   j++;
//   strip.setPixelColor(j%9,200,0,255);
//   strip.show();
  digitalWrite(LEDG,LOW);
  digitalWrite(LEDY,HIGH);
//    matrix.setTextColor(colors[1]);
 }
  
 void playNote(char note, int duration) {
  
   char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',           
  
                   'c', 'd', 'e', 'f', 'g', 'a', 'b',
  
                   'x', 'y' };
  
   int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
  
                   956,  834,  765,  593,  468,  346,  224,
  
                   655 , 715 };
  
   int SPEE = 5;
  
  // play the tone corresponding to the note name
  
   for (int i = 0; i < 17; i++) {
  
     if (names[i] == note) {
      int newduration = duration/SPEE;
       playTone(tones[i], newduration);
  
      }
  
    }
  
  }
  
  void buzzer(){
    Serial.println("buzzer");
    for (int i = 0; i < length; i++) {
  
     if (notes[i] == ' ') {
  
       delay(beats[i] * tempo); // rest
  
       } 
     else {
  
       playNote(notes[i], beats[i] * tempo);
  
       }
  
     // pause between notes
//     strip.setPixelColor(j%9,200,0,255);
//    j++;
//    strip.show();
     delay(tempo);
  
    }
    
  }
  
 void setup() {
   Serial.begin(9600);
   pinMode(speakerPin, OUTPUT);
   pinMode(LEDG, OUTPUT);
   pinMode(LEDY, OUTPUT);
   pinMode(candlePin,OUTPUT);
   servo.attach(servoPin);
//   pinMode(neopixelPin,OUTPUT);
   
   strip.begin();
   strip.show(); // Initialize all pixels to 'off'
//     matrix.begin();
//     matrix.setTextWrap(false);
//     matrix.setBrightness(40);
//     matrix.setTextColor(colors[0]);
  
 }
  
 void loop() {
//   matrix.fillScreen(0);
//   matrix.setCursor(matrix.width(), 0);
   photocellReading = analogRead(photocellPin);
//   photocellReading = 1023 - photocellReading;
//   Serial.println(photocellReading);
   flexposition = analogRead(flexPin);
   Serial.println(flexposition);
   
   fsrReading = analogRead(FSR_PIN);
//   Serial.println(fsrReading);
   angle = map(fsrReading,0,1023,0,180);
//   Serial.println(angle);
   if(angle>50&&flagS==0){
     servo.write(angle);
     flagS=1;
     servoPosition = angle;
   }
   else if(flagS==1){
     servo.write(servoPosition);
//     Serial.println(servoPosition);
   }
   else{
     servo.write(angle);
   }
   
   if(photocellReading<800&&flag==0){
     digitalWrite(candlePin,HIGH);
   }
   else if(photocellReading>=800&&flag==0){
     digitalWrite(candlePin,LOW);
   }
   else if(photocellReading>=800&&flag==1){
     noTone(11);
//     digitalWrite(5,0);
//     digitalWrite(6,0);
     flagTone=0;
   }
   if(flexposition>330&&flag==0){
     digitalWrite(candlePin,LOW);
     flag=1;
   }
   else if(flag==1&&flagTone==1){
     digitalWrite(candlePin,LOW);
     buzzer();
     Serial.println("music");
   }
   else if(flag==1&&flagTone==0){
     digitalWrite(candlePin,LOW);
   }
   else{
     digitalWrite(candlePin,HIGH);
   }
//   j++;
//   for(int i=0;i<9;i++){
//     strip.setPixelColor(j%9,200,0,255);
//   }
//   
//   strip.show();
   
  
  }
