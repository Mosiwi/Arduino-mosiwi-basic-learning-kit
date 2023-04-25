/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. EEPROM (Electrically Erasable Programmable Read-Only Memory)
	2. 1-Wire protocol
	3. variable: word
	3. class
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
*/
#include <MswEEPROM.h>
// One Wire pin, change according to your needs. 
const int ONE_WIRE_PIN = 7; 

// Create two classes of objects
OneWire oneWire(ONE_WIRE_PIN);
GX2431 eeprom(oneWire);

void setup() {
  Serial.begin(9600);
  
  // 1.Find if there is a device on the 1-wire bus.
  if(!oneWire.reset()){
    Serial.println("No GX2431 found on the 1-Wire bus.");
    return;
  }

  // 2.Read all memory content
  byte data[128];
  eeprom.read(0, data, sizeof(data));
  Serial.print("Memory contents : ");
  for(uint8_t i=0;i<128;i++){
    if(i%8 == 0)
      Serial.println("");
	  
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");

  // 3.Write data to address 0-7.
  byte newData[] = {60,77,111,115,105,119,105,62};
  //The address must be : 8*n --> between 0 and 120
  word address = 0;
  // Write a 8-byte row, must write 8 bytes at a time.
  if (eeprom.write(address, newData, sizeof(newData))){
    Serial.print("Successfully wrote data to address 0-7: 60 77 111 115 105 119 105 62");
    Serial.println(address);
  }
  else{
    Serial.print("Failed wrote data to address 0-7!");
    Serial.println(address);
  }
  Serial.println("");

  // 4.Reread memory content
  eeprom.read(0, data, sizeof(data));
  Serial.print("Reread memory contents : ");
  for(uint8_t i=0;i<128;i++){
    if(i%8 == 0)
      Serial.println("");
	  
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");

  // 5.To convert numbers to characters.
  Serial.print("Our team: ");
  for(uint8_t i=0;i<8;i++){
    Serial.print(char(data[i]));
  }
  Serial.println("");
}

void loop() {
  // Nothing to do
}



