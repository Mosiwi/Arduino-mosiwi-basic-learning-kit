/*
 This example works with the Mosiwi Basic learning board.

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-5
 */

#ifndef stc_h
#define stc_h
#include <stdio.h>
#include <bcm2835.h>

int stc_fd = 0;
unsigned int stc_adr = 0x2d;

char stc_init(void){
    if (!bcm2835_init()){
      printf("bcm2835_init failed. Are you running as root??\n");
      return -1;
    }

    	// I2C begin if specified
	if (!bcm2835_i2c_begin()){
		printf("bcm2835_i2c_begin failed. Are you running as root??\n");
		return -1;
	}
	// I2C clock: 100k
	bcm2835_i2c_set_baudrate(100000);
	return 0;
}

unsigned int readi2c(unsigned char chan){
	unsigned int res = 0;
	char reg[1] = {chan};
	char buf[3] = {0, 0};

	bcm2835_i2c_setSlaveAddress(stc_adr);
	//bcm2835_i2c_write(reg, 1);
	//bcm2835_i2c_read(buf, 2);
	//bcm2835_i2c_read_register_rs(reg, buf, 2);
	bcm2835_i2c_write_read_rs(reg, 1, buf, 2);
	res = (buf[0]*256) + buf[1];
	//printf("%d \n", buf[0]);
	return res;
}

unsigned int stc_mic(void){
	return readi2c(0x00);
}

unsigned int stc_sr(void){
	return readi2c(0x02);
}

unsigned int stc_ir(void){
	return readi2c(0x04);
}

#endif
