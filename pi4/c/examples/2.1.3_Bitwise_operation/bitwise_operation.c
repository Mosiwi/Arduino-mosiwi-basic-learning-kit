/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Bitwise operators: &, |, ~, ^, <<, >>

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-2
*/
#include "../../src/M74x595/m74x595.h"

char button  = 22;

int main(void){
	// ST_CP = 3, SH_CP = 2, DS = 0
	Init_LED_IO(3, 2, 0);

	wiringPiSetup();
	pinMode(button, INPUT);

	int led = 0;
	while(1){
		led = 0b10101010;
		// Led initial state
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : ON   OFF  ON   OFF  ON   OFF  ON   OFF
		SetLedBar(led);
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);

		// Binary AND Operator
		//       0b10101010
		//     & 0b11110000
		// led = 0b10100000
		led = 0b10101010 & 0b11110000;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : ON   OFF  ON   OFF  OFF  OFF  OFF  OFF
		SetLedBar(led);
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);

		// Binary OR Operator
		//       0b10100000
		//     | 0b11110000
		// led = 0b11110000
		led = 0b10100000 | 0b11110000;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : ON   ON   ON   ON   OFF  OFF  OFF  OFF
		SetLedBar(led);   
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);

		// Binary take the inverse operation
		//     ~ 0b11110000
		// led = 0b00001111
		led = ~0b11110000;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : OFF  OFF  OFF  OFF  ON   ON   ON   ON
		SetLedBar(led);   
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);

		// Binary Xor Operator
		//       0b00001111
		//     ^ 0b00011110
		// led = 0b00010001
		led = 0b00001111 ^ 0b00011110;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : OFF  OFF  OFF  ON   OFF  OFF  OFF  ON 
		SetLedBar(led);   
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);  

		// Binary left shift operator
		//       0b00010001 << 7
		// led = 0b10000000
		led = led << 7;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : ON   OFF  OFF  OFF  OFF  OFF  OFF  OFF  
		SetLedBar(led);   
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);

		// Binary right shift operator
		//       0b10000000 >> 7
		// led = 0b00000001
		led = led >> 7;
		// LED   : Q7   Q6   Q5   Q4   Q3   Q2   Q1   Q0
		// State : OFF  OFF  OFF  OFF  OFF  OFF  OFF  ON  
		SetLedBar(led);   
		while(digitalRead(button)==HIGH);
		while(digitalRead(button)==LOW);
	}
}











