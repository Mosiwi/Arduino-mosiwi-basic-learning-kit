/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Logical operation: &&
	2. Jump statements: goto
	3. Function: max(), min()
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-7
 */
 #include <MswLed.h>
 
// Analog input pin that the microphone is attached to
byte microphoneInPin = A6;       
// Used to store analog value of microphone
int voiceAnalogValue     = 0;   

// Regulatory sensitivity
#define sensitivity 5
int up_0 = 0 + sensitivity*50;
int dow_1023 = 1023 - sensitivity*50;

// Adjust the first LED light flashing.
#define adj_flash 1
int up_512  = 512 + adj_flash*5;
int dow_512 = 512 - adj_flash*5;

// the setup function runs once when you press reset or power the board
void setup(){
  // Limit the maximum value of the up_0 variable to 461.
  // Limit the minimum value of the dow_1023 variable to 563.
  up_0  = min(up_0, 461);         // Get the minimum, 461 = 462 - 1
  dow_1023 = max(dow_1023, 563);  // Get the maximum, 563 = 562 + 1

  // Limit the maximum value of the up_512 variable to 562.
  // Limit the minimum value of the dow_512 variable to 462.
  up_512  = min(up_512, 562);     // Get the minimum, 562 = 512 + 50
  dow_512 = max(dow_512, 462);    // Get the maximum, 462 = 512 - 50

  // Initialize the control pin of led
  // Parameter1 = ST_CP, Parameter2 = SH_CP, Parameter3 = DS
  Init_LED_IO(8, A1, A0);
}

// The loop function runs over and over again forever.
void loop(){
  // readAnalog label
  readAnalog:
  // Read the voice value of the microphone.
  voiceAnalogValue  = analogRead(microphoneInPin);

  byte ledNum = 0;
  if((up_512 <= voiceAnalogValue) && (voiceAnalogValue <= dow_1023)){
    ledNum = map(voiceAnalogValue, up_512, dow_1023, 0, 7);
  }
  else if((up_0 <= voiceAnalogValue) && (voiceAnalogValue <= dow_512)){
    ledNum = map(voiceAnalogValue, dow_512, up_0, 0, 7);
  }
  else{
    SetLedBar(0b00000000);
	// Jump to readAnalog label
    goto readAnalog;
  }

  byte leds = 0;
  int i = 0;
  for(i = 0; i <= ledNum; i++){
    leds = leds | (1 << i);
    SetLedBar(leds);
    delay(10*i);
  }
  for(i = ledNum; i >= 0; i--){
    leds = leds & (~(1 << i));
    SetLedBar(leds);
    delay(10*i);
  }
}





