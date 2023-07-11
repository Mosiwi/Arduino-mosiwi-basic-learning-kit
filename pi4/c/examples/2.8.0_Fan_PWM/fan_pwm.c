/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. The speed of the fan is controlled by PWM

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
 */
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define INA 23
#define INB 24

int main(void){
	wiringPiSetup();
	/*
	 Function prototype: int softPwmCreate (int pin, int initialValue, int pwmRange);
	 The minimum pulse width is 100μS, Pulse width = initialValue * 100uS
	 PWM frequency = 1s / (100us * pwmRange)
	*/
	int pwm1 = softPwmCreate(INA, 0, 100);  // PWM frequency = 100Hz
	int pwm2 = softPwmCreate(INB, 0, 100);  // PWM frequency = 100Hz
	if(pwm1 != 0 || pwm2 != 0){
		printf("PWM function initialization failed! \n");
		return 0;
	}

	int i;

	// clockwise rotation
	for(i = 0; i<160; i++){
		// Speed allowed range: i = 0-255
		softPwmWrite(INA, i);
		softPwmWrite(INB, 0);     
		delay(10);	
	}
	for(i = 160; i>0; i--){
		softPwmWrite(INA, i);
		softPwmWrite(INB, 0);     
		delay(10);	
	}

	// Counterclockwise rotation
	for(i = 0; i<160; i++){
		softPwmWrite(INA, 0);
		softPwmWrite(INB, i);     
		delay(10);	
	}
	for(i = 160; i>0; i--){
		softPwmWrite(INA, 0);
		softPwmWrite(INB, i);     
		delay(10);	
	}
}

