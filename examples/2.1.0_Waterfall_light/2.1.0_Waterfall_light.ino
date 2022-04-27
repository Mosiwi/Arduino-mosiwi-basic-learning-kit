/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Controls 8 yellow leds.
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
*/

#include <MswLed.h>

void setup() {
  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
}

void loop() {
  int i;
  for(i=0; i<=7; i++){
  SetLed(i, On);
  delay(500);
  }
  for(i=7; i>=0; i--){
  SetLed(i, Off);
  delay(500);
  }
}
