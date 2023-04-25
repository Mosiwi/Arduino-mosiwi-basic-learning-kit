/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Math function: map()
	2. break statement: break
	3. Judgment statement: switch
	4. binary number, Decimal number, Hexadecimal number
	5. Control the led bar
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <MswLed.h>

byte analogInPin = A3;       // Analog input pin that the Slide potentiometer is attached to
int analogValue  = 0;        // value read from the pot
byte ledNum      = 0;

void setup() {
  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
}

void loop() {
  // Read the analog value in the potentiometer.
  analogValue = analogRead(analogInPin);
  
  // Convert 0-1010 data to 0-8 data.
  ledNum = map(analogValue, 0, 1010, 0, 8);
  
  // Binary number    :   Decimal number   :   Hexadecimal number
  //  0b00000000      =          0         =         0x00
  //  0b00000001      =          1         =         0x01
  //  0b00000011      =          3         =         0x03
  //  0b00000111      =          7         =         0x07
  //  0b00001111      =          15        =         0x0f
  //  0b00011111      =          31        =         0x1f
  //  0b00111111      =          63        =         0x3f
  //  0b01111111      =          127       =         0x7f
  //  0b11111111      =          255       =         0xff
  switch (ledNum){                           
	  case 0: SetLedBar(0b00000000); break;   // Turn off all led      
	  case 1: SetLedBar(0b00000001); break;   // Turn on 1 led         
	  case 2: SetLedBar(0b00000011); break;   // Turn on 2 led        
	  case 3: SetLedBar(0b00000111); break;   // Turn on 3 led          
	  case 4: SetLedBar(0b00001111); break;   // Turn on 4 led         
	  case 5: SetLedBar(0b00011111); break;   // Turn on 5 led          
	  case 6: SetLedBar(0b00111111); break;   // Turn on 6 led         
	  case 7: SetLedBar(0b01111111); break;   // Turn on 7 led        
	  case 8: SetLedBar(0b11111111); break;   // Turn on 8 led         
	  default: break;
  }
  delay(50);
}
