/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Button
	2. Variable: char, global variable
	3. Assignment operator: =
	4. Digital pin input Settings
	5. Relational operator: ==, !=
	6. Judgment statement: if

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-16
*/
#include<wiringPi.h>

// Global variable.
char buttonPin = 22;   // the number of the pushbutton pin
char ledPin = 23;      // the number of the LED pin
char buttonState = 0;  // variable for reading the pushbutton status


int main(void) {
	wiringPiSetup();
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);

	while(1){
		// read the state of the pushbutton value:
		buttonState = digitalRead(buttonPin);

		// check if the pushbutton is pressed. If it is, the buttonState is LOW:
		if (buttonState == LOW) {
		// turn LED on:
		digitalWrite(ledPin, HIGH);
		}
		if (buttonState != LOW){
		// turn LED off:
		digitalWrite(ledPin, LOW);
		}
	}
}
