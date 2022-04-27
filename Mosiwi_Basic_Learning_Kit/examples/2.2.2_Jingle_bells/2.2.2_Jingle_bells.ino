/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
  1. Music
  2. Function: sizeof()
  3. Store operation: PROGMEM, pgm_read_word_near()
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-4-19
*/
// F Tone Bass
#define NTFL1 175
#define NTFL2 196
#define NTFL3 221
#define NTFL4 234
#define NTFL5 262
#define NTFL6 294
#define NTFL7 330

// F Tone Alto, NTF0 is empty
#define NTF0 -1
#define NTF1 350
#define NTF2 393	
#define NTF3 441
#define NTF4 495
#define NTF5 556
#define NTF6 624
#define NTF7 661

// F Tone Treble 
#define NTFH1 700
#define NTFH2 786
#define NTFH3 882
#define NTFH4 935
#define NTFH5 965
#define NTFH6 996
#define NTFH7 1023

// array of note frequencies
int tune[]={
	NTF3,NTF3,NTF3,NTF3,NTF3,NTF3,
	NTF3,NTF5,NTF1,NTF2,NTF3,NTF0,
	NTF4,NTF4,NTF4,NTF4,NTF4,NTF3,NTF3,NTF3,NTF3,
	NTF5,NTF5,NTF4,NTF2,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,NTFL5,NTFL5,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,
	NTFL6,NTF4,NTF3,NTF2,NTFL7,NTF0,
	NTF5,NTF5,NTF4,NTF2,NTF3,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,NTFL6,
	NTFL6,NTF4,NTF3,NTF2,NTF5,NTF5,NTF5,NTF5,
	NTF6,NTF5,NTF4,NTF2,NTF1,NTF0
};

// array of note beats
// Store data in flash (program) memory instead of SRAM.
const PROGMEM float durt[]={
	0.5,0.5,1,0.5,0.5,1,
	0.5,0.5,0.75,0.25,1.5,0.5,
	0.5,0.5,1,0.5,0.5,0.5,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1.5,0.5,

	0.5,0.5,0.5,0.5,1,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.75,0.25,

	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.5,0.5,
	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
	0.5,0.5,0.5,0.5,0.75,0.25
};

// Define buzzer pin, note length variable.
int buzzer_pin = 9;
int length;

void setup(){
	// Initialize the pin, calculate the length
	pinMode(buzzer_pin, OUTPUT);
	length = sizeof(tune)/sizeof(tune[0]);
}

void loop(){
	// for loop to play the song
	for(int x=0;x<length;x++){
		tone(buzzer_pin, tune[x]);
		// 500 here is the unit of duration to control each note
		delay(500*pgm_read_word_near(durt+x));					
		noTone(buzzer_pin);
	}
	// The time interval to start the next cycle
	delay(1000);								
}



