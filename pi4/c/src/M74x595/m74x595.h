/*
 This example works with the Mosiwi Basic learning board.
 
 function: Drive led through 74HC595
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-5
 */
#ifndef M74hc595_h
#define M74hc595_h
#include<wiringPi.h>

// Data transmission mode
#define MSBFIRST  1
#define LSBFIRST  0

// Used to store control pin data.
char latchPin = 3;    // Pin connected to ST_CP of 74HC595
char clockPin = 2;    // Pin connected to SH_CP of 74HC595
char dataPin  = 0;    // Pin connected to DS of 74HC595

// Used to store LEDs display data.
unsigned char ledData = 0;

// Initialize the control pin
void Init_LED_IO(char latchP, char clockP, char dataP){
	wiringPiSetup();
	latchPin = latchP;  // Pin connected to ST_CP of 74HC595 
	clockPin = clockP;  // Pin connected to SH_CP of 74HC595
	dataPin = dataP;    // Pin connected to DS of 74HC595
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
}

//////////////////////////////////////////////////////////////
// Send 8-bit data to 74HC595.
// bitOrder: MSBFIRST or LSBFIRST
void shiftOut(char dataPin_, char clockPin_, char bitOrder, unsigned char val){
	unsigned char i;
	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin_, val & 1);
			val >>= 1;
		} else {
			digitalWrite(dataPin_, (val & 128) != 0);
			val <<= 1;
		}

		digitalWrite(clockPin_, HIGH);
		delayMicroseconds(10);
		digitalWrite(clockPin_, LOW);
		delayMicroseconds(10);
	}
}

// Control leds on and off.
// bitLed = 0-7， OnOff = 0 or 1 (0=off, 1=on)
void SetLed(unsigned char bitLed, char OnOff){
	if(OnOff == 1){
		ledData = ledData | (0x01 << bitLed);
	}
	else{
		ledData = ledData & (~(0x01 << bitLed));
	}

	//ground latchPin and hold low for as long as you are transmitting
	digitalWrite(latchPin, LOW);
	// bitOrder: MSBFIRST or LSBFIRST
	shiftOut(dataPin, clockPin, MSBFIRST, ledData);  
	digitalWrite(latchPin, HIGH);	
}

// Control leds on and off.
// data = led7 -- led0 = xxxxxxxxB
void SetLedBar(unsigned char data){
	ledData = data;
	//ground latchPin and hold low for as long as you are transmitting
	digitalWrite(latchPin, LOW);
	// bitOrder: MSBFIRST or LSBFIRST
	shiftOut(dataPin, clockPin, MSBFIRST, ledData);  
	digitalWrite(latchPin, HIGH);	
}

#endif
