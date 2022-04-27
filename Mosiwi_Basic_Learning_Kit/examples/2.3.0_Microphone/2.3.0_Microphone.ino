/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. microphone
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
 */
 
// Analog input pin that the microphone is attached to
byte microphoneInPin = A6;       

int voiceAnalogValue     = 0;   // Used to store analog value of microphone
float voiceVoltageValue  = 0;   // Used to store voltage value of microphone

// the setup function runs once when you press reset or power the board
void setup(){
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

// The loop function runs over and over again forever.
void loop(){
  // Read the voice value of the microphone.
  voiceAnalogValue = analogRead(microphoneInPin);
  // Calculated voltage
  voiceVoltageValue = 5.0/1024*voiceAnalogValue;
  
  // print the results to the Serial Monitor:
  Serial.print("Voice-analog = ");
  Serial.println(voiceAnalogValue);  
  // print the voltage value to the Serial Monitor:
  Serial.print("Voice-voltage = ");
  Serial.println(voiceVoltageValue);

  // wait 20 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(200);
}





