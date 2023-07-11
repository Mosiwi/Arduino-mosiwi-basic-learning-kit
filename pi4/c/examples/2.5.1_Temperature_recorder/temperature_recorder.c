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
 Date：2023-5-10
 */
#include <wiringPi.h>
#include "../../src/AHT20/AHT20.h"
#include "../../src/BC7278/BC7278.h"
#include "../../src/GX2431/GX2431.h"

// _recording_period  = second
// 15  minute  = 90
// 1   minute  = 60
// 30 seconds  = 30
// 10 seconds  = 10
#define _recording_period  30  // 30 seconds

int _second_counter = 0;
int _recording_period_counter = 0;

// An array of temporarily recorded temperatures.
unsigned char Data[128] = {0};

// One Wire pin, change according to your needs. 
const int ONE_WIRE_PIN = 27; 

unsigned long oldTime = 0;
// Key value signal output flag pin.
const char keySignalPin = 21;
unsigned char KeyValue = 0;

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

// Interrupt function with no return value
void GetKeyValue(void){
  //            bit: 0 0 0  x x x x x
  // Read key value: 0 0 0 OK U D L R
  KeyValue = ReadKeyValue();
  
  if(KeyValue == 1){ // OK button
	unsigned char data[128];
    eeprom_m_read(0, data, sizeof(data));
    printf("Temperature recorded by eeprom : \n");
    for(int i=0; i<128; i++){
      printf(" %d", data[i]);
      if((i%8==7) && (i!=0))
        printf("\n");			
    } 
    printf("\n");
  }
}

int main(void) {
	wiringPiSetup();
	// Use the external interrupt function to get the key value.
	wiringPiISR(keySignalPin, INT_EDGE_FALLING, GetKeyValue);
	
	Aht20_init();

	// Initialize SPI communications.
	bc7278_init();
	// Clear the screen.
	ClearAll();
  
	// Find if there is a device on the 1-wire bus.
	if(!eeprom_init(ONE_WIRE_PIN)){
		// EEPEOM initialization error, 4 digit digital tube display "EEEE".
		DisplayChar(0, 14);
		DisplayChar(1, 14);
		DisplayChar(2, 14);
		DisplayChar(3, 14);
		return 0;
	}


	// Stores temperature and humidity data.
	float HT_data[2];
	while(1){
		// You are advised to read the data every 1S
		// Function prototype: Aht20_read_CTdata(float *ct);
		Aht20_read_CTdata(HT_data);                // Read AHT20 temperature and humidity data directly without CRC check.              

		// Execute the program inside the if statement every 1 second.
		if((millis() - oldTime) >= 1000){
			oldTime = millis();
			_second_counter ++;

			// 4 digit digital display temperature value.
			displayTemperature(HT_data[1]);

			// This statement is executed when the record period is reached.
			if(_second_counter >= _recording_period){
				_second_counter = 0;

				Data[_recording_period_counter] = HT_data[1];
				if(!eeprom_m_write((_recording_period_counter/8)*8, &Data[(_recording_period_counter/8)*8], 8))
					printf("Write data to eeprom, error!");
				_recording_period_counter ++;

				// 0-127: A maximum of 128 data can be recorded.
				// When the number of records reaches 127, all recorded data will be cleared.
				if(_recording_period_counter == 128){
					_recording_period_counter = 0;
					for(int i=0; i<128; i++)
						Data[i] = 0;
				}
			}
		}
	}
}

















