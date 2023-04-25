/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
  1. infrared receiver 
  
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-10
*/
// Use timer 1
//#define IR_USE_TIMER1
// Use timer 2
#define IR_USE_TIMER2

#include <MswIR.h>

// Define infrared receiving pins
byte RECV_PIN = 4;

// Create an infrared receiver class
MswIR IR(RECV_PIN);

void setup()
{
  Serial.begin(9600);
  // The infrared receiver function is enabled
  IR.enable(); 
}

void loop() {
  //Key value of remote control
  //0xff18e7: ▲
  //0xff4ab5: ▼
  //0xff10ef: ◄
  //0xff5aa5: ►
  //0xff38c7: OK
  //0xff9867: 0
  //0xffa25d: 1
  //0xff629d: 2
  //0xffe21d: 3
  //0xff22dd: 4	
  //0xff02fd: 5
  //0xffc23d: 6
  //0xffe01f: 7
  //0xffa857: 8	
  //0xff906f: 9  
  //0xff6897: *   
  //0xffb04f: #   
		
  // decode and store the decoded data.
  if (IR.decode()) {  
    // Prints the decoded data to the serial port monitor
    Serial.println(IR.value, HEX);
  }
}
