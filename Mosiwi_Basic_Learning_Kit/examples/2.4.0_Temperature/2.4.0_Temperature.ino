/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Temperature
	2. class
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-16
*/
#include <MswTemperature.h>
// Create a temperature object
DS18B20 ds(10);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Conduct temperature measurement
  ds.doConversion();
  
  // Output Celsius temperature
  Serial.print("Temperature: ");
  Serial.print(ds.getTempC());   
  Serial.println(" C");   
	
  // Output Fahrenheit
  Serial.print("Temperature: ");
  Serial.print(ds.getTempF());
  Serial.println(" F");

  delay(1000);
}
