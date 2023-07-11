/*
 * Project: BC7278 hardware spi
 * Function: Display 8 segments of nixie tube and read key values
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-5-5
 */

#include "BC7278.h"

// Record bits to display data. 
unsigned char BitFlashData = 0x00;

////////////////////////////////////////////
// Initializes the SPI.
void bc7278_init(void){ 
    if (!bcm2835_init()){
		printf("bcm2835_init failed. Are you running as root??\n");
		return;
    }
    if (!bcm2835_spi_begin()){
		printf("bcm2835_spi_begin failed. Are you running as root??\n");
		return;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                  
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // 6.1035156kHz 
	//bcm2835_spi_set_speed_hz(6000);                               // 6kHz	
    bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);                  // The default
    //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
	bcm2835_delay(28);   // Necessary delay, standard is 28ms.
}

////////////////////////////////////////////
// The host reads slave data.
unsigned int bc7278_spi_read_data(unsigned char addr, unsigned char dat){
  unsigned int data = 0;
  
  data = bcm2835_spi_transfer(addr) << 8;
  data = bcm2835_spi_transfer(dat) + data;
  
  bcm2835_delay(10);   // Necessary delay, standard is 28ms.
  return data;
}

////////////////////////////////////////////
// The host writes data to the slave.
void bc7278_spi_write_data(unsigned char addr, unsigned char dat){
  //unsigned int data = 0;
  //data = (addr << 8) + dat;
  //bcm2835_spi_write(data);
  
  bcm2835_spi_transfer(addr);
  bcm2835_spi_transfer(dat);
  bcm2835_delay(10);   // Necessary delay, standard is 28ms.
}

////////////////////////////////////////////
// display segment
// Seg   DP    G    F    E    D    C    B    A
// Bit 
//  0    7h    6h   5h   4h   3h   2h   1h   0h
//  1    fh    eh   dh   ch   bh   ah   9h   8h
//  2    17h   16h  15h  14h  13h  12h  11h  10h
//  3    1fh   1eh  1dh  1ch  1bh  1ah  19h  18h
//  
// OnOff = 1 = on, OnOff = 0 = off
void SetDisplaySeg(unsigned char Seg, unsigned char OnOff){
  if (OnOff != 0 && OnOff != 1)
	  return;
  Seg = ((~OnOff) << 7) + Seg;
  bc7278_spi_write_data(bc7278_SegAddReg, Seg);
}

////////////////////////////////////////////
// display segment
// DG: 0--3 
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=0=on, x=1=off)
void SetDisplayReg(unsigned char DG, unsigned char Seg){
  unsigned char addr;
  switch(DG){
    case 0: addr = bc7278_DisReg0; break;
    case 1: addr = bc7278_DisReg1; break;
    case 2: addr = bc7278_DisReg2; break;
    case 3: addr = bc7278_DisReg3; break;
    default: return;
  }
  bc7278_spi_write_data(addr, Seg);
}

////////////////////////////////////////////
// set segment flash 
// DG: 0--3
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
void SetSegFlash(unsigned char DG, unsigned char Seg){
  unsigned char BitAddr;
  switch(DG){
    case 0: BitAddr = bc7278_SegFlaReg0; break;
    case 1: BitAddr = bc7278_SegFlaReg1; break;
    case 2: BitAddr = bc7278_SegFlaReg2; break;
    case 3: BitAddr = bc7278_SegFlaReg3; break;
    default: return;
  }
  bc7278_spi_write_data(BitAddr, Seg);
}

////////////////////////////////////////////
  // set DGx flash 
  // GD = B7  B6  B5  B4  B3  B2  B1  B0 (binary)
  //      --  --  --  --  DG3 DG2 DG1 DG0 (1=flash, 0=not flash)
void SetDgFlash(unsigned char DG){
  if(DG > 15)
    return;
  bc7278_spi_write_data(bc7278_BitFlaReg, DG);
}

////////////////////////////////////////////
// set flash speed： 0--255
void SetFlashSpeed(unsigned char Speed){
  Speed = 255 - Speed;
  bc7278_spi_write_data(bc7278_FlaSpeReg, Speed);
}

////////////////////////////////////////////
// Clear the screen.
void ClearAll(void){
  bc7278_spi_write_data(bc7278_GloReg, 0xff);  // turn off all segment
  SetDgFlash(0x00);      // turn off all bit flash. 
  SetSegFlash(0, 0x00);   // turn off bit0 flash 
  SetSegFlash(1, 0x00);   // turn off bit1 flash 
  SetSegFlash(2, 0x00);   // turn off bit2 flash 
  SetSegFlash(3, 0x00);   // turn off bit3 flash 
}

////////////////////////////////////////////
// Clear one bit display.
// DG = 0--3
void ClearBit(unsigned char DG){
  SetDisplayReg(DG, 0xff);              // Closes all display segments of bit
  BitFlashData = BitFlashData & (0xfe << DG); 
  SetDgFlash(BitFlashData);      // turn off one bit flash.  
  SetSegFlash(DG, 0x00);                // Turn off all segment flashing for the corresponding bit.
}

////////////////////////////////////////////
// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// DG    : 0, 1, 2, 3
void DisplayChar(unsigned char DG, unsigned char Dat){
  if(DG > 3 || Dat > 15)
    return;
  unsigned char dat = 0;
  dat = (DG << 4)| Dat; 
  bc7278_spi_write_data(bc7278_DecReg, dat);
}

////////////////////////////////////////////
// display: 0.0-999.9
void DisplayFloat(float num){
  if(num > 999.9)
    return;
  unsigned int dat = num*10;
  SetDisplaySeg(0x17, 1);          // Turn on the decimal pounsigned  int.
  if(dat/1000 != 0){
    DisplayChar(0, dat/1000);  
    DisplayChar(1, dat%1000/100);
    DisplayChar(2, dat%100/10);
    DisplayChar(3, dat%10);
    return;
  }
  if(dat%1000/100 != 0){
    ClearBit(0); 
    DisplayChar(1, dat%1000/100); 
    DisplayChar(2, dat%100/10);
    DisplayChar(3, dat%10);
    return;
  }
  ClearBit(0); 
  ClearBit(1);
  DisplayChar(2, dat%100/10);
  DisplayChar(3, dat%10);
}

////////////////////////////////////////////
// display: 0-9999
void DisplayInt(unsigned int num){
  if(num > 9999)
    return;
  unsigned  int dat = num;
  SetDisplaySeg(0x17, 0);          // Turn off the decimal pounsigned  int.
  if(dat/1000 != 0){
    DisplayChar(0, dat/1000);  
    DisplayChar(1, dat%1000/100);
    DisplayChar(2, dat%100/10);
    DisplayChar(3, dat%10);
    return;
  }
  if(dat%1000/100 != 0){
    ClearBit(0); 
    DisplayChar(1, dat%1000/100); 
    DisplayChar(2, dat%100/10);
    DisplayChar(3, dat%10);
    return;
  }
  if(dat%1000/10 != 0){
    ClearBit(0); 
	ClearBit(1); 
    DisplayChar(2, dat%100/10);
    DisplayChar(3, dat%10);
    return;
  }
  ClearBit(0); 
  ClearBit(1);
  ClearBit(2);
  DisplayChar(3, dat%10);
}

////////////////////////////////////////////
//            bit: 0 0 0 x x x x x
// Read key value: 0 0 0 U D L R OK
// x = 0, There's no button to press. 
// x = 1, There are buttons to press.
unsigned char ReadKeyValue(void){ 	
    // 0xff: pseudoinstruction
	// Gets 16 key values
	unsigned int AllKey = bc7278_spi_read_data(0xff, 0xff);
	AllKey = AllKey^0xffff;
	
	// After processing data, obtain the key values of S12-S15.
    unsigned char keyValue = (unsigned char)(AllKey >> 11);	
	return keyValue;
}

