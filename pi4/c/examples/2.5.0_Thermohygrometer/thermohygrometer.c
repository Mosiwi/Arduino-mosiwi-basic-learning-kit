/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
    1. Thermohygrometer
	2. variable: long, unsigned long
	3. Function: millis()
	4. Function with arguments
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-20
 */
#include <wiringPi.h>
#include "../../src/AHT20/AHT20.h"
#include "../../src/BC7278/BC7278.h"

unsigned long oldTime = 0;

void displayTemperature(float T){
	int temp = (int)T*10;
	// display C char
	SetDisplayReg(3, 0xC6);
	// Turn on the decimal point.
	SetDisplaySeg(0x0f, 1);          

	// display temperature
	DisplayChar(0, temp/100);
	DisplayChar(1, temp%100/10);
	DisplayChar(2, temp%10);
}

void displayHumidity(float H){
	int temp = (int)H*10;
	// display H char
	SetDisplayReg(3, 0x89);
	// Turn on the decimal point.
	SetDisplaySeg(0x0f, 1);          

	// display temperature
	DisplayChar(0, temp/100);
	DisplayChar(1, temp%100/10);
	DisplayChar(2, temp%10);
}

int main(void){
	Aht20_init();

	// Initialize SPI communications.
	bc7278_init();

	// Clear the screen.
	ClearAll();

	// Stores temperature and humidity data.
	float HT_data[2];

	while(1){
		// You are advised to read the data every 1S
		// Function prototype: Aht20_read_CTdata(float *ct);
		Aht20_read_CTdata(HT_data);                // Read AHT20 temperature and humidity data directly without CRC check.

		// Function prototype: Aht20_read_CTdata_crc(float *ct);
		// Aht20_read_CTdata_crc(HT_data);         // After CRC verification, read temperature and humidity data of AHT20                

		if((millis() - oldTime) < 5000){
			displayTemperature(HT_data[1]);
		}

		if((millis() - oldTime) >= 5000){
			displayHumidity(HT_data[0]);
		}

		if(abs(millis() - oldTime) >= 10000){
			oldTime = millis();
		}
	}
}
