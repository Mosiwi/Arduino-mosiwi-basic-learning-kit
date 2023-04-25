/*
 The circuit: No external hardware needed.
 
 Learning target:
	1. Serial
	2. Serial monitor
	3. Use of two serial port functions
	4. Variable: char, String
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/

char a  = 'a';
String Str1=" ASCII-encoded decimal: ";

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps.
  Serial.begin(9600);
}


// the loop function runs over and over again forever
void loop() {	        
  Serial.print("Print character: ");        // Prints title with no ending line break    
  Serial.print(a); 
  Serial.print(Str1); 
  Serial.println(a, DEC);                   // prints data with ending line break 
  
  delay(2000);                             

}


