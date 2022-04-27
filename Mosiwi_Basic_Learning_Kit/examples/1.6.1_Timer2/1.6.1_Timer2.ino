/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Timer
	2. Using library files
	3. Variable modifier: static
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-20
 */


//Include library file
#include <MswTimer2.h>

// Defining character variables: 
const byte ledPin = 5;               // the number of the LED pin(Blue LED D5)


// Interrupt function
void flash(void) {  
  // The program is initialized only once when it runs.
  static bool output = HIGH;
  
  digitalWrite(ledPin, output);
  output = !output;
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // Set interrupt function
  MsTimer2::set(500, flash); // 500ms period
  
  // Start interrupt function
  MsTimer2::start();
  
  // Stop interrupt function
  // MsTimer2::stop();
}


// the loop function runs over and over again forever
void loop() {
}


