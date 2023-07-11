/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. LED
	2. What is a programming statement?
	3. Loop statement: while
	4. Digital pin output Settings
	5. Use of the delay() function
	6. Annotation symbols: //, / *...* /

 Compile command: gcc -Wall -o blink blink.c -lwiringPi
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-16
*/
#include<wiringPi.h>

int main(void){
	wiringPiSetup();
	pinMode(23, OUTPUT);

	while(1){
		digitalWrite(23, HIGH);    // turn the LED on (HIGH is the voltage level)
		delay(1000);               // wait for a second (1000ms = 1s)
		digitalWrite(23, LOW);     // turn the LED off by making the voltage LOW
		delay(1000);               // wait for a second (1000ms = 1s) 
	}
}

