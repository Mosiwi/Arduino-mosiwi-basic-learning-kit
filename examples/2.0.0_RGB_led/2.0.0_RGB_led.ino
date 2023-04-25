/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. What is Resistor？
	2. Controls R,G,B leds(colour).
	3. Preprocessor: #define
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/

#define T 500      // delay 500ms

const byte R_led = 5;
const byte G_led = 6;
const byte B_led = 9;

// the setup function runs once when you press reset or power the board
void setup(){
  // Initialize LED pins as an output.
  pinMode(R_led, OUTPUT);
  pinMode(G_led, OUTPUT);
  pinMode(B_led, OUTPUT);
  // Turn off all leds
  digitalWrite(R_led, LOW);
  digitalWrite(G_led, LOW);
  digitalWrite(B_led, LOW);
}


// the loop function runs over and over again forever
void loop(){
  digitalWrite(R_led, HIGH);    // turn the red LED on 
  delay(T);                     // wait for 0.5 second (T = 500ms = 0.5s)
  digitalWrite(R_led, LOW);     // turn the red LED off 
  digitalWrite(G_led, HIGH);    // turn the green LED on 
  delay(T);                     // wait for 0.5 second (T = 500ms = 0.5s)
  digitalWrite(G_led, LOW);     // turn the green LED off 
  digitalWrite(B_led, HIGH);    // turn the blue LED on 
  delay(T);                     // wait for 0.5 second (T = 500ms = 0.5s)
  digitalWrite(B_led, LOW);     // turn the blue LED off
  
  // yellow light
  digitalWrite(R_led, HIGH);    
  digitalWrite(G_led, HIGH);   
  delay(T); 
  digitalWrite(R_led, LOW);     
  digitalWrite(G_led, LOW); 

  // cyan
  digitalWrite(G_led, HIGH);    
  digitalWrite(B_led, HIGH);
  delay(T);
  digitalWrite(G_led, LOW);     
  digitalWrite(B_led, LOW);
  
  // magenta
  digitalWrite(R_led, HIGH);       
  digitalWrite(B_led, HIGH);  
  delay(T);
  digitalWrite(R_led, LOW);         
  digitalWrite(B_led, LOW); 
  
  // white light
  digitalWrite(R_led, HIGH);    
  digitalWrite(G_led, HIGH);    
  digitalWrite(B_led, HIGH);    
  delay(T); 
  digitalWrite(R_led, LOW);     
  digitalWrite(G_led, LOW);     
  digitalWrite(B_led, LOW);   
}

