/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. PWM output port
	2. Loop statement: for 
	3. Arithmetic operator: --, ++
	4. Relational operator: >, <, >=, <=
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-3-20
 */

// These constants won't change. They're used to give names to the pins used:
const byte pwmOutPin   = 5;         // Analog output pin that the LED(D5) is attached to
int pwmValue    = 0;                // value output to the PWM (analog out)


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


// the loop function runs over and over again forever
void loop() {
  // Judgment loop statement 
  Serial.println("Test <= 255 : --------------------------");
  delay(1000);
  for(pwmValue = 0; pwmValue <= 255; pwmValue ++){
  // change the analog out value:
  analogWrite(pwmOutPin, pwmValue);
  
  // prints title with ending line break
  Serial.println(pwmValue);
  delay(20);
  }

  // Judgment loop statement 
  Serial.println("Test >=0 : --------------------------");
  delay(1000);
  for(pwmValue = 255; pwmValue >= 0; pwmValue --){
  // change the analog out value:
  analogWrite(pwmOutPin, pwmValue);

  // prints title with ending line break
  Serial.println(pwmValue);
  delay(20);
  }

  // Judgment loop statement 
  Serial.println("Test < 256 : ---------------------------");
  delay(1000);
  for(pwmValue = 0; pwmValue < 256; pwmValue ++){
  // change the analog out value:
  analogWrite(pwmOutPin, pwmValue);
  
  // prints title with ending line break
  Serial.println(pwmValue);
  delay(10);
  }

  // Judgment loop statement 
  Serial.println("Test > -1 : --------------------------");
  delay(1000);
  for(pwmValue = 255; pwmValue > -1; pwmValue --){
  // change the analog out value:
  analogWrite(pwmOutPin, pwmValue);

  // prints title with ending line break
  Serial.println(pwmValue);
  delay(10);
  }
}




