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
 Date：2022-3-20
 */
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

// These constants won't change. They're used to give names to the pins used:
const char pwmOutPin   = 23;         // Define the PWM pin
int pwmValue    = 0;                 // value output to the PWM (analog out)


int main(void){
	wiringPiSetup();
	/*
	 Function prototype: int softPwmCreate (int pin, int initialValue, int pwmRange);
	 The minimum pulse width is 100μS, Pulse width = initialValue * 100uS
	 PWM frequency = 1s / (100us * pwmRange)
	*/
	int pwm = softPwmCreate(pwmOutPin, 0, 100);  // PWM frequency = 100Hz
	if(pwm != 0){
		printf("PWM function initialization failed! \n");
		return 0;
	}

	while(1){
		// Judgment loop statement
		printf("Test <= 255 : --------------------------\n");
		delay(1000);
		for(pwmValue = 0; pwmValue <= 255; pwmValue ++){
			// change the analog out value:
			softPwmWrite(pwmOutPin, pwmValue);

			// prints title with ending line break
			printf("%d\n", pwmValue);
			delay(20);
		}

		// Judgment loop statement
		printf("Test >=0 : --------------------------\n");
		delay(1000);
		for(pwmValue = 255; pwmValue >= 0; pwmValue --){
			// change the analog out value:
			softPwmWrite(pwmOutPin, pwmValue);

			// prints title with ending line break
			printf("%d\n", pwmValue);
			delay(20);
		}

		// Judgment loop statement
		printf("Test < 256 : ---------------------------\n");
		delay(1000);
		for(pwmValue = 0; pwmValue < 256; pwmValue ++){
			// change the analog out value:
			softPwmWrite(pwmOutPin, pwmValue);

			// prints title with ending line break
			printf("%d\n", pwmValue);
			delay(20);
		}

		// Judgment loop statement
		printf("Test > -1 : --------------------------\n");
		delay(1000);
		for(pwmValue = 255; pwmValue > -1; pwmValue --){
			// change the analog out value:
			softPwmWrite(pwmOutPin, pwmValue);

			// prints title with ending line break
			printf("%d\n", pwmValue);
			delay(20);
		}
	}
}




