/*
 The circuit: No external hardware needed.
 
 Learning target:
	1. Variable: array, pointer
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/

char *p1 = NULL;                  // Define a character pointer
byte *p2 = NULL;                  // Define a byte pointer

char str1[3] ={'1', '2', '3'};    // Define a character array
byte num[3]  ={4, 5, 6};          // Define a byte array

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


// the loop function runs over and over again forever
void loop() {	
  Serial.print("Print str1 by array name: ");   
  Serial.println(str1);                         
  
  Serial.print("Print str1 by array index: ");  
  Serial.print(str1[0]);  
  Serial.print(str1[1]); 
  Serial.println(str1[2]);  
  
  p1 = str1;    // Reassign to pointer.
  Serial.print("Print str1 by pointer name: ");     
  Serial.println(p1);        
 
  Serial.print("Print str1 by pointer index: ");  
  Serial.print(p1[0]);  
  Serial.print(p1[1]); 
  Serial.println(p1[2]); 

  Serial.print("Print num by array index: ");  
  Serial.print(num[0]);  
  Serial.print(num[1]); 
  Serial.println(num[2]);
  
  p2 = num;     // Reassign to pointer.
  Serial.print("Print num by pointer index: ");  
  Serial.print(p2[0]);  
  Serial.print(p2[1]); 
  Serial.println(p2[2]);  
  
  while(1);             
}


