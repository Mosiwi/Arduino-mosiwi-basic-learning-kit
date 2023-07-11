/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  

 Learning target:
  1. infrared receiver

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-10
*/
#include<stdio.h>
#include<wiringPi.h>
#include"../../src/STC/stc.h"

int main(void){
	//Key value of remote control
	//0xff18: ▲
	//0xff4a: ▼
	//0xff10: ◄
	//0xff5a: ►
	//0xff38: OK
	//0xff98: 0
	//0xffa2: 1
	//0xff62: 2
	//0xffe2: 3
	//0xff22: 4
	//0xff02: 5
	//0xffc2: 6
	//0xffe0: 7
	//0xffa8: 8
	//0xff90: 9
	//0xff68: *
	//0xffb0: #

	if(stc_init() == -1){
		printf("stc init err!\n");
		return 0;
	}

	unsigned int ir_data = 0;
	while(1){
		ir_data = stc_ir();
		if(ir_data != 0){
			printf("Get the key: 0x%04x \n", ir_data);
		}
		delay(500);
	}
}
