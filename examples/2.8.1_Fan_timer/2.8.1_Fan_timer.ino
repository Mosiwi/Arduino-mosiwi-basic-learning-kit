/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. fan

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
 */
// Use timer 1, generate 400 Hz PWM signal
#define FAN_USE_TIMER1
// Use timer 2, generate 80 Hz PWM signal
//#define FAN_USE_TIMER2

//Include library file
#include <MswFan.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // Fan control pins: 5, 6
  MswFan::init(5, 6);

  // Setting fan run
  //MswFan::run();
  
  // Setting fan stop
  //MswFan::stop();
}

// put your main code here, to run repeatedly:
void loop() {
  int i;
  
  // clockwise rotation
  MswFan::direction(CW);
  for(i = 0; i<150; i++){
	// Speed allowed range: 0-255
    MswFan::speed(i);     
    delay(10);	
  }
  for(i = 150; i>0; i--){
    MswFan::speed(i); 
    delay(10);	
  }
  
  // Counterclockwise rotation
  MswFan::direction(CCW);
  for(i = 0; i<150; i++){
    MswFan::speed(i);     
    delay(10);	
  }
  for(i = 150; i>0; i--){
    MswFan::speed(i); 
    delay(10);	
  }
}

