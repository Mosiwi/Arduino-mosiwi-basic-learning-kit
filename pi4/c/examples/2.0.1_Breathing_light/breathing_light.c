/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. random(min,max);
	2. ternary conditional operator: Expression 1? Expression 2: Expression 3;

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-22
*/
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char R_led = 23;
const char G_led = 24;
const char B_led = 26;
char random_num = 0;
char RGB_pin = 0;

void fadeOn(unsigned int time,int increament){
	for (char value = 0 ; value < 255; value+=increament){
		softPwmWrite(RGB_pin, value);
		delay(time/(255/increament));
	}
}

void fadeOff(unsigned int time,int decreament){
	for (char value = 255; value >0; value-=decreament){
		softPwmWrite(RGB_pin, value);
		delay(time/(255/decreament));
	}
}

int main(void){
	wiringPiSetup();
	/*
	 Function prototype: int softPwmCreate (int pin, int initialValue, int pwmRange);
	 The minimum pulse width is 100μS, Pulse width = initialValue * 100uS
	 PWM frequency = 1s / (100us * pwmRange)
	*/
	int pwm1 = softPwmCreate(R_led, 0, 100);  // PWM frequency = 100Hz
	int pwm2 = softPwmCreate(G_led, 0, 100);  // PWM frequency = 100Hz
	int pwm3 = softPwmCreate(B_led, 0, 100);  // PWM frequency = 100Hz
	if(pwm1 != 0 || pwm2 != 0 || pwm3 != 0){
		printf("PWM function initialization failed!");
		return 0;
	}

	int random_num = 0;
	while(1){
		// Randomly obtain values between 1-3.
		srand((unsigned int)time(NULL));
		random_num = rand()%3 + 1;

		RGB_pin = random_num == 1 ? R_led : (random_num == 2 ? G_led : B_led);
		fadeOn(1000,5);
		fadeOff(1000,5);
	}
}




