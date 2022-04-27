/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Use of timer and buzzer
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-5
 */

//Include library file
#include <MswTimer2.h>

// Define the buzzer using the 9 pin of the M328 PRO.
byte BuzzerPin   = 9;  

// Set frequency 
int frequency = 880;

// Set interrupt time, 1S = 1000mS
int time = 1000/frequency; 

// Interrupt function
void tone(void) {  
  // The program is initialized only once when it runs.
  static boolean output = HIGH;
  
  digitalWrite(BuzzerPin, output);
  output = !output;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(BuzzerPin, OUTPUT);
  
  // Set interrupt function
  MsTimer2::set(time, tone); 
  
  // Start interrupt function
  MsTimer2::start();
  
  // Stop interrupt function
  // MsTimer2::stop();
}

// the loop function runs over and over again forever
void loop() {
}


