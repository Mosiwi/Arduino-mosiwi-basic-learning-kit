/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Temperature
	2. Single bus protocol
	3. class
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-16
*/
#include <MswTemperature.h>
// Create a temperature object
DS18B20 ds(7);

void setup() {
  Serial.begin(9600);
  
  // Setting precision: 9,    10,    11,    12 ---> 0.5, 0.25, 0.125, 0.0625
  // Function delay   : 94ms, 188ms, 375ms, 750ms
  //ds.setResolution(12);
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
