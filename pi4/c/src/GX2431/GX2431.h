/*
 This example works with the Mosiwi Basic learning board.
 
 function: Used to drive the GX2431 EEPROM.
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
 */

#ifndef _GX2431_H
#define _GX2431_H

#include <stdio.h>
#include<string.h>
#include <stdbool.h>

#define MAC_SIZE  8
#define FAMILY_CODE  0x2D
#define EEPROM_SIZE  128
#define ROW_SIZE  8

#define PF_MASK  0x07
#define WRITE_MASK  0xAA
#define CMD_SIZE  3
#define CRC_SIZE  2
#define READ_RETRY  2
#define BUFFER_SIZE  ROW_SIZE + CMD_SIZE + CRC_SIZE

#define WRITE_SCRATCHPAD  0x0F
#define READ_SCRATCHPAD  0xAA
#define COPY_SCRATCHPAD  0x55
#define READ_MEMORY  0xF0


char eeprom_init(unsigned char pin);

/* family code, 48bit serial number and CRC as returned by OneWire search function */
bool eeprom_begin(unsigned char *_serialNumber); 

void eeprom_startTransmission(void);

/* Multiple byte read. */
void eeprom_m_read(unsigned int address, unsigned char *buf, unsigned int len);

/* Single byte read */
unsigned char eeprom_read(unsigned int address);

bool eeprom_m_write(unsigned int address, unsigned char *buf, unsigned int count);

/* Multiple byte write.
Please note : address must be a multiple of 8. Write up to 8 bytes
Return true if operation was successful.
The OneWire bus should be de-powered after calling this function.
*/
bool eeprom_write(unsigned int address, unsigned char *buf, unsigned int count);

#endif // _GX2431_H







