/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. i2c communication
	2. Variable: array

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-20
 */
#include <wiringPi.h>
#include <stdio.h>
#include "../../src/AHT20/AHT20.h"

int main(void) {
	wiringPiSetup();
	if(Aht20_init() == -1){
		printf("IIC init err!");
		return 0;
	}

	while(1){
		float HT_data[2]={1,2};

		// You are advised to read the data every 1S
		// Function prototype: Aht20_read_CTdata(float *ct);
		Aht20_read_CTdata(HT_data);                // Read AHT20 temperature and humidity data directly without CRC check.

		// Function prototype: Aht20_read_CTdata_crc(float *ct);
		//Aht20_read_CTdata_crc(HT_data);         // After CRC verification, read temperature and humidity data of AHT20 

		delay(1000);
		printf("Humidity = %.3f%%, Temperature = %.3fC \n", HT_data[0], HT_data[1]);
	}
	return 0;
}
