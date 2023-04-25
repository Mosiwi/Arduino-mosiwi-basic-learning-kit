/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Use of timer and buzzer
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-5
 */

//Include library file
#include <MswTimer2.h>

// Define the buzzer using the 10 pin of the M328 PRO(UNO R3).
byte BuzzerPin   = 10;  

// Interrupt function
void tone(void) {  
  // The program is initialized only once when it runs.
  static bool output = HIGH;
  
  digitalWrite(BuzzerPin, output);
  output = !output;
}

void set_frequency(int frequency){
  // Set interrupt time, 1S = 1000mS
  int time = 1000/frequency;

  // Set interrupt function
  MsTimer2::set(time, tone); 
  
  // Start interrupt function
  MsTimer2::start();
  // Stop interrupt function
  // MsTimer2::stop();
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(BuzzerPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for(int i=100; i<=800; i=i+50){
    set_frequency(i);
    delay(200);
  }
}

