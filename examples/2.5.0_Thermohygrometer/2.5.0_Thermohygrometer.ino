/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
    1. Thermohygrometer
	2. variable: long, unsigned long
	3. Function: millis(), abs()
	4. Function with arguments
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-20
 */
#include <MswHT.h>
#include <MswDT_Button.h>

unsigned long oldTime = 0;

void displayTemperature(float T){
  int temp = int(T*10);
  // display C char
  DTB.SetDisplayReg(3, 0xC6);
  // Turn on the decimal point.
  DTB.SetDisplaySeg(0x0f, 1);          
  
  // display temperature
  DTB.DisplayChar(0, temp/100);
  DTB.DisplayChar(1, temp%100/10);
  DTB.DisplayChar(2, temp%10);
}

void displayHumidity(float H){
  int temp = int(H*10);
  // display H char
  DTB.SetDisplayReg(3, 0x89);
  // Turn on the decimal point.
  DTB.SetDisplaySeg(0x0f, 1);          
  
  // display temperature
  DTB.DisplayChar(0, temp/100);
  DTB.DisplayChar(1, temp%100/10);
  DTB.DisplayChar(2, temp%10);
}

void setup() {
  // When the chip is powered on for the first time, the system reads the status word 
  // by sending 0x71 and determines whether the status word is 0x18. 
  // If it is not 0x18, the register is initialized.  
  if((MswHT.Read_Status() & 0x18) != 0x18){
    // Reinitialize the register 
    MswHT.Start_Init();                  
    delay(10);     
  }

  // Initialize SPI communications.
  DTB.Start_Init();
  
  // Clear the screen or light up all leds.
  DTB.ClearAll();
}
	
void loop() { 
  // Stores temperature and humidity data.
  float HT_data[2];

  // Read AHT20 temperature and humidity data.
  MswHT.Read_CTdata(HT_data);                
  
  if((millis() - oldTime) < 5000){
	displayTemperature(HT_data[1]);
  }
  
  if((millis() - oldTime) >= 5000){
	displayHumidity(HT_data[0]);
  }
  
  if(abs(millis() - oldTime) >= 10000){
    oldTime = millis();
  }
}
