/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo myservo_lr;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
Servo myservo_ud;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards

#define SERVO_UP_LIMIT (180)
#define SERVO_DOWN_LIMIT (0)

int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo_lr.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo_ud.attach(6); 
  myservo_ud.write(170);
} 
 
void loop() 
{ 
  for(pos = SERVO_DOWN_LIMIT; pos <= SERVO_UP_LIMIT; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo_lr.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = SERVO_UP_LIMIT; pos>=SERVO_DOWN_LIMIT; pos -= 1)     // goes from 180 degrees to 0 degrees 
  {                                
     myservo_lr.write(pos);              // tell servo to go to position in variable 'pos' 
     delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 
