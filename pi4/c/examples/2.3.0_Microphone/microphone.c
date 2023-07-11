/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. microphone
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-8
 */
#include <stdio.h>
#include <wiringPi.h>
#include "../../src/STC/stc.h"    

int voiceAnalogValue     = 0;   // Used to store analog value of microphone
float voiceVoltageValue  = 0;   // Used to store voltage value of microphone

int main(void){
	stc_init();
	
	while(1){
		// Read the voice value of the microphone.
		voiceAnalogValue = stc_mic();
		// Calculated voltage
		voiceVoltageValue = 3.3/1024*voiceAnalogValue;

		// print the results to the Serial Monitor:
		printf("Voice-analog = %d \n", voiceAnalogValue); 
		// print the voltage value to the Serial Monitor:
		printf("Voice-voltage = %f \n", voiceVoltageValue);

		delay(200);
	}
}





