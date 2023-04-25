/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. Improve your coding skills

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-20
 */
#include <MswDT_Button.h>
#include <MswLed.h>
#include <MswHT.h>

// Use timer 1, generate 400 Hz PWM signal
//#define FAN_USE_TIMER1
// Use timer 2, generate 80 Hz PWM signal
#define FAN_USE_TIMER2
#include <MswFan.h>

// Use timer 1
#define IR_USE_TIMER1
// Use timer 2
//#define IR_USE_TIMER2
#include <MswIR.h>

// Define infrared receiving pins
byte RECV_PIN = 4;
// Create an infrared receiver class
MswIR IR(RECV_PIN);

// Variables that record temperature
float HT_data[2];

// Fan speed
int Speed = 60;
// Fan switch
bool OnOff = false;
// Temperature control flag.
bool TC_flag = false;

// High temperature alarm LED.
const byte B_led = 9;

// Store the system running time.
int oldTime = 0;

// High temperature enable threshold
byte High_temperature_warning = 35;
// Low temperature disable threshold
byte Low_temperature_warning = 16;

// Analog input pin that the Slide potentiometer is attached to
byte analogInPin = A3;   

// the setup function runs once when you press reset or power the board
void setup() {
  // The infrared receiver function is enabled
  IR.enable(); 

  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
  
  // Fan control pins: 5, 6
  MswFan::init(5, 6);
  // clockwise rotation
  MswFan::direction(CW); 
  // Setting fan stop
  MswFan::stop();
  
  // Initialize the 8-segment digital display tube 
  DTB.Start_Init();
  
  // Initialize LED, turn on LED when temperature control mode is on, 
  // turn off LED otherwise. 
  pinMode(B_led, OUTPUT);
  digitalWrite(B_led, LOW);
  
  // Initialize the temperature and humidity sensor.
  if((MswHT.Read_Status() & 0x18) != 0x18){
    MswHT.Start_Init();                  
    delay(10);     
  }
}

// put your main code here, to run repeatedly:
void loop() { 
  // decode and store the decoded data.
  if (IR.decode()) {  	
	switch (IR.value){
	  case 0xff18e7: Speed += 30; break;           //▲, speed up
	  case 0xff4ab5: Speed -= 30; break;           //▼, speed cut
	  case 0xff38c7: OnOff = !OnOff; break;        //OK, Turn off or on the fan
	  case 0xff629d: TC_flag = !TC_flag; break;    //2, Turn off or on the temperature control fan
	  default: break;
	}
  }
  if(Speed > 240)
	  Speed = 240;
  if(Speed < 0)
	  Speed = 0;
  
  // Fan speed indicator.
  byte ledNum = map(Speed, 0, 240, 0, 8);
  byte displayData = 0;
  while(ledNum > 0){
	// same as: displayData = displayData | (1 << (ledNum-1));
    displayData |= 1 << (ledNum-1);
	ledNum --;
  }
  SetLedBar(displayData);  
  
  //Display frequency is 1
  if((millis()-oldTime) > 1000){
    // Conduct temperature measurement
    MswHT.Read_CTdata(HT_data);
    DTB.DisplayNumber(HT_data[1]);
    oldTime = millis();
  }
  
  // The temperature control fan is enabled
  if(TC_flag){
	// Enable fan
	if(HT_data[1] > High_temperature_warning){
	  OnOff = true;
	// Disable fan
	}else if(HT_data[1] < Low_temperature_warning){
	  OnOff = false;
	}
	// Enable temperature control indicator LED
	digitalWrite(B_led, HIGH);
  }
  // The temperature control fan is disabled
  else{ 
    // Disable temperature control indicator LED
    digitalWrite(B_led, LOW);
  }
  
  if(OnOff){
    // Setting fan run
    MswFan::run();
    MswFan::speed(Speed);
  }else{
    // Setting fan stop
    MswFan::stop();
  }
}

