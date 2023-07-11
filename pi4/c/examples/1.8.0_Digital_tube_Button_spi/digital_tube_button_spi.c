/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. spi communication
	2. digital tube display, button

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-6
 */
 //Include library file
#include <wiringPi.h>
#include "../../src/BC7278/BC7278.h"

//               A                       A                       A                       A
//           ----------              ----------              ----------              ----------
//          |          |            |          |            |          |            |          |
//        F |          | B        F |          | B        F |          | B        F |          | B
//          |    G     |            |    G     |            |    G     |            |    G     |
//           ----------              ----------              ----------              ----------   
//          |          |            |          |            |          |            |          |
//        E |          | C        E |          | C        E |          | C        E |          | C
//          |    D     |            |    D     |            |    D     |            |    D     |
//           ----------  o DP        ----------  o DP        ----------  o DP        ----------  o DP
//
//              DG0                     DG1                     DG2                     DG3                             

// Key value signal output flag pin.
const char keySignalPin = 21;
unsigned char KeyValue = 0;

// Interrupt function with no return value
void GetKeyValue(void){
  //            bit: 0 0 0 x x x x x
  // Read key value: 0 0 0 U D L R OK
  KeyValue = ReadKeyValue();
}

int main(void) {
	wiringPiSetup();

	// Use the external interrupt function to get the key value.
	wiringPiISR(keySignalPin, INT_EDGE_FALLING, GetKeyValue);

	// Initialize SPI communications.
	bc7278_init();

	// display segment
	// Seg   DP    G    F    E    D    C    B    A
	// DG
	//  0    7h    6h   5h   4h   3h   2h   1h   0h
	//  1    fh    eh   dh   ch   bh   ah   9h   8h
	//  2    17h   16h  15h  14h  13h  12h  11h  10h
	//  3    1fh   1eh  1dh  1ch  1bh  1ah  19h  18h
	//
	// OnOff = 1 = on, OnOff = 0 = off
	// SetDisplaySeg(byte Seg, byte OnOff);

	// display segment
	// DG: 0--3
	// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=off, x=0=on)
	// SetDisplayReg(byte DG, byte Seg);

	// set segment flash
	// DG: 0--3
	// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
	// SetSegFlash(byte DG, byte Seg);

	// set DGx flash
	// GD = B7  B6  B5  B4  B3  B2  B1  B0 (binary)
	//      --  --  --  --  DG3 DG2 DG1 DG0 (1=flash, 0=not flash)
	// SetDgFlash(byte DG);

	// set flash speed： 0--255
	// SetFlashSpeed(byte Speed);

	// Clear the screen.
	ClearAll();

	// Clear one bit display.
	// DG = 0--3
	// ClearBit(byte DG);

	// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
	// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	// DG     : 0, 1, 2, 3
	// DisplayChar(byte DG, byte Dat);


	while(1) {
		// Force a byte variable to be converted to a float variable.
		float Num = (float)KeyValue;
		// prints data with ending line break

		if(Num != 0.0){
			// int parameter: num = 0-9999
			DisplayInt(Num);

			// float parameter: num = 0.0-999.9
			//DisplayFloat(Num);
			// It is recommended that the refresh interval be at least 100ms
			delay(100);

			//            bit: 0 0 0 x x x x x
			// Read key value: 0 0 0 U D L R OK
			printf("Key value = %d \n", KeyValue);
			KeyValue = 0.0;
		}
	}
}










