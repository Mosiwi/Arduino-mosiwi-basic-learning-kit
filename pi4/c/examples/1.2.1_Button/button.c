/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Variable: local variable
	2. Judgment statement: if...else..., if...else if...else...

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include<wiringPi.h>

// Global variable.
char buttonPin = 22; // the number of the pushbutton pin
char ledPin = 23;    // the number of the LED pin

int main(void){
	wiringPiSetup();
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);

	// local variable
	char buttonState = 0;

	while(1){
		// read the state of the pushbutton value:
		buttonState = digitalRead(buttonPin);

		// check if the pushbutton is pressed. If it is, the buttonState is LOW:
		if (buttonState == LOW) {
			// turn LED on:
			digitalWrite(ledPin, HIGH);
		}
		else {
			// turn LED off:
			digitalWrite(ledPin, LOW);
		}

		/************************Another judgment statement**********************
		// check if the pushbutton is pressed. If it is, the buttonState is LOW:
		if (buttonState == LOW) {
			// turn LED on:
			digitalWrite(ledPin, HIGH);
		}
		else if(buttonState == HIGH){
			// turn LED off:
			digitalWrite(ledPin, LOW);
		}
		else{
			// statement
		}
		************************************************************************/
	}
}
