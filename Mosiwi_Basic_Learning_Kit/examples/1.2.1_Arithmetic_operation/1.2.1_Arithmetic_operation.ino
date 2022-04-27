/*
 The circuit: No external hardware needed.
 
 Learning target:
	1. Serial
	2. Serial monitor
	3. Use of two serial port functions
	4. Arithmetic operator: +, -, *, /, %
	
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
 Designer: jalen
 Date：2022-2-12
*/

byte num = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


// the loop function runs over and over again forever
void loop() {	
  num = 10+1-3;
  Serial.print("10+1-3 = ");   
  Serial.println(num);        
  
  num = 2*5;
  Serial.print("2*5 = ");  
  Serial.println(num);         
  
  num = 5/2;
  Serial.print("5/2 = ");    
  Serial.println(num);        
  
  num = 5%2;
  Serial.print("5%2 = ");      
  Serial.println(num);        
  
  delay(2000);          

}


