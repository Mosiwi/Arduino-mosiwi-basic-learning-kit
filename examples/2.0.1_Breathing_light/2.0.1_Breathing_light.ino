/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. random(min,max);
	2. ternary conditional operator: Expression 1? Expression 2: Expression 3;
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-22
*/

const byte R_led = 5;
const byte G_led = 6;
const byte B_led = 9;
byte random_num = 0;
byte RGB_pin = 0;

void setup() {
    pinMode(R_led,OUTPUT);
    pinMode(G_led,OUTPUT);
    pinMode(B_led,OUTPUT);
}

void loop(){
	//Randomly obtain values between 0-3.
	random_num = random(1,4);
	RGB_pin = random_num == 1 ? R_led : (random_num == 2 ? G_led : B_led);
    fadeOn(1000,5);
    fadeOff(1000,5);
}

void fadeOn(unsigned int time,int increament){	
	for (byte value = 0 ; value < 255; value+=increament){ 
		analogWrite(RGB_pin, value);
		delay(time/(255/increament));
	} 
}

void fadeOff(unsigned int time,int decreament){
	for (byte value = 255; value >0; value-=decreament){ 
		analogWrite(RGB_pin, value); 
		delay(time/(255/decreament)); 
	}
}


