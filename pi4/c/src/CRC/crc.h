/*
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-5
 */
 
#ifndef crc_h
#define crc_h

#include <stdbool.h>

// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (Use tiny 2x16 entry CRC table)
unsigned char crc8_t(unsigned char *message, unsigned char len);


//CRC：CRC8/MAXIM
//polynomial：X8+X5+X4+1
//Poly：0011 0001  0x31
//If I put the higher order in the back, it becomes: 1000 1100 0x8c
unsigned char crc8_l(unsigned char *message,unsigned char Num);

unsigned char crc8_r(unsigned char *message, unsigned char len);

unsigned int crc16(unsigned char* input, unsigned int len);

bool check_crc16(unsigned char* input, unsigned int len, unsigned char* inverted_crc);

#endif