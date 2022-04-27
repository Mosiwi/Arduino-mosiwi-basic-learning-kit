/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Bitwise operators: &, |, ~, ^, <<, >>
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-2
*/
#include <MswLed.h>

byte led = 0b10101010;

void setup() {
  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
  
  // Led initial state  
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : ON   OFF  ON   OFF  ON   OFF  ON   OFF
  SetLedBar(led);
  delay(1000);
}

void loop() { 
  // Binary AND Operator
  //       0b10101010
  //     & 0b11110000
  // led = 0b10100000
  led = 0b10101010 & 0b11110000;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : ON   OFF  ON   OFF  OFF  OFF  OFF  OFF
  SetLedBar(led);
  delay(1000);
  
  // Binary OR Operator
  //       0b10100000
  //     | 0b11110000
  // led = 0b11110000
  led = 0b10100000 | 0b11110000;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : ON   ON   ON   ON   OFF  OFF  OFF  OFF
  SetLedBar(led);   
  delay(1000);
  
  // Binary take the inverse operation
  //     ~ 0b11110000
  // led = 0b00001111
  led = ~0b11110000;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : OFF  OFF  OFF  OFF  ON   ON   ON   ON
  SetLedBar(led);   
  delay(1000);  
  
  // Binary Xor Operator
  //       0b00001111
  //     | 0b00011110
  // led = 0b00010001
  led = 0b00001111 ^ 0b00011110;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : OFF  OFF  OFF  ON   OFF  OFF  OFF  ON 
  SetLedBar(led);   
  delay(1000);  
  
  // Binary left shift operator
  //       0b00000001 << 7
  // led = 0b11111111
  led = led ^ 0b10000000;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : ON   OFF  OFF  OFF  OFF  OFF  OFF  OFF  
  SetLedBar(led);   
  delay(1000); 
  
  // Binary right shift operator
  //       0b10000000 >> 7
  // led = 0b00000001
  led = led ^ 0b10000000;
  // LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
  // State : OFF  OFF  OFF  OFF  OFF  OFF  OFF  ON  
  SetLedBar(led);   
  delay(1000); 
}











