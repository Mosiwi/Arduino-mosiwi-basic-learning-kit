/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  

 Learning target:
  1. Infrared Remote

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-20
*/
#include <stdio.h>
#include <wiringPi.h>
#include "../../src/STC/stc.h"

// For the led
char led_pin = 23;
char ok = 0;
unsigned int key = 0;

int main(void){
	wiringPiSetup();
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, LOW);

	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}

	while(1){
		key = stc_ir();
		if(key != 0){
			printf("Get the key: 0x%04x \n", key);

			if(key == 0xff38){   // OK button
				ok = 1 - ok;
				digitalWrite(led_pin, ok);

				if(ok == 1)
					printf("Red LED ON!\n");
				else
					printf("Red LED OFF!\n");
			}
		}
		delay(100);
	}
}
