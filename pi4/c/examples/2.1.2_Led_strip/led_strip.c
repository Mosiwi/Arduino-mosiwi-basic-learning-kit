/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Math function: map()
	2. break statement: break
	3. Judgment statement: switch
	4. binary number, Decimal number, Hexadecimal number
	5. Control the led bar

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <stdio.h>
#include "../../src/M74x595/m74x595.h"
#include "../../src/STC/stc.h"

int analogValue  = 0;        // value read from the pot
char ledNum  = 0;

int main(void){
	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}

	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	while(1){
		// Read the analog value in the slide resistor.
		analogValue = stc_sr();

		// 0---8
		ledNum = analogValue/126;
		// Binary number    :   Decimal number   :   Hexadecimal number
		//  0b00000000      =          0         =         0x00
		//  0b00000001      =          1         =         0x01
		//  0b00000011      =          3         =         0x03
		//  0b00000111      =          7         =         0x07
		//  0b00001111      =          15        =         0x0f
		//  0b00011111      =          31        =         0x1f
		//  0b00111111      =          63        =         0x3f
		//  0b01111111      =          127       =         0x7f
		//  0b11111111      =          255       =         0xff
		switch (ledNum){
		  case 0: SetLedBar(0b00000000); break;   // Turn off all leds
		  case 1: SetLedBar(0b00000001); break;   // Turn on 1 led
		  case 2: SetLedBar(0b00000011); break;   // Turn on 2 leds
		  case 3: SetLedBar(0b00000111); break;   // Turn on 3 leds
		  case 4: SetLedBar(0b00001111); break;   // Turn on 4 leds
		  case 5: SetLedBar(0b00011111); break;   // Turn on 5 leds
		  case 6: SetLedBar(0b00111111); break;   // Turn on 6 leds
		  case 7: SetLedBar(0b01111111); break;   // Turn on 7 leds
		  case 8: SetLedBar(0b11111111); break;   // Turn on 8 leds
		  default: break;
		}
		delay(50);
	}
}
