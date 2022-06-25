/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
    1. Using library files
	2. Timer
	3. Variable: bool 
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-20
 */
//Include library file
#include <MswTimer1.h>

// Defining character variables: 
const byte ledPin = 5;               // the number of the LED pin(Blue LED D5)
// The program is initialized only once when it runs.
bool output = true;
  

// Interrupt function
void flash(void) {  
  digitalWrite(ledPin, output);
  output = !output;
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // Set interrupt function
  // 500000us = 500ms = 0.5s
  Timer1.initialize(500000);
  Timer1.attachInterrupt(flash); // flash to run every 0.5 seconds
  
  // run control
  // Timer1.stop();
  // Timer1.start();
}


// the loop function runs over and over again forever
void loop() {
}


