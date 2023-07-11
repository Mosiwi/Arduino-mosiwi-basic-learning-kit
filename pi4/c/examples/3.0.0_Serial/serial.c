/*
 The circuit: No external hardware needed.

 Learning target:
	1. Serial
	2. Variable: char, string

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12

 Enable serial port terminal instructions:
 1. sudo raspi-config
	Interface Options --> Serial Port --> <No>(disable controller) --> <Yes>
 2. sudo reboot
 3. ls -l /dev/serial*
	lrwxrwxrwx 1 root root 5 May 11 03:22 /dev/serial0 -> ttyS0
	lrwxrwxrwx 1 root root 7 May 11 03:22 /dev/serial1 -> ttyAMA0

 raspberry pi 4: https://www.raspberrypi.com/documentation/computers/configuration.html#primary-uart
	ttyAMA0: bluetooth
	ttyS0(We use this): controller(default) --> Wiringpi: GPIO 15 (transmit) and 16 (receive) 

*/
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main(void){
	int fd = 0;
	int snum = 0;
	wiringPiSetup();
	fd = serialOpen("/dev/ttyS0",9600);

	char string[] = "Hello Mosiwi!\n";
	serialPrintf(fd, string);
	// Read 14 bytes from the serial port cache and complete output of the received data.
	do{
		// Gets the number of bytes available in the serial port receive cache.
		snum = serialDataAvail(fd);
	}while(snum < 14);

	while(snum--){
		// Print the received data to the terminal.
		printf("%c", serialGetchar(fd));
	}

	serialPutchar(fd,'W');
	serialPutchar(fd,'e');
	serialPutchar(fd,'l');
	serialPutchar(fd,'c');
	serialPutchar(fd,'o');
	serialPutchar(fd,'m');
	serialPutchar(fd,'e');
	serialPutchar(fd,'!');
	serialPutchar(fd,'\n');
	do{
		snum = serialDataAvail(fd);
	}while(snum < 9);

	while(snum--){
		printf("%c", serialGetchar(fd));
	}
	return 0;
}



