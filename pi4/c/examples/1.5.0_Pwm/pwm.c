/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. PWM output port
	2. Loop statement: for
	3. Arithmetic operator: --, ++
	4. Relational operator: >, <, >=, <=

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-20
 */
#include <wiringPi.h>
#include <stdio.h>

// These constants won't change. They're used to give names to the pins used:
const char pwmOutPin   = 1;         // Define the PWM pin
int pwmValue    = 0;                // value output to the PWM (analog out)


int main(void){
	wiringPiSetup();
	pinMode(pwmOutPin, PWM_OUTPUT);
	pwmSetRange(1023);
	pwmSetClock(1000);  //19.2MHz/1000

	// Judgment loop statement
	printf("Test <= 1023 : --------------------------\n");
	delay(1000);
	for(pwmValue = 0; pwmValue <= 1023; pwmValue ++){
		// change the analog out value:
		pwmWrite(pwmOutPin, pwmValue);

		// prints title with ending line break
		printf("%d\n", pwmValue);
		delay(10);
	}

	// Judgment loop statement
	printf("Test >=0 : --------------------------\n");
	delay(1000);
	for(pwmValue = 1023; pwmValue >= 0; pwmValue --){
		// change the analog out value:
		pwmWrite(pwmOutPin, pwmValue);

		// prints title with ending line break
		printf("%d\n", pwmValue);
		delay(10);
	}

	// Judgment loop statement
	printf("Test < 1024 : ---------------------------\n");
	delay(1000);
	for(pwmValue = 0; pwmValue < 1024; pwmValue ++){
		// change the analog out value:
		pwmWrite(pwmOutPin, pwmValue);

		// prints title with ending line break
		printf("%d\n", pwmValue);
		delay(10);
	}

	// Judgment loop statement
	printf("Test > -1 : --------------------------\n");
	delay(1000);
	for(pwmValue = 1024; pwmValue > -1; pwmValue --){
		// change the analog out value:
		pwmWrite(pwmOutPin, pwmValue);

		// prints title with ending line break
		printf("%d\n", pwmValue);
		delay(10);
	}
}




