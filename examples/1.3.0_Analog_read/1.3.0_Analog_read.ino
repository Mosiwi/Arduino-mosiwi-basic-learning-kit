/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Slide potentiometer
	2. voltage
	3. Analog input port
	4. Variable: int, float
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
 */
 
// These constants won't change. They're used to give names to the pins used:
byte analogInPin = A7;       // Analog input pin that the potentiometer is attached to

int analogValue       = 0;   // value read from the pot
float voltageValue    = 0;   // Storage voltage value

// the setup function runs once when you press reset or power the board
void setup(){
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


// the loop function runs over and over again forever
void loop(){
  // Read the analog value in the potentiometer.
  analogValue = analogRead(analogInPin);
  // Calculated voltage
  voltageValue = 5.0/1024*analogValue;
  
  // print the results to the Serial Monitor:
  Serial.print("Analog value = ");
  Serial.println(analogValue);  
  // print the voltage value to the Serial Monitor:
  Serial.print("Voltage value = ");
  Serial.println(voltageValue);

  // wait 20 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(200);
}





