/*
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-5-5
 */

#include "AHT20.h"
#include "../CRC/crc.h"

////////////////////////////////////////////
void Aht20_SendAC(void){
	char buf[3] = {aht20_ac, aht20_ac_d1, aht20_ac_d2};
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_write(buf, 3);
}

///////////////////////////////////////////
//No CRC check, read AHT20 temperature and humidity data directly
void Aht20_read_CTdata(float *ct){
	char  Byte[6] = {0,0,0,0,0,0};
	unsigned long  RetuData = 0;
	unsigned char  cnt = 0;  

	Aht20_SendAC();                // Send the AC command to AHT20
	bcm2835_delay(80);
  
	//Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
	while(((Aht20_status() & 0x80) == 0x80)){
		bcm2835_delay(5);
		if(cnt++ >= 100){
			break;
		}
	}
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_read(Byte, 6);
  
	// Byte[0]  //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.  
	// Byte[1]  //humidity 
	// Byte[2]  //humidity 
	// Byte[3]  //humidity / temperature
	// Byte[4]  //temperature 
	// Byte[5]  //temperature 
	RetuData = (RetuData|Byte[1])<<8;
	RetuData = (RetuData|Byte[2])<<8;
	RetuData = (RetuData|Byte[3]);
	RetuData =RetuData >>4;
	ct[0] = (float)RetuData*100/1024/1024;           //humidity 
	RetuData = 0;
	RetuData = (RetuData|Byte[3])<<8;
	RetuData = (RetuData|Byte[4])<<8;
	RetuData = (RetuData|Byte[5]);
	RetuData = RetuData&0xfffff;
	ct[1] = (float)RetuData*200/1024/1024-50;        //temperature 
}

///////////////////////////////////////////
//CRC check, read AHT20 temperature and humidity data directly
void Aht20_read_CTdata_crc(float *ct){
	char  Byte[7] = {0,0,0,0,0,0,0};
	unsigned long   RetuData = 0;
	unsigned char  cnt = 0;  
	//unsigned char  CTDATA[6]={0};        // For CRC pass arrays
  
	Aht20_SendAC();                      // Send the AC command to AHT20
	bcm2835_delay(80);

	//Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
	while(((Aht20_status() & 0x80) == 0x80)){
		bcm2835_delay(5);
		if(cnt++ >= 100){
			break;
		}
	}
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_read(Byte, 7);
  
	// Byte[0]  //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.  
	// Byte[1]  //humidity 
	// Byte[2]  //humidity 
	// Byte[3]  //humidity / temperature
	// Byte[4]  //temperature 
	// Byte[5]  //temperature 
	// Byte[6]  //CRC data
	if(crc8_l((unsigned char *)Byte,6)==Byte[6]){
		RetuData = (RetuData|Byte[1])<<8;
		RetuData = (RetuData|Byte[2])<<8;
		RetuData = (RetuData|Byte[3]);
		RetuData =RetuData >>4;
		ct[0] = (float)RetuData*100/1024/1024;           //humidity 
		RetuData = 0;
		RetuData = (RetuData|Byte[3])<<8;
		RetuData = (RetuData|Byte[4])<<8;
		RetuData = (RetuData|Byte[5]);
		RetuData = RetuData&0xfffff;
		ct[1] = (float)RetuData*200/1024/1024-50;        //temperature 
	} 
	else{
		printf("crc err! %d\n", (int)Byte[6]);
		ct[0]=0x00;
		ct[1]=0x00;
	}
}

////////////////////////////////////////////
char Aht20_status(void){
	char stat[1];
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_read (stat, 1);
	return stat[0];
}

////////////////////////////////////////////
void Aht20_reset_reg(unsigned char reg){
	char Byte[3] = {0,0,0};
	char buf[3] = {0, 0, 0};

	buf[0] = reg;
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_write(buf, 3);

	bcm2835_delay(5);
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_read(Byte, 3);

	bcm2835_delay(10); 
	Byte[0] = 0xb0|reg;
	bcm2835_i2c_setSlaveAddress(aht20_addr);
	bcm2835_i2c_write(Byte, 3);
}

////////////////////////////////////////////
int Aht20_init(void){
    if (!bcm2835_init()){
      printf("bcm2835_init failed. Are you running as root??\n");
      return -1;
    }

    // I2C begin if specified    
	if (!bcm2835_i2c_begin()){
		printf("bcm2835_i2c_begin failed. Are you running as root??\n");
		return -1;
	}
	// I2C clock: 125k
	bcm2835_i2c_set_baudrate(125000);
	
	if((Aht20_status() & 0x18) != 0x18){
		Aht20_reset_reg(aht20_reg1); 
		Aht20_reset_reg(aht20_reg2);
		Aht20_reset_reg(aht20_reg3);
		bcm2835_delay(10);
	}
	return 0;
}

