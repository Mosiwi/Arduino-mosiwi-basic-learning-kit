/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
  1. Buzzer
  2. MOSFET  
  3. function: tone(), notone()
     https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-3
*/

// Define the buzzer using the 9 pin of the M328 PRO.
byte BuzzerPin   = 9;        

void setup() {
  // Syntax:
  // tone(pin, frequency)
  // tone(pin, frequency, duration)
  
  // Parameters:
  // pin: the Arduino pin on which to generate the tone.
  // frequency: the frequency of the tone in hertz. Allowed data types: unsigned int.
  //            The range of frequencies: 31Hz -- 8MHz
  // duration: the duration of the tone in milliseconds (optional). Allowed data types: unsigned long.
  
  // Notes and Warnings:
  // If you want to play different pitches on multiple pins, 
  // you need to call noTone() on one pin before calling tone() on the next pin.
  // Use of the tone() function will interfere with PWM output on pins 3 and 11 (timer2)
}

void loop() {
  // play a note on BuzzerPin for 1000 ms:
  tone(BuzzerPin, 440, 1000);
  delay(2000);
  
  // play a note on BuzzerPin for 1000 ms:
  tone(BuzzerPin, 880);
  delay(1000);
  // turn off tone function for pin BuzzerPin:
  noTone(BuzzerPin);
  delay(1000);
}
