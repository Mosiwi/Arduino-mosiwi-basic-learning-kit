/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. LED
	2. Arduino programs must have setup() and loop() functions
	3. What is a programming statement?
	4. Annotation symbols: //, /*...* /
	5. Digital pin output settings function 
	6. Function parameter
	7. Use of the delay() function
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
*/


// the setup function runs once when you press reset or power the board
void setup(){
  // initialize digital pin 5(Blue LED D5) as an output.
  pinMode(5, OUTPUT);
}


// the loop function runs over and over again forever
void loop(){
  digitalWrite(5, HIGH);    // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second (1000ms = 1s)
  digitalWrite(5, LOW);     // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second (1000ms = 1s) 
}

