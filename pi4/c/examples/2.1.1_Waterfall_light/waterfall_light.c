/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Controls 8 yellow leds.

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-19
*/
#include <wiringPi.h>
#include "../../src/M74x595/m74x595.h"

int main(void) {
	int i;
	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	while(1){
		for(i=0; i<=7; i++){
			SetLed(i, 1);
			delay(500);
		}
		for(i=7; i>=0; i--){
			SetLed(i, 0);
			delay(500);
		}
	}
}
