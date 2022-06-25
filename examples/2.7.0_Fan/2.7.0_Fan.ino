/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. fan

 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
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
  // Fan control pins: A2, A3
  MswFan::init(A2, A3);

  // Setting fan run
  //MswFan::run();
  
  // Setting fan stop
  //MswFan::stop();
}

// put your main code here, to run repeatedly:
void loop() {
  // clockwise rotation
  int i;
  MswFan::direction(CW);
  for(i = 0; i<160; i++){
	// Speed allowed range: 0-255
    MswFan::speed(i);     
    delay(10);	
  }
  for(i = 160; i>0; i--){
    MswFan::speed(i); 
    delay(10);	
  }
  // Counterclockwise rotation
  MswFan::direction(CCW);
  for(i = 0; i<160; i++){
    MswFan::speed(i);     
    delay(10);	
  }
  for(i = 160; i>0; i--){
    MswFan::speed(i); 
    delay(10);	
  }
}

