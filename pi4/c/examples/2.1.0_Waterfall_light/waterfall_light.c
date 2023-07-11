/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Controls 8 yellow leds.

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <wiringPi.h>
#include <sr595.h>

int main(void) {
	wiringPiSetup ();
	// Pin base 100 for 8 pins.
	// Use wiringPi pins 0, 2 & 3 for data(DS), clock(SH_CP) and latch(ST_CP)
	sr595Setup (100, 8, 0, 2, 3) ;

	int pin;
	while(1){
		for(pin=0; pin<8; pin++){
			digitalWrite (100 + pin, 1) ;
			delay(500);
		}
		for(pin=7; pin>=0; pin--){
			digitalWrite (100 + pin, 0) ;
			delay(500);
		}
	}
}
