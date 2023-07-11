/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
  1. NEC Infrared protocol
  2. Timer interruption
  3. #define common methods
  4. struct variable

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-15
*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define IR_PIN 6
#define IO digitalRead(IR_PIN)
unsigned char i,idx,cnt;
unsigned char count;
unsigned char data[4];

int main(void){
	wiringPiSetup();
	pinMode(IR_PIN, INPUT);
	pullUpDnControl(IR_PIN, PUD_OFF);
	printf("IRM test program ... \n");
	while(1){
		if(IO == 0){
			count = 0;
			while(IO == 0 && count++ < 200)  //9ms
				delayMicroseconds(60);

			count = 0;
			while(IO == 1 && count++ < 100)  //4.5ms
				delayMicroseconds(60);

			idx = 0;
			cnt = 0;
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			for(i = 0; i < 32; i++){
				count = 0;
				while(IO == 0 && count++ < 15) //0.56ms
					delayMicroseconds(60);

				count = 0;
				while(IO == 1 && count++ < 35) //0: 0.56ms, 1: 1.69ms
					delayMicroseconds(60);

				if(count > 15)
					data[idx] |= (1 << cnt);

				if(cnt == 7){
					cnt = 0;
					idx++;
				}else{
					cnt++;
				}
			}
			//data[0] = address
			//data[1] = address(logical inverse)
			//data[2] = command
			//data[3] = command(logical inverse)
			if((data[0]+data[1] == 0xff) && (data[2]+data[3] == 0xff)){ //check
				printf("Get the key: 0x%02x\n", data[2]);
			}
		}
	}
}
