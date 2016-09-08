# Setup

* Connect Arduino Yun board to computer (not working well with Galileo)

* Upload compiled sketch from Arduino IDE to the board. 

# Connection
  
  1. Servo moter PWD pin: d9;
  
  2. Switch: d7;
  
  3. DotStar: data d4, clock d5;
  
  4. LED positive: d12.
  
  5. A0 reserved for random seed, must be left unattached;
  
  6. Softspot: A1;
  
  7. FlexSensor: A3. 
  
# How to play
  
  * Press switch to start/end a round;
  
  * Press softspot to change color;
  
  * Blend flex sensor strip to trigger sucessful match. 
