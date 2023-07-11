/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. Improve your coding skills

 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-20
 */
#include <stdio.h>
#include <stdbool.h>
#include <softPwm.h>
#include <wiringPi.h>
#include "../../src/STC/stc.h"
#include "../../src/AHT20/AHT20.h"
#include "../../src/BC7278/BC7278.h"
#include "../../src/M74x595/m74x595.h"

#define INA 23
#define INB 24

// Variables that record temperature
float HT_data[2];

// Fan speed
int Speed = 60;
// Fan switch
bool OnOff = false;
// Temperature control flag.
bool TC_flag = false;

// High temperature alarm LED.
const char B_led = 26;

// Store the system running time.
int oldTime = 0;

// High temperature enable threshold
char High_temperature_warning = 35;
// Low temperature disable threshold
char Low_temperature_warning = 16;

// the setup function runs once when you press reset or power the board
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

	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	// Initialize SPI communications.
	bc7278_init();

	// Initialize LED pins as an output.
	pinMode(B_led, OUTPUT);
	digitalWrite(B_led, LOW);

	Aht20_init();

	stc_init();

	while(1){
		switch (stc_ir()){
		  case 0xff18e7: Speed += 20; break;           //▲, speed up
		  case 0xff4ab5: Speed -= 20; break;           //▼, speed cut
		  case 0xff38c7: OnOff = !OnOff; break;        //OK, Turn off or on the fan
		  case 0xff629d: TC_flag = !TC_flag; break;    //2, Turn off or on the temperature control fan
		  default: break;
		}

		if(Speed > 100)
		  Speed = 100;
		if(Speed < 0)
		  Speed = 0;

		// Fan speed indicator.
		char ledNum = Speed/(100/8);
		char displayData = 0;
		while(ledNum > 0){
			// same as: displayData = displayData | (1 << (ledNum-1));
			displayData |= 1 << (ledNum-1);
			ledNum --;
		}
		SetLedBar(displayData);

		//Display frequency is 1
		if((millis()-oldTime) > 1000){
			// Conduct temperature measurement
			Aht20_read_CTdata(HT_data);
			DisplayInt(HT_data[1]);
			oldTime = millis();
		}

		// The temperature control fan is enabled
		if(TC_flag){
			// Enable fan
			if(HT_data[1] > High_temperature_warning){
				OnOff = true;
			// Disable fan
			}else if(HT_data[1] < Low_temperature_warning){
				OnOff = false;
			}
			// Enable temperature control indicator LED
			digitalWrite(B_led, HIGH);
		}
		// The temperature control fan is disabled
		else{
			// Disable temperature control indicator LED
			digitalWrite(B_led, LOW);
		}

		if(OnOff){
			// Setting fan run
			softPwmWrite(INA, Speed);
			softPwmWrite(INB, 0);
		}else{
			// Setting fan stop
			softPwmWrite(INA, 0);
			softPwmWrite(INB, 0);
		}
	}
}

