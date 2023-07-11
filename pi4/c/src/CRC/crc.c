/*
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-5-5
 */

#include "crc.h"

// Dow-CRC using polynomial X^8 + X^5 + X^4 + X^0
// Tiny 2x16 entry CRC table created by Arjen Lentz
// See http://lentz.com.au/blog/calculating-crc-with-a-tiny-32-entry-lookup-table
unsigned char dscrc2x16_table[] = {
	0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
	0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
	0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
	0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
};

// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (Use tiny 2x16 entry CRC table)
unsigned char crc8_t(unsigned char *message, unsigned char len){
	unsigned char crc = 0;

	while (len--) {
		crc = *message++ ^ crc;  // just re-using crc as intermediate
		crc = dscrc2x16_table[crc & 0x0f] ^ dscrc2x16_table[16 + ((crc >> 4) & 0x0f)];
	}
	return crc;
}


//CRC：CRC8/MAXIM
//polynomial：X8+X5+X4+1
//Poly：0011 0001  0x31
//If I put the higher order in the back, it becomes: 1000 1100 0x8c
unsigned char crc8_l(unsigned char *message,unsigned char Num){
  unsigned char i;
  unsigned char byte_;
  unsigned char crc=0xFF;
  for(byte_=0; byte_<Num; byte_++){
    crc^=(message[byte_]);
    for(i=8;i>0;--i){
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
  return crc;
}

unsigned char crc8_r(unsigned char *message, unsigned char len){
	unsigned char crc = 0;
	while (len--) {
		unsigned char inbyte = *message++;
		for (unsigned char i = 8; i; i--) {
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}


unsigned int crc16(unsigned char* input, unsigned int len){
  unsigned char i;
  unsigned int crc = 0x0000;   
  while(len--){
    crc ^= *input++;
    for (i = 0; i < 8; ++i){   
		  // Anti-order CRC16
		  // 1. X16+X15+X2+1 = 11000000000000101 		  
		  // 2. The calculation of reverse XOR is used : 11000000000000101 ---> 10100000000000011
		  // 3. The lowest bit of data is not processed : 10100000000000011 ---> 1010000000000001
		  //    (Move (discard) one bit if the lowest bit of both the data and the polynomial is 1)
		  // 4. 1010000000000001 = 0xA001
      if (crc & 0x01)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }
  }
  return crc;
}

bool check_crc16(unsigned char* input, unsigned int len, unsigned char* inverted_crc){
    unsigned int crc = ~crc16(input, len);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

