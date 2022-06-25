/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. Improve your coding skills

 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-20
 */
#include <MswTemperature.h>
#include <MswDT_Button.h>
#include <MswLed.h>

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

// Create a temperature object
DS18B20 ds(7);
float T = 0;

// Fan speed
int Speed = 60;
// Fan switch
bool OnOff = false;
// Temperature control flag.
bool TC_flag = false;
const byte G_led = 6;

// Switch indicator light
const byte R_led = 10;

// Store the system running time.
int oldTime = 0;

// High temperature enable threshold
byte High_temperature_warning = 35;
// Low temperature disable threshold
byte Low_temperature_warning = 16;

// Analog input pin that the Slide potentiometer is attached to
byte analogInPin = A7;   

// the setup function runs once when you press reset or power the board
void setup() {
  // The infrared receiver function is enabled
  IR.enable(); 

  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
  
  // Fan control pins: A2, A3
  MswFan::init(A2, A3);
  // clockwise rotation
  MswFan::direction(CW);
  
  // Setting fan stop
  MswFan::stop();
  
  // Initialize the 8-segment digital display tube 
  DTB.Start_Init();
  
  // Initialize LED, turn on LED when temperature control mode is on, 
  // turn off LED otherwise. 
  pinMode(G_led, OUTPUT);
  digitalWrite(G_led, LOW);
  
  // Set temperature accuracy.
  ds.setResolution(10);
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
    ds.doConversion();
    T = ds.getTempC();
    DTB.DisplayNumber(T);
    oldTime = millis();
  }
  
  // The temperature control fan is enabled
  if(TC_flag){
	// Enable fan
	if(T > High_temperature_warning){
	  OnOff = true;
	// Disable fan
	}else if(T < Low_temperature_warning){
	  OnOff = false;
	}
	// Enable temperature control indicator LED
	digitalWrite(G_led, HIGH);
  }
  // The temperature control fan is disabled
  else{ 
    // Disable temperature control indicator LED
    digitalWrite(G_led, LOW);
  }
  
  if(OnOff){
    // Setting fan run
    MswFan::run();
    MswFan::speed(Speed);
    digitalWrite(R_led, HIGH);
  }else{
    // Setting fan stop
    MswFan::stop();
    digitalWrite(R_led, LOW);
  }
}

