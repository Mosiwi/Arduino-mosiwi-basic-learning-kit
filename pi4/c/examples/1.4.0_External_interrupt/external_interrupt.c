/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. External interrupt
	2. Variable modifier: const, volatile
	3. Write a function with no return value

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include<stdio.h>
#include<wiringPi.h>

// Defining character variables. They're used to give names to the pins used:
const char ledPin = 23;              // the number of the LED pin
const char interruptPin = 22;        // the number of the pushbutton pin

// variables will change:
volatile char state = 0;

// Interrupt function with no return value
void blink(void) {
	state = 1 - state;
}


int main(void) {
	wiringPiSetup();
	// Initialize the LED pin as an output:
	pinMode(ledPin, OUTPUT);

	// Interrupt mode: defines when the interrupt should be triggered. Four constants are predefined as valid values:
	// INT_EDGE_FALLING for when the pin goes from high to low.
	// INT_EDGE_RISING to trigger when the pin goes from low to high.
	// INT_EDGE_BOTH high to low and low to high
	// INT_EDGE_SETUP
	char INT = wiringPiISR(interruptPin, INT_EDGE_FALLING, blink);
	if(INT != 0){
		printf("Interrupt function initialization failed! \n");
		return 0;
	}

	//Loop statement
	while(1){
		while(state == 0){
			digitalWrite(ledPin, LOW);
		}
		digitalWrite(ledPin, HIGH);
	}
}



