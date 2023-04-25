/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Control the fan
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-21
*/
#include <MswLed.h>

// Use timer 1, generate 400 Hz PWM signal
#define FAN_USE_TIMER1
// Use timer 2, generate 80 Hz PWM signal
//#define FAN_USE_TIMER2
#include <MswFan.h>

// Analog input pin that the Slide potentiometer
byte analogInPin = A3;       

void setup() {
  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);

  // Fan control pins: 5, 6
  MswFan::init(5, 6);
  // clockwise rotation
  MswFan::direction(CW);
}

void loop() {
  // Read the analog value in the potentiometer.
  int analogValue = analogRead(analogInPin);

  // Convert 0-1010 data to 0-150 data.
  byte fanSpeed = map(analogValue, 0, 1020, 0, 150);
  MswFan::speed(fanSpeed);
  
  // Fan speed indicator.
  byte ledNum = map(analogValue, 0, 1020, 0, 8);
  byte displayData = 0;
  while(ledNum > 0){
	// same as: displayData = displayData | (1 << (ledNum-1));
    displayData |= 1 << (ledNum-1);  
	ledNum --;
  }
  SetLedBar(displayData);
}
