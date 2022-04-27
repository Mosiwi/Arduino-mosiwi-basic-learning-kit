/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. spi communication
	2. digital tube display, button
	3. Forced variable conversion
	4. system clock
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-20
 */
 //Include library file
#include <MswDT_Button.h>

// Key value signal output flag pin.
const byte keySignalPin = 3;         
byte KeyValue = 0;

// Interrupt function with no return value
void GetKeyValue(void){
  //            bit: 0 0 0 0 x x x x
  // Read key value: 0 0 0 0 U L R D
  KeyValue = DTB.ReadKeyValue();
}

void setup() {
  // Initialize serial communications at 9600 bps.
  Serial.begin(9600);
  
  // Initialize the 8-segment digital display tube and keys through SPI communication. 
  pinMode(keySignalPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(keySignalPin), GetKeyValue, CHANGE);
  
  // Initialize SPI communications.
  DTB.Start_Init();
  
  // display segment
  // Seg   DP    G    F    E    D    C    B    A
  // Bit 
  //  0    7h    6h   5h   4h   3h   2h   1h   0h
  //  1    fh    eh   dh   ch   bh   ah   9h   8h
  //  2    17h   16h  15h  14h  13h  12h  11h  10h
  //  3    1fh   1eh  1dh  1ch  1bh  1ah  19h  18h
  //  
  // OnOff = 1 = on, OnOff = 0 = off  
  // DTB.SetDisplaySeg(byte Seg, byte OnOff);
  
  // display segment
  // Bit: 0--3 
  // Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
  // DTB.SetDisplayReg(byte Bit, byte Seg);
	
  // set segment flash 
  // Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
  // DTB.SetSegFlash(byte Bit, byte Seg);

  // set bit flash 
  // Bit = D7 D6 D5 D4 D3 D2 D1 D0
  //       -- -- -- -- b3 b2 b1 b0 (1=flash, 0=not flash)
  // DTB.SetBitFlash(byte Bit);

  // set flash speed： 0--255
  // DTB.SetFlashSpeed(byte Speed);
  
  // Clear the screen or light up all leds.
  // DTB.ClearAll();

  // Clear one bit display.
  // Bit = 0--3
  // DTB.ClearBit(byte Bit);
	
  // display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
  // Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  // Bit    : 0, 1, 2, 3
  // DTB.DisplayChar(byte Bit, byte Dat);
}

void loop() {
  // Force a byte variable to be converted to a float variable.
  float Num = float(KeyValue);
  // prints data with ending line break     
  
  if(Num != 0){
    // float parameter: num = 0.0-999.9
	// int parameter: num = 0-9999
    DTB.DisplayNumber(Num);
	// It is recommended that the refresh interval be at least 100ms
	delay(100);
	
	//            bit: 0 0 0 0 x x x x
    // Read key value: 0 0 0 0 U L R D
    Serial.println(KeyValue); 
  }
}











