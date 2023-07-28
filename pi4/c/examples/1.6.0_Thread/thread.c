/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Thread
	2. Logical operator: !
	3. Variable: bool
	4. Variable modifier: static

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-20
 */
//Include library file
#include <wiringPi.h>
#include <stdio.h>
#include <stdbool.h>

// Defining character variables:
const char ledPin = 23;               // the number of the LED pin

// Thread function
PI_THREAD (flashThread){
	while(1){
		// The program is initialized only once when it runs.
		static bool output = true;
		digitalWrite(ledPin, output);
		output = !output;
		delay(1000);
	}
}


int main(void){
	wiringPiSetup();
	pinMode(ledPin, OUTPUT);

	char x = piThreadCreate(flashThread);
	if (x != 0){
		printf ("Thread initialization failed! \n");
		return 0;
	}

	while(1);
}


