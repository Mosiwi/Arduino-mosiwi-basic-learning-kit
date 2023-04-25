/*
 This example works with the Mosiwi Basic learning board. 
 
 Learning target:
	1. The speed of the fan is controlled by PWM

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
 */
#define INA 6
#define INB 5

// the setup function runs once when you press reset or power the board
void setup() {
  // Fan control pins: 6, 5
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
}

// put your main code here, to run repeatedly:
void loop() {
  int i;
  
  // clockwise rotation
  for(i = 0; i<150; i++){
	// Speed allowed range: i = 0-255
	analogWrite(INA, i);
    analogWrite(INB, 0);     
    delay(10);	
  }
  for(i = 150; i>0; i--){
	analogWrite(INA, i);
    analogWrite(INB, 0);     
    delay(10);	
  }
  
  // Counterclockwise rotation
  for(i = 0; i<150; i++){
	analogWrite(INA, 0);
    analogWrite(INB, i);     
    delay(10);	
  }
  for(i = 150; i>0; i--){
	analogWrite(INA, 0);
    analogWrite(INB, i);     
    delay(10);	
  }
}

