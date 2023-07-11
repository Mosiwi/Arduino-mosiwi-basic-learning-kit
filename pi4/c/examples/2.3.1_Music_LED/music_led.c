/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Logical operation: &&
	2. Jump statements: goto

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-8
 */
#include <stdio.h>
#include <wiringPi.h>
#include "../../src/M74x595/m74x595.h"
#include "../../src/STC/stc.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

// Used to store analog value of microphone
int voiceAnalogValue = 0;

// Regulatory sensitivity
#define sensitivity 5
int up_0 = 0 + sensitivity*50;
int dow_1023 = 1023 - sensitivity*50;

// Adjust the first LED light flashing.
#define adj_flash 1
int up_512  = 512 + adj_flash*5;
int dow_512 = 512 - adj_flash*5;

// vl must be between in1 and in2
// in2 must be greater than in1
// out2 must be greater than out2
int map(int vl, int in1, int in2, int out1, int out2){
	int i = 0;
	int lev = (in2-in1)/(out2-out1);
	for(i = 0; i < (out2 - out1); i++){
		if((in1+(i*lev) <= vl) && (vl < in1+((i+1)*lev))){
			return i;
		}
	}
	return 0;
}

int main(void){
	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}

	// Limit the maximum value of the up_0 variable to 461.
	// Limit the minimum value of the dow_1023 variable to 563.
	up_0  = min(up_0, 461);         // Get the minimum. 461 = 462 - 1
	dow_1023 = max(dow_1023, 563);  // Get the maximum. 563 = 562 + 1

	// Limit the maximum value of the up_512 variable to 562.
	// Limit the minimum value of the dow_512 variable to 462.
	up_512  = min(up_512, 562);     // Get the minimum. 562 = 512 + 50
	dow_512 = max(dow_512, 462);    // Get the maximum. 462 = 512 - 50

	// Initialize the control pin of led
	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	while(1){
		// readAnalog label
		readAnalog:
		// Read the voice value of the microphone.
		voiceAnalogValue  = stc_mic();
		//printf("Voice:%d \n", voiceAnalogValue);

		char ledNum = 0;
		if((up_512 <= voiceAnalogValue) && (voiceAnalogValue <= dow_1023)){
			ledNum = map(550, up_512, dow_1023, 0, 7);
		}
		else if((up_0 <= voiceAnalogValue) && (voiceAnalogValue <= dow_512)){
			ledNum = map(550, up_0, dow_1023, 0, 7);
		}
		else{
			SetLedBar(0b00000000);
			// Jump to readAnalog label
			goto readAnalog;
		}

		unsigned char leds = 0;
		int i = 0;
		for(i = 0; i <= ledNum; i++){
			leds = leds | (unsigned char)(1 << i);
			SetLedBar(leds);
			delay(10*i);
		}
		for(i = ledNum; i >= 0; i--){
			leds = leds & (unsigned char)(~(1 << i));
			SetLedBar(leds);
			delay(10*i);
		}
	}
}





