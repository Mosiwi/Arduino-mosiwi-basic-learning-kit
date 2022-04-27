/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. A function with a return value
	2. Logical operation: ||
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-6
 */
 //Include library file
#include <MswDT_Button.h>

#define Echo A3
#define Trig A2

// Key value signal output flag pin.
byte keySignalPin = 3;  
byte keyValue = 0;        
bool OnOff = false;

// Ultrasonic measuring distance
float Measuring_distance(void){
  unsigned long rxTime;
  float d;
  // The Trig pin sends a 10uS high level trigger pulse signal.
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW); 
  
  // The Echo pin captures a feedback high level pulse signal(in microseconds). 
  rxTime = pulseIn(Echo, HIGH);  
  // Convert microseconds to seconds: rxTime/1000000.0  --->  1s = 1000000us
  // The speed of ultrasound: 340m/s = 34000cm/s. 
  // Because the data measured by ultrasound has two distances back and forth, the data has to be divided by 2.
  // Convert the pulse signal to a distance in centimeters.
  d = ((rxTime/1000000.0) * 34000.0) / 2;
  return d;
}

// Read the button value.
void GetKeyValue(void){
  keyValue = DTB.ReadKeyValue();
  if(keyValue == 8 || keyValue == 4 || keyValue == 2 || keyValue == 1){
    OnOff = !OnOff;
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  // Initialize the ultrasonic control pins
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trig, LOW); 
  
  // Initialize interrupt pins with a pull resistor.
  pinMode(keySignalPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(keySignalPin), GetKeyValue, FALLING);
  
  // Initialize SPI communications.
  DTB.Start_Init();
}

// put your main code here, to run repeatedly:
void loop() {
  if(OnOff){
    float distance;
    // Ultrasonic measuring distance
    distance = Measuring_distance();
  
    DTB.DisplayNumber(distance);
    // It is recommended that the refresh interval be at least 100ms
    delay(100);
  }else{
	// Clear display
	DTB.ClearAll();
	delay(100);
  }
}
