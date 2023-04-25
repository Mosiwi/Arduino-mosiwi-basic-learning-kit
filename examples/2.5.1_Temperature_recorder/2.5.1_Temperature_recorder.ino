/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
    1. Thermohygrometer
	2. variable: long, unsigned long
	3. Function: millis()
	4. Function with arguments
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-20
 */
#include <MswHT.h>
#include <MswEEPROM.h>
#include <MswDT_Button.h>

// _recording_period  = second
// 15  minute  = 90
// 1   minute  = 60
// 30 seconds  = 30
// 10 seconds  = 10
#define _recording_period  30  // 30 seconds

int _second_counter = 0;
int _recording_period_counter = 0;

// An array of temporarily recorded temperatures.
byte Data[128] = {0};

// One Wire pin, change according to your needs. 
const int ONE_WIRE_PIN = 7; 
// Create two classes of objects
OneWire oneWire(ONE_WIRE_PIN);
GX2431 eeprom(oneWire);

unsigned long oldTime = 0;
byte KeyValue = 0;

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

// Interrupt function with no return value
void GetKeyValue(void){
  //            bit: 0 0 0 x x x x x
  // Read key value: 0 0 0 U D L R OK
  KeyValue = DTB.ReadKeyValue();
  
  if(KeyValue == 1){ // OK button
	  byte data[128];
    eeprom.read(0, data, sizeof(data));
    Serial.println("Temperature recorded by eeprom :");
    for(int i=0; i<128; i++){
      Serial.print(data[i]);
      Serial.print(" ");
      if((i%8==7) && (i!=0))
        Serial.println("");			
    } 
    Serial.println("");
  }
}

void setup() {
  Serial.begin(9600);
  
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
 
  // Use the external interrupt function to get the key value.
  attachInterrupt(digitalPinToInterrupt(3), GetKeyValue, FALLING);
  
  if(!oneWire.reset()){
    // EEPEOM initialization error, 4 digit digital tube display "EEEE".
    DTB.DisplayChar(0, 14);
    DTB.DisplayChar(1, 14);
    DTB.DisplayChar(2, 14);
    DTB.DisplayChar(3, 14);
    return;
  }
}
	
void loop() { 
  // Stores temperature and humidity data.
  float HT_data[2];
  // Read AHT20 temperature and humidity data.
  MswHT.Read_CTdata(HT_data);                
  
  // Execute the program inside the if statement every 1 second.
  if((millis() - oldTime) >= 1000){
	  oldTime = millis();
	  _second_counter ++;
	  
	  // 4 digit digital display temperature value.
	  displayTemperature(HT_data[1]);
	
    // This statement is executed when the record period is reached.
    if(_second_counter >= _recording_period){
      _second_counter = 0;
      
      Data[_recording_period_counter] = HT_data[1];
      eeprom.write((_recording_period_counter/8)*8, &Data[(_recording_period_counter/8)*8], 8);
      _recording_period_counter ++;
      
      // 0-127: A maximum of 128 data can be recorded.
      // When the number of records reaches 127, all recorded data will be cleared.
      if(_recording_period_counter == 128){
        _recording_period_counter = 0;
        for(int i=0; i<128; i++)
          Data[i] = 0;
      }
    }
  }
}

















