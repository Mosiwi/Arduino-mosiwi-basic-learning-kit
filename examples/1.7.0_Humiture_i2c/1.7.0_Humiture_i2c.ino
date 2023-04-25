/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. i2c communication
	2. Variable: array
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-20
 */
#include <MswHT.h>

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // It takes 100 to 500ms for the chip to be ready after it is powered on. 500ms is recommended
  delay(500);
  // When the chip is powered on for the first time, the system reads the status word 
  // by sending 0x71 and determines whether the status word is 0x18. 
  // If it is not 0x18, the register is initialized.  
  if((MswHT.Read_Status() & 0x18) != 0x18){
	Serial.println("Initialize register..."); 
    // Reinitialize the register 
    MswHT.Start_Init();                  
    delay(10);     
  }
}


void loop() {
  float HT_data[2];

  // You are advised to read the data every 1S  
  // Function prototype: MswHT.Read_CTdata(float *ct);
  MswHT.Read_CTdata(HT_data);                // Read AHT20 temperature and humidity data directly without CRC check.
  
  // Function prototype: MswHT.Read_CTdata_crc(float *ct);
  // MswHT.Read_CTdata_crc(HT_data);         // After CRC verification, read temperature and humidity data of AHT20 
  delay(1000); 
  
  Serial.print("Humidity = ");      
  Serial.println(HT_data[0]);
  Serial.print("Temperature = ");      
  Serial.println(HT_data[1]);
}
