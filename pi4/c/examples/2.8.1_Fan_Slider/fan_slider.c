/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Control the fan

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-21
*/
#include <stdio.h>
#include <softPwm.h>
#include <wiringPi.h>
#include"../../src/STC/stc.h"
#include "../../src/M74x595/m74x595.h"

#define INA 23
#define INB 24

int main(void){
	wiringPiSetup();

	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}

	/*
	 softPwmCreate(int pin, int initialValue, int pwmRange);
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

	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	while(1){
		// Read the analog value in the potentiometer.
		int analogValue = stc_sr();

		// Convert 0-1020 data to 0-255 data.
		unsigned char fanSpeed = analogValue/(1020/100);
		softPwmWrite(INB, fanSpeed);
		softPwmWrite(INA, 0);

		// Fan speed indicator.
		unsigned char ledNum = analogValue/(1020/8);
		unsigned char displayData = 0;
		while(ledNum > 0){
			// same as: displayData = displayData | (1 << (ledNum-1));
			displayData |= 1 << (ledNum-1);
			ledNum --;
		}
		SetLedBar(displayData);
	}
}





