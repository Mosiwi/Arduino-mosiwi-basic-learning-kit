/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
  1. The relationship between frequency and tone.

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-8
*/
 //Include library file
#include <wiringPi.h>
#include <softTone.h>
#include "../../src/STC/stc.h"
#include "../../src/BC7278/BC7278.h"
#include "../../src/M74x595/m74x595.h"

// Define the buzzer using the 1 pin of the wiringpi.
char BuzzerPin   = 1;

int main(void){
	wiringPiSetup ();

	if(softToneCreate (BuzzerPin) != 0){
		printf("Soft tone create fail!");
	}

	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	// Initialize SPI communications.
	bc7278_init();
	// Clear the screen.
	ClearAll();

	while(1){
		// Read the analog value in the slide resistor.
		int analogValue = stc_sr();

		// Map analog values to tones.
		char note = analogValue/127;

		switch(note){
			case 1: softToneWrite(BuzzerPin, 523); DisplayInt(523); break;    // DO
			case 2: softToneWrite(BuzzerPin, 587); DisplayInt(587); break;    // RE
			case 3: softToneWrite(BuzzerPin, 659); DisplayInt(659); break;    // MI
			case 4: softToneWrite(BuzzerPin, 698); DisplayInt(698); break;    // FA
			case 5: softToneWrite(BuzzerPin, 784); DisplayInt(784); break;    // SO
			case 6: softToneWrite(BuzzerPin, 880); DisplayInt(880); break;    // LA
			case 7: softToneWrite(BuzzerPin, 988); DisplayInt(988); break;    // SI
			default: softToneWrite(BuzzerPin, 0); break;   // Turn off the buzzer.
		}
		if(note != 0){
			SetLedBar(1 << (note-1));
		}
		else{
			DisplayInt(0);
			SetLedBar(0);
		}
	}
}


