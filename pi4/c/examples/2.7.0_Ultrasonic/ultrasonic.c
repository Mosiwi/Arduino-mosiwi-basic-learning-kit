/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. ultrasonic
	2. pluse

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
 */
#include <stdio.h>
#include <wiringPi.h>

#define Trig 23
#define Echo 24

float ultr(void){
	float distance;
	unsigned int t;
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
	// The speed of ultrasound: 340m/s = 34000cm/s,
	// Because the data measured by ultrasound has two distances back and forth, the data has to be divided by 2.
	// Convert the pulse signal to a distance in centimeters.
	distance = ((t/1000000.0) * 34000.0) / 2;
	return distance;
}

// the setup function runs once when you press reset or power the board
int main(void){
	wiringPiSetup();

	// Example Initialize the I/O port
	pinMode(Trig, OUTPUT);
	pinMode(Echo, INPUT);
	digitalWrite(Trig, LOW);

	float dis = 0;
	while(1){
		dis = ultr();

		// Print distance
		printf("distance: %.2f cm \n", dis);
		delay(500);
	}
}
