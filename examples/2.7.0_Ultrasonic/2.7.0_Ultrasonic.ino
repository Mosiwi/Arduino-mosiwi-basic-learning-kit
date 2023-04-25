/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. ultrasonic
	2. pluse
	3. function: pulseIn()
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-4-3
 */
#define Echo 6   //S1
#define Trig 5   //S2
unsigned long rxTime;
float distance;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  // Example Initialize the I/O port
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trig, LOW); 
}

// put your main code here, to run repeatedly:
void loop() {
  // The Trig pin sends a 10uS high level trigger pulse signal.
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW); 
  
  // The Echo pin captures a feedback high level pulse signal(in microseconds). 
  rxTime = pulseIn(Echo, HIGH);  
  // Convert microseconds to seconds: rxTime/1000000.0  --->  1s = 1000000us
  // The speed of ultrasound: 340m/s = 34000cm/s, 
  // Because the data measured by ultrasound has two distances back and forth, the data has to be divided by 2.
  // Convert the pulse signal to a distance in centimeters.
  distance = ((rxTime/1000000.0) * 34000.0) / 2;
  
  // Print distance of serial port monitor.
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}
