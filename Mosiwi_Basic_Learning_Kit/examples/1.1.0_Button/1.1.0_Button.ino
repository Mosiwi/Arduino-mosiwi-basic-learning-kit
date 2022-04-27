/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Button
	2. Digital pin input Settings: INPUT, INPUT_PULLUP, OUTPUT (HIGH, LOW)
	3. Variable: byte, global variable
	4. Relational operator: ==, !=
	5. Assignment operator: =
	6. Judgment statement: if
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
*/

// Global variable. They're used here to set pin numbers:
byte buttonPin = 2;          // the number of the pushbutton pin(Button D2)
byte ledPin    = 5;          // the number of the LED pin(Blue LED D5)

// Global variable
byte buttonState = 0;        // variable for reading the pushbutton status


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  // pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT);
}


// the loop function runs over and over again forever
void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } 
  if (buttonState != LOW){
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}