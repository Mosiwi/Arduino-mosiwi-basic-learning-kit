/*
 * Project: 74HC595 3wire soft communication protocol
 * Function: Controls 8 leds.
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */
#include <stdio.h>
#include <wiringPi.h>

// Pin connected to ST_CP of 74HC595
#define latchPin  3
// Pin connected to SH_CP of 74HC595
#define clockPin  2
// Pin connected to DS of 74HC595
#define dataPin   0

// Data transmission mode
#define MSBFIRST  1
#define LSBFIRST  0

// LED state
#define ON   1
#define OFF  0

// Used to store LEDs display data.
unsigned char ledData = 0;



//////////////////////////////////////////////////////////////
void Init_LED_IO(unsigned char latchP, unsigned char clockP, unsigned char dataP){ 
  //set pins to output because they are addressed in the main loop
  pinMode(latchP, OUTPUT);
  pinMode(clockP, OUTPUT);
  pinMode(dataP, OUTPUT);
}

//////////////////////////////////////////////////////////////
// Send 8-bit data to 74HC595.
// bitOrder: MSBFIRST or LSBFIRST
void shiftOut(unsigned char dataPin_, unsigned char clockPin_, unsigned char bitOrder, unsigned char val){
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

//////////////////////////////////////////////////////////////
// Turn on or off LEDs
// bit: 0-8, OnOff: ON or OFF
void SetLed(unsigned char bit, unsigned char OnOff){
	if(bit > 8 || OnOff > 1){
		return;
	}
	if(OnOff == 1){
		ledData = ledData | (1 << bit);
	}
	else{
		ledData = ledData & (~(1 << bit));
	}
	//ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, ledData);
	//no longer needs to listen for information
    digitalWrite(latchPin, HIGH);
}

void setLed(unsigned char bitLed, unsigned char OnOff){
  if(OnOff == 1){
    ledData = ledData | (0x01 << bitLed);
  }
  else{
    ledData = ledData & (~(0x01 << bitLed));
  }

  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledData);
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);
}

//////////////////////////////////////////////////////////////
int main(void){
	wiringPiSetup();

	//set pins to output so you can control the shift register;
	Init_LED_IO(latchPin, clockPin, dataPin);

	int a;
	while(1){
		for(a=0; a<=7; a++){
			SetLed(a, ON);
			delay(1000);
		}
		for(a=7; a>=0; a--){
			setLed(a, OFF);
			delay(1000);
		}
	}
}



