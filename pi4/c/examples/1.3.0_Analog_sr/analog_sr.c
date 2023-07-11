/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. voltage
	2. Slide resistor
	3. Variable: int, float

 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
 */
#include<stdio.h>
#include<wiringPi.h>
#include"../../src/STC/stc.h"

int analogValue       = 0;   // value read from the pot
float voltageValue    = 0;   // Storage voltage value

int main(void){
	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}
	while(1){
	// Read the analog value in the slide resistor.
	analogValue = stc_sr();
	// Calculated voltage
	voltageValue = (3.3/1024)*analogValue;

	// print the results to the teminal
	printf("Analog value = %d \n", analogValue);
	// print the voltage value to the teminal
	printf("Voltage value = %0.3fV \n", voltageValue);

	// wait 20 milliseconds before the next loop for the analog-to-digital
	// converter to settle after the last reading:
	delay(1000);
	}
}





