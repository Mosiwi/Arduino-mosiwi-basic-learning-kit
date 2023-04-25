/*
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

#ifndef AHT20_h
#define AHT20_h

#include <Arduino.h>
#include <Wire.h>

class AHT20{
  public:
    enum element{
      // AHT20 Register address		
	  reg1_ = 0x1b,
	  reg2_ = 0x1c,
	  reg3_ = 0x1e,

	  ac_   = 0xac,
	  ac_d1 = 0x33,
      ac_d2 = 0x00
    };   
    AHT20(int addr = 0x38);          //0x38 is device address
	byte Calc_CRC8(byte *message,byte Num);
	void SendAC(void);
	void Read_CTdata(float *ct);
	void Read_CTdata_crc(float *ct);
	byte Read_Status(void);
	void Reset_REG(int reg);
	void Start_Init(void);

  private:
	int addr_;                       
};

#endif
