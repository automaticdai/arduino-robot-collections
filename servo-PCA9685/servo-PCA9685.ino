#include "FaBoPWM_PCA9685.h"

#include <Firmata.h>
FaBoPWM faboPWM;

int SPEED_MAX = 9;
int SPEED_MIN = -9;

int X_MAX = 450;
int X_MIN = 150;
int Y_MAX = 450;
int Y_MIN = 150;

int xpos = 280;
int ypos = 320;
int xvel = 0;
int yvel = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // init PCA9685
  if(faboPWM.begin()) {
    faboPWM.init(300);
  }
  faboPWM.set_hz(50);

  // reset to default position
  faboPWM.set_channel_value(0, xpos);
  faboPWM.set_channel_value(1, ypos);
}


void loop() {
  
  xpos += xvel;
  if (xpos > X_MAX) xpos = X_MAX;
  if (xpos < X_MIN) xpos = X_MIN;

  ypos += yvel;
  if (ypos > Y_MAX) ypos = Y_MAX;
  if (ypos < Y_MIN) ypos = Y_MIN;
  
  faboPWM.set_channel_value(0, xpos); 
  faboPWM.set_channel_value(1, ypos);
  
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    String xvel_str = getValue(inputString, ':', 0);
    String yvel_str = getValue(inputString, ':', 1);
    xvel = xvel_str.toInt();
    yvel = yvel_str.toInt();
    
    if (xvel > SPEED_MAX) xvel = SPEED_MAX;
    if (xvel < SPEED_MIN) xvel = SPEED_MIN;
    if (yvel > SPEED_MAX) yvel = SPEED_MAX;
    if (yvel < SPEED_MIN) yvel = SPEED_MIN;
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  delay(10);
}
