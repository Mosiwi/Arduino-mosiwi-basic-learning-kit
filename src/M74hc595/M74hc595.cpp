/*
 This example works with the Mosiwi Basic learning board.
 
 function: Drive led through 74HC595
 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-17
 */

#include "M74hc595.h"
#include <Arduino.h>

// Used to store control pin data.
byte latchPin = 0;    // Pin connected to ST_CP of 74HC595
byte clockPin = 0;    // Pin connected to SH_CP of 74HC595
byte dataPin  = 0;    // Pin connected to DS of 74HC595

// Used to store LEDs display data.
byte ledData = 0;

void Init_LED_IO(byte latchP, byte clockP, byte dataP){ 
  latchPin = latchP;  // Pin connected to ST_CP of 74HC595 
  clockPin = clockP;  // Pin connected to SH_CP of 74HC595
  dataPin = dataP;    // Pin connected to DS of 74HC595
	
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void SetLed(byte bitLed, byte OnOff){
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
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);	
}

void SetLedBar(byte data){
  ledData = data;
  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, LOW);
  // bitOrder: MSBFIRST or LSBFIRST
  shiftOut(dataPin, clockPin, MSBFIRST, ledData);  
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);	
}


