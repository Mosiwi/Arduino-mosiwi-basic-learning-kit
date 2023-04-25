/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Button
	2. Variable: byte, global variable
	3. Assignment operator: =
	4. Digital pin input Settings
	5. Relational operator: ==, !=
	6. Judgment statement: if
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-5-13
*/

// Global variable. 
byte buttonPin;          
byte ledPin;          
byte buttonState;         

// the setup function runs once when you press reset or power the board
void setup() {
  // the number of the pushbutton pin(Button D2)
  buttonPin   = 2; 
  // the number of the LED pin(Red LED D5)     
  ledPin      = 5;
  // variable for reading the pushbutton status         
  buttonState = 0; 

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