/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Variable: local variable
	2. Judgment statement: if...else..., if...else if...else...
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/

// Defining character variables. They're used here to set pin numbers:
byte buttonPin = 2;          // the number of the pushbutton pin(Button D2)
byte ledPin    = 5;          // the number of the LED pin(Red LED D5)


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
  // local variable
  byte buttonState = 0;         // variable for reading the pushbutton status

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
  
  /************************Another judgment statement**********************
  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } 
  else if(buttonState == HIGH){
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
  else{
	// statement 
  }
  ************************************************************************/
}