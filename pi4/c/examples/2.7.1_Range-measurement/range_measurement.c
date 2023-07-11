/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. A function with a return value
	2. Logical operation: ||

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-6
 */
#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include "../../src/BC7278/BC7278.h"

#define Trig 23
#define Echo 24

// Key value signal output flag pin.
char keySignalPin = 21;
unsigned char keyValue = 0;
bool OnOff = false;

// Ultrasonic measuring distance
float Measuring_distance(void){
	unsigned int t;
	float d;
	int cnt = 0;
	// The Trig pin sends a 10uS high level trigger pulse signal.
	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(Trig, LOW);

	while(!digitalRead(Echo)){
		delayMicroseconds(10);
		cnt ++;
		if(cnt > 5100){
			cnt = 0;
			break;
		}
	}
	t = micros();
	while(digitalRead(Echo)){
		delayMicroseconds(10);
		cnt ++;
		if(cnt > 5100){
			cnt = 0;
			break;
		}
	}
	t = micros() - t;

	// Convert microseconds to seconds: rxTime/1000000.0  --->  1s = 1000000us
	// The speed of ultrasound: 340m/s = 34000cm/s. 
	// Because the data measured by ultrasound has two distances back and forth, the data has to be divided by 2.
	// Convert the pulse signal to a distance in centimeters.
	d = ((t/1000000.0) * 34000.0) / 2;
	return d;
}

// Read the button value.
void GetKeyValue(void){
  keyValue = ReadKeyValue();
  if(keyValue == 8 || keyValue == 4 || keyValue == 2 || keyValue == 1){
    OnOff = !OnOff;
  }
}


int main(void){
	wiringPiSetup();

	// Initialize the ultrasonic control pins
	pinMode(Trig, OUTPUT);
	pinMode(Echo, INPUT);
	digitalWrite(Trig, LOW); 

	// Use the external interrupt function to get the key value.
	wiringPiISR(keySignalPin, INT_EDGE_FALLING, GetKeyValue);

	// Initialize SPI communications.
	bc7278_init();

	// Clear the screen.
	ClearAll();

	while(1){
		if(OnOff){
			float distance;
			// Ultrasonic measuring distance
			distance = Measuring_distance();

			DisplayInt(distance);
			// It is recommended that the refresh interval be at least 100ms
			delay(100);
		}else{
			// Clear display
			ClearAll();
			delay(100);
		}
	}
}
