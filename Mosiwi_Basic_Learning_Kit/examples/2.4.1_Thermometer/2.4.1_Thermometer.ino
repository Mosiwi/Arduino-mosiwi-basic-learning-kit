/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Thermometer
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-16
*/
#include <MswTemperature.h>
#include <MswDT_Button.h>

#define LOW_ALARM 20    // Alarm lower limit
#define HIGH_ALARM 30   // Alarm upper limit

// Create a temperature object
DS18B20 ds(10);

// Define the buzzer using the 9 pin of the M328 PRO.
byte BuzzerPin   = 9; 

void setup() {
  // Initialize the 8-segment digital display tube 
  // and keys through SPI communication. 
  DTB.Start_Init();
  
  // Set alarm temperature
  while (ds.selectNext()) {
    ds.setAlarms(LOW_ALARM, HIGH_ALARM);
  }
}

void loop() {
  // Conduct temperature measurement
  ds.doConversion();
  
  // An alarm signal is generated when the value is higher 
  // than the highest and lower than the lowest warning value.
  if (ds.selectNextAlarm()) {
    tone(BuzzerPin, 659);
	delay(500);
	noTone(BuzzerPin);
  }
  
  DTB.DisplayNumber(ds.getTempC());
}
