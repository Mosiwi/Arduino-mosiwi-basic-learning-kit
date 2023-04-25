/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. LED
	2. What is a programming statement?
	3. Function and parameter
	4. Arduino programs must have setup() and loop() functions
	5. Digital pin output Settings 
	6. Use of the delay() function
	7. Annotation symbols: //, /*...* /
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-5-12
*/


// the setup function runs once when you press reset or power the board
void setup(){
  // initialize digital pin 5(Red LED D5) as an output.
  pinMode(5, OUTPUT);
}


// the loop function runs over and over again forever
void loop(){
  digitalWrite(5, HIGH);    // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second (1000ms = 1s)
  digitalWrite(5, LOW);     // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second (1000ms = 1s) 
}

