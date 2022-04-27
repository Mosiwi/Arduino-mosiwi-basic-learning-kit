/*
 This example works with the Mosiwi Basic learning board.
 
 function: Drive led through 74HC595
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-17
 */
#ifndef M74hc595_h
#define M74hc595_h
#include <Arduino.h>

// The state of the LEDs
const byte On  = 1;
const byte Off = 0;

// Initialize the control pin
void Init_LED_IO(byte latchP, byte clockP, byte dataP);

// Control leds on and off.
// bitLed = 0-7， OnOff = 0 or 1 (0=off, 1=on)
void SetLed(byte bitLed, byte OnOff);

// Control leds on and off.
// data = led7 -- led0 = xxxxxxxxB
void SetLedBar(byte data);

#endif
