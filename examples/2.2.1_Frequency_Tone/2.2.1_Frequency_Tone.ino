/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
  1. The relationship between frequency and tone.
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-6
*/
 //Include library file
#include <MswDT_Button.h>

// Define the buzzer using the 9 pin of the M328 PRO.
byte BuzzerPin   = 9; 
// Analog input pin that the potentiometer is attached to       
byte analogInPin = A7;    
   
void setup() {
  // Initialize the 8-segment digital display tube and keys through SPI communication.
  DTB.Start_Init();
}

void loop() {
  // Read the analog value in the potentiometer.
  int analogValue = analogRead(analogInPin);
  // Map analog values to tones.
  byte note = map(analogValue, 0, 1023, 0, 7);
  
  switch(note){
	case 1: tone(BuzzerPin, 523); DTB.DisplayNumber(523); break;    // DO
	case 2: tone(BuzzerPin, 578); DTB.DisplayNumber(578); break;    // RE
	case 3: tone(BuzzerPin, 659); DTB.DisplayNumber(659); break;    // MI
	case 4: tone(BuzzerPin, 698); DTB.DisplayNumber(698); break;    // FA
	case 5: tone(BuzzerPin, 784); DTB.DisplayNumber(784); break;    // SO
	case 6: tone(BuzzerPin, 880); DTB.DisplayNumber(880); break;    // LA
	case 7: tone(BuzzerPin, 988); DTB.DisplayNumber(988); break;    // SI
	default: noTone(BuzzerPin); break;   // Turn off the buzzer.
  }
  delay(100);
}


