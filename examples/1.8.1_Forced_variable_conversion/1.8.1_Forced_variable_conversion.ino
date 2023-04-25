/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. Forced variable conversion
	2. Loop statement: while(1)
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-20
 */

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  Serial.print("byte(9.9) = ");
  Serial.println(byte(9.9)); 
  
  Serial.print("byte('a') = ");
  Serial.println(byte('a')); 
  
  Serial.print("char(97.3) = ");
  Serial.println(char(97.3)); 
  
  Serial.print("char(97) = ");
  Serial.println(char(97));

  Serial.print("int(97.3) = ");
  Serial.println(int(97.3)); 
  
  Serial.print("int('a') = ");
  Serial.println(int('a'));

  Serial.print("float(97) = ");
  Serial.println(float(97)); 
  
  Serial.print("float('a') = ");
  Serial.println(float('a'));

  while(1);
}
