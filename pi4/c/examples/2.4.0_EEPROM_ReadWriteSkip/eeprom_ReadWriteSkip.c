/*
 This example works with the Mosiwi Basic learning board.
 
 Learning target:
	1. EEPROM (Electrically Erasable Programmable Read-Only Memory)
	2. 1-Wire protocol
	3. variable: word
	3. class
	
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
*/
#include "../../src/GX2431/GX2431.h"

// One Wire pin, change according to your needs. 
const int ONE_WIRE_PIN = 27; 

int main(void){
	// 1.Find if there is a device on the 1-wire bus.
	if(!eeprom_init(ONE_WIRE_PIN)){
		printf("No GX2431 found on the 1-Wire bus. \n");
		return 0;
	}

	// 2.Read all memory content
	unsigned char data[128];
	eeprom_m_read(0, data, sizeof(data));
	printf("Memory contents : \n");
	for(unsigned char i=0;i<128;i++){
		if((i%8 == 0) && (i != 0))
		  printf("\n");
		  
		printf(" %d", data[i]);
	}
	printf("\n");

	// 3.Write data to address 0-7.
	unsigned char newData[] = {60,77,111,115,105,119,105,62};
	//The address must be : 8*n --> between 0 and 120
	int address = 0;
	// Write a 8-byte row, must write 8 bytes at a time.
	if (eeprom_m_write(address, newData, sizeof(newData))){
		printf("Successfully wrote data to address 0-7: 60 77 111 115 105 119 105 62 \n");
	}
	else{
		printf("Failed wrote data to address 0-7! \n");
	}

	// 4.Reread memory content
	eeprom_m_read(0, data, sizeof(data));
	printf("Reread memory contents : \n");
	for(unsigned char i=0;i<128;i++){
		if((i%8 == 0) && (i != 0))
		  printf("\n");
		  
		printf(" %d", data[i]);
	}
	printf("\n");

	// 5.To convert numbers to characters.
	printf("Our team: ");
	for(unsigned char i=0;i<8;i++){
		printf("%c", data[i]);
	}
	printf("\n");

	return 0;
}



