/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
  1. Buzzer
  2. function: tone(), notone()
     http://wiringpi.com/reference/software-tone-library

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
*/
#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

// Define the buzzer using the 1 pin of the wiringpi.
char BuzzerPin   = 1;

int main(void){
	wiringPiSetup();

	if(softToneCreate (BuzzerPin) != 0){
		printf("Soft tone create fail!");
	}

	while(1){
		// play a note on BuzzerPin for 1000 ms:
		softToneWrite(BuzzerPin, 440);
		delay(1000);
		softToneWrite(BuzzerPin, 0);
		delay(1000);
	}
}
