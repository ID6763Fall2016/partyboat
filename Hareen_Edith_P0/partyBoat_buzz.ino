#include <Servo.h>

Servo myservo;

int sensorPin = A0;
float sensorValue; // the analog reading from the soft potentiometer
 

int ledPin = 13; //LED pin connected to pin 13
int ledPin1 = 11; //LED pin connected to pin 11
int servoPin = 9; //servo connected to pin 9
int rangePin = A1; //range sensor connected to analog pin 1
float rangeSensorValue = 0;
 
int sensorValueForLed = 0;
int sensorValueForLed1 = 0;
int lastChange;
int lastChange1; // the time of changing pin state
int state = HIGH;
int state1 = HIGH;
 
int potMin = 0;    // starting value of the pot.  
int potSpan=  700;
            
                   
void setup() 
{
  Serial.begin(9600);   
  myservo.attach(servoPin);  // pin the servo is connected to
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);

  lastChange = millis();
  lastChange1 = millis();  // ledpin state changed NOW
  digitalWrite(ledPin, state);
  digitalWrite(ledPin1, state1);
 
}
 
void loop()
{

  //source of this part: http://forum.arduino.cc/index.php?topic=42212.0
  sensorValue = analogRead(sensorPin);   // Reads the potentiometer
  Serial.print("force reading = "); 
  Serial.println(sensorValue ); // This will print the raw force value
  constrain(sensorValue, potMin, potMin + potSpan);
  int angle = map(sensorValue, potMin, potMin + potSpan, 0, 179); 
  Serial.print("adjusted reading = "); 
  Serial.println(angle); 
  myservo.write(angle); // Write the new angle to the servo
  delay(150); // Delay 150 milliseconds before taking another reading


  //when the soft potentiometer is pressed, read the sensor value and output as the blue led
  //upper part -> blink faster; lower part -> blink slower
  sensorValueForLed1 = map(analogRead(sensorPin), 0, 1023, 500, 5);  
  if (lastChange1 +  sensorValueForLed1 <= millis()) 
  {
      state1 = !state1; // invert led's state
      lastChange1 = millis();  
      digitalWrite(ledPin1, state1);
   }

  //when the range sensor is covered, read the sensor value and output as green led 
  //covered -> blink slower; not covered -> blink faster
  sensorValueForLed = map(analogRead(rangePin), 0, 1023, 500, 5);  
  if (lastChange +  sensorValueForLed*0.4 <= millis()) 
  {
      state = !state; // invert led's state
      lastChange = millis();  
      digitalWrite(ledPin, state);
   }
 
} 
