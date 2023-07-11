/*
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-5-5
 */

#ifndef AHT20_h
#define AHT20_h

#include <bcm2835.h>
#include <stdio.h>

#define aht20_reg1  0x1b
#define aht20_reg2  0x1c
#define aht20_reg3  0x1e

#define aht20_ac    0xac
#define aht20_ac_d1 0x33
#define aht20_ac_d2 0x00

#define aht20_addr  0x38

void Aht20_SendAC(void);
void Aht20_read_CTdata(float *ct);
void Aht20_read_CTdata_crc(float *ct);
char Aht20_status(void);
void Aht20_reset_reg(unsigned char reg);
int Aht20_init(void);

#endif
