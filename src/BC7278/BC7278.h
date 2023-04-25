/*
 * Project: BC7278 hardware spi
 * Function: Display 8 segments of nixie tube and read key values
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

#ifndef BC7278_h
#define BC7278_h

#include <Arduino.h>
#include <SPI.h>       // the sensor communicates using SPI, so include the library:

class BC7278{
	
//               A                       A                       A                       A
//           ----------              ----------              ----------              ----------
//          |          |            |          |            |          |            |          |
//        F |          | B        F |          | B        F |          | B        F |          | B
//          |    G     |            |    G     |            |    G     |            |    G     |
//           ----------              ----------              ----------              ----------   
//          |          |            |          |            |          |            |          |
//        E |          | C        E |          | C        E |          | C        E |          | C
//          |    D     |            |    D     |            |    D     |            |    D     |
//           ----------  o DP        ----------  o DP        ----------  o DP        ----------  o DP
//           
//              DG0                     DG1                     DG2                     DG3 
  
  public:
    enum element{
	  // They correspond to 4-bit digital tube and can control 8 digital sections of the code tube.
	  // default = 0xff, bit: on = 0, off = 1
	  // D7  D6  D5  D4  D3  D2  D1  D0
	  // DP  G   F   E   D   C   B   A
	  DisReg0 = 0x00,
      DisReg1 = 0x01,
      DisReg2 = 0x02,
      DisReg3 = 0x03,
		
      // Corresponding to 4-bit digital tube, can control the flicker of 8 digital sections of the code tube.  
      // default = 0x00, bit: flash = 1, not flash = 0
      // D7  D6  D5  D4  D3  D2  D1  D0
      // DP  G   F   E   D   C   B   A
      SegFlaReg0 = 0x10,
      SegFlaReg1 = 0x11,
      SegFlaReg2 = 0x12,
      SegFlaReg3 = 0x13,

      // It can control the flicker of each digit.
      // default = 0x00, bit: flash = 1, not flash = 0
      // D7  D6  D5  D4  D3  D2  D1  D0
      // --  --  --  --  b3  b2  b1  b0                
      BitFlaReg  = 0x18,

      // Can control flashing speed.
      // The larger the value, the slower the flicker, and the smaller the value, the faster the flicker.
      FlaSpeReg  = 0x1A,

      // The user can input the value and get the digital display directly.
      // D7  D6  D5  D4  D3  D2  D1  D0
      // A3  A2  A1  A0  d3  d2  d1  d0
      // A3:A0 controls which digit bits are displayed.
      // d3:d0 = 0---F
      DecReg = 0x1B,

      // Realize display control in unit of segment
      //    DP   G   F   E   D   C   B   A
      // 0  07   06  05  04  03  02  01  00
      // 1  0F   0F  0D  0C  0B  0A  09  08   
      // 2  17   16  15  14  13  12  11  10
      // 3  1F   1E  1D  1C  1B  1A  19  18
      // data format:
      // D7  D6  D5  D4  D3  D2  D1  D0
      // Seg A6  A5  A4  A3  A2  A1  A0
      // A5:A0 = data address, Seg = 0 = on, Seg = 1 = off  
      SegAddReg = 0x1C,

      // Clear the screen or light up all leds.
      GloReg = 0x1D
    };  
    BC7278();          
	void Start_Init(void);
	unsigned int  BC7278_spi_read_data(byte addr, byte dat);
	void BC7278_spi_write_data(byte addr, byte dat);
	void SetDisplaySeg(byte Seg, byte OnOff);
	void SetDisplayReg(byte DG, byte Seg);
	void SetSegFlash(byte DG, byte Seg);
	void SetDgFlash(byte DG);
	void SetFlashSpeed(byte Speed);
	void ClearAll(void);
	void ClearBit(byte DG);
	void DisplayChar(byte DG, byte Dat);
	void DisplayNumber(double num);
	void DisplayNumber(float num);
	void DisplayNumber(int num);
	byte ReadKeyValue(void);

  private:
	// Record bits to display data. 
    byte BitFlashData = 0x00;                    
};

#endif
