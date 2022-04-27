/*
 * Project: BC7278 hardware spi
 * Function: Display 8 segments of nixie tube and read key values
 * Github: https://github.com/mosiwi
 * Wiki: http://wiki.mosiwi.com
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

#include "BC7278.h"

////////////////////////////////////////////
BC7278::BC7278(){ 
 
}

////////////////////////////////////////////
// Initializes the SPI.
// Note: This function changes the system clock from 16Mhz to 8Mhz.
void BC7278::Start_Init(void){ 
  // define spi port of M328 PRO
  // cs = not use;  mosi = 11;  miso = 12;  sck = 13; 
  SPI.begin();
  
  // Parameter 1: The communication speed is set to 10K (BC7278 allow:35--64K)
  // Parameter 2: Sets the order of the bits shifted out of and into the SPI bus, 
  //              either LSBFIRST (least-significant bit first) or MSBFIRST (most-significant bit first).
  // Parameter 3: The CLK idle state is high; data changes on the first clock edge (falling edge) 
  //              and is sampled on the second clock edge (rising edge).
  // Note: The clock frequency of write data can be 125KHz, and that of read data must be less than 64KHz.  
  SPI.beginTransaction(SPISettings(125000, MSBFIRST, SPI_MODE3));      
  SPI.endTransaction(); 
}

////////////////////////////////////////////
// The host reads slave data.
unsigned int BC7278::BC7278_spi_read_data(byte addr, byte dat){
  unsigned int data = 0;
  data = (addr << 8) + dat;
  
  // disable interrupts
  cli();
  
  // Another option I saw was to prescale the clock to 8 mHz by using this in the setup:
  // clock: 125k/2 = 62.5K
  CLKPR = 0b10000000;
  CLKPR = 0b00000001;
  delayMicroseconds(100);
  // So use one of the other prescale values:
  // 0b0000000 = 0 (16 MHz)
  // 0b0000001 = 2 (8 MHz)
  // 0b0000010 = 4 (4 MHz)
  // 0b0000011 = 8 (2 MHz)
  // 0b0000100 = 16 (1 MHz)
  // ...
  // 0b0001000 = 256 (62 kHz)
   
  data = SPI.transfer16(data);
	
  // Another option I saw was to prescale the clock to 16 mHz by using this in the setup:
  CLKPR = 0b10000000;
  CLKPR = 0b00000000;
  delayMicroseconds(50);
  
  // enable interrupts
  sei(); 
  delay(28);   // Necessary delay time.
  return data;
}

////////////////////////////////////////////
// The host writes data to the slave.
void BC7278::BC7278_spi_write_data(byte addr, byte dat){
  unsigned int data = 0;
  data = (addr << 8) + dat;
  
  //BC7278_spi_start();
  SPI.transfer16(data); 
  //BC7278_spi_stop();
  delay(28);   // Necessary delay time.
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
void BC7278::SetDisplaySeg(byte Seg, byte OnOff){
  if (OnOff != 0 && OnOff != 1)
	  return;
  Seg = ((~OnOff) << 7) + Seg;
  BC7278_spi_write_data(SegAddReg, Seg);
}

////////////////////////////////////////////
// display segment
// Bit: 0--3 
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=0=on, x=1=off)
void BC7278::SetDisplayReg(byte Bit, byte Seg){
  byte addr;
  switch(Bit){
    case 0: addr = DisReg0; break;
    case 1: addr = DisReg1; break;
    case 2: addr = DisReg2; break;
    case 3: addr = DisReg3; break;
    default: return;
  }
  BC7278_spi_write_data(addr, Seg);
}

////////////////////////////////////////////
// set segment flash 
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
void BC7278::SetSegFlash(byte Bit, byte Seg){
  byte BitAddr;
  switch(Bit){
    case 0: BitAddr = SegFlaReg0; break;
    case 1: BitAddr = SegFlaReg1; break;
    case 2: BitAddr = SegFlaReg2; break;
    case 3: BitAddr = SegFlaReg3; break;
    default: return;
  }
  BC7278_spi_write_data(BitAddr, Seg);
}

////////////////////////////////////////////
// set bit flash 
// Bit = D7 D6 D5 D4 D3 D2 D1 D0
//       -- -- -- -- b3 b2 b1 b0 (1=flash, 0=not flash)
void BC7278::SetBitFlash(byte Bit){
  if(Bit > 15)
    return;
  BC7278_spi_write_data(BitFlaReg, Bit);
}

////////////////////////////////////////////
// set flash speed： 0--255
void BC7278::SetFlashSpeed(byte Speed){
  Speed = map(Speed, 0, 255, 255, 0);
  BC7278_spi_write_data(FlaSpeReg, Speed);
}

////////////////////////////////////////////
// Clear the screen or light up all leds.
void BC7278::ClearAll(void){
  BC7278_spi_write_data(GloReg, 0xff);  // turn off all segment
  SetBitFlash(0x00);      // turn off all bit flash. 
  SetSegFlash(0, 0x00);   // turn off bit0 flash 
  SetSegFlash(1, 0x00);   // turn off bit1 flash 
  SetSegFlash(2, 0x00);   // turn off bit2 flash 
  SetSegFlash(3, 0x00);   // turn off bit3 flash 
}

////////////////////////////////////////////
// Clear one bit display.
// Bit = 0--3
void BC7278::ClearBit(byte Bit){
  SetDisplayReg(Bit, 0xff);       // Closes all display segments of bit
  BitFlashData = BitFlashData & (0xfe << Bit); 
  SetBitFlash(BitFlashData);      // turn off one bit flash.  
  SetSegFlash(Bit, 0x00);         // Turn off all segment flashing for the corresponding bit.
}

////////////////////////////////////////////
// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// Bit    : 0, 1, 2, 3
void BC7278::DisplayChar(byte Bit, byte Dat){
  if(Bit > 3 || Dat > 15)
    return;
  byte dat = 0;
  dat = (Bit << 4)| Dat; 
  BC7278_spi_write_data(DecReg, dat);
}

////////////////////////////////////////////
// display: 0.0-999.9
void BC7278::DisplayNumber(float num){
  if(num > 999.9)
    return;
  int dat = num*10;
  SetDisplaySeg(0x17, 1);          // Turn on the decimal point.
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
void BC7278::DisplayNumber(int num){
  if(num > 9999)
    return;
  int dat = num;
  SetDisplaySeg(0x17, 0);          // Turn off the decimal point.
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
//            bit: 0 0 0 0 x x x x
// Read key value: 0 0 0 0 U L R D
// x = 1, There's no button to press. 
// x = 0, There are buttons to press.
byte BC7278::ReadKeyValue(void){ 	
    // 0xff: pseudoinstruction
	// Gets 16 key values
	unsigned int AllKey = BC7278_spi_read_data(0xff, 0xff);
	// Serial.println(AllKey, HEX);
	
	// After processing data, obtain the key values of S12-S15.
    byte keyValue = byte((~AllKey) >> 12);
	// Serial.println(keyValue, HEX);
	
	return keyValue;
}

