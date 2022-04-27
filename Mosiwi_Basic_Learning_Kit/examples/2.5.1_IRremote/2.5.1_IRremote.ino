/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  
 
 Learning target:
  1. Single bus protocol
  
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-20
*/
// Use timer 1
//#define IR_USE_TIMER1
// Use timer 2
#define IR_USE_TIMER2
#include <MswIR.h>

// Define infrared receiving pins
int RECV_PIN = 4;
// Create an infrared receiver class
MswIR IR(4);

// For the led
const byte R_led = 5;
byte OnOff = 0;

void setup(){
  // The infrared receiver function is enabled
  IR.enable(); 
  
  // Initialize LED pins as an output.
  pinMode(R_led, OUTPUT);
  digitalWrite(R_led, LOW);
}

void loop() {		
  // decode and store the decoded data.
  if (IR.decode()) {  
    if(IR.value == 0xff38c7)
      OnOff = ~OnOff;
  }
  digitalWrite(R_led, OnOff);
}
