/*
 * Project: 74HC595 3wire soft communication protocol
 * Function: Controls 8 leds.  
 * Github: https://github.com/mosiwi
 * Wiki: http://wiki.mosiwi.com
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

// Pin connected to ST_CP of 74HC595
byte latchPin = 8;
// Pin connected to SH_CP of 74HC595
byte clockPin = A1;
// Pin connected to DS of 74HC595
byte dataPin = A0;

// Used to store LEDs display data.
byte ledData = 0;

// LED state
byte ON  = 1;
byte OFF = 0;

////////////////////////////////////////////////////////////// 
void Init_LED_IO(byte latchP, byte clockP, byte dataP){ 
  //set pins to output because they are addressed in the main loop
  pinMode(latchP, OUTPUT);
  pinMode(clockP, OUTPUT);
  pinMode(dataP, OUTPUT);
}

//////////////////////////////////////////////////////////////
// Send 8-bit data to 74HC595.
// bitOrder: MSBFIRST or LSBFIRST
void ShiftOut(byte dataPin, byte clockPin, byte bitOrder, byte val){
	byte i;
	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, val & 1);
			val >>= 1;
		} else {	
			digitalWrite(dataPin, (val & 128) != 0);
			val <<= 1;
		}
   
    digitalWrite(clockPin, LOW);
    delayMicroseconds(10);  		
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(10);
	}
}

//////////////////////////////////////////////////////////////
// Turn on or off LEDs
// bit: 0-8, OnOff: ON or OFF
void SetLed(byte bit, byte OnOff){
	if(bit > 8 || OnOff > 1){
		return;
	}
	if(OnOff == 1){
		ledData = ledData | (1 << bit);
	}
	else{
		ledData = ledData & (~(1 << bit));
	}

  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, LOW);
  ShiftOut(dataPin, clockPin, MSBFIRST, ledData);
  //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);
}

//////////////////////////////////////////////////////////////
void setup(){
	//set pins to output so you can control the shift register;
	Init_LED_IO(latchPin, clockPin, dataPin);
}

//////////////////////////////////////////////////////////////
void loop() {
	int a;
	for(a=0; a<=7; a++){
		SetLed(a, ON);
		delay(1000);
	}
	for(a=7; a>=0; a--){
		SetLed(a, OFF);
		delay(1000);
	}
}
