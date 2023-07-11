/*
 * Project: BC7278 hardware spi
 * Function: Display 8 segments of nixie tube and read key values
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-5-5
 */

#ifndef BC7278_h
#define BC7278_h

#include <bcm2835.h>
#include <stdio.h>

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


// They correspond to 4-bit digital tube and can control 8 digital sections of the code tube.
// default = 0xff, bit: on = 0, off = 1
// D7  D6  D5  D4  D3  D2  D1  D0
// DP  G   F   E   D   C   B   A
#define bc7278_DisReg0  0x00
#define bc7278_DisReg1  0x01
#define bc7278_DisReg2  0x02
#define bc7278_DisReg3  0x03

// Corresponding to 4-bit digital tube, can control the flicker of 8 digital sections of the code tube.  
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// DP  G   F   E   D   C   B   A
#define bc7278_SegFlaReg0  0x10
#define bc7278_SegFlaReg1  0x11
#define bc7278_SegFlaReg2  0x12
#define bc7278_SegFlaReg3  0x13

// It can control the flicker of each digit.
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// --  --  --  --  b3  b2  b1  b0                
#define bc7278_BitFlaReg   0x18

// Can control flashing speed.
// The larger the value, the slower the flicker, and the smaller the value, the faster the flicker.
#define bc7278_FlaSpeReg   0x1A

// The user can input the value and get the digital display directly.
// D7  D6  D5  D4  D3  D2  D1  D0
// A3  A2  A1  A0  d3  d2  d1  d0
// A3:A0 controls which digit bits are displayed.
// d3:d0 = 0---F
#define bc7278_DecReg  0x1B

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
#define bc7278_SegAddReg  0x1C

// Clear the screen or light up all leds.
#define bc7278_GloReg  0x1D 

    
void bc7278_init(void);
unsigned int  bc7278_spi_read_data(unsigned char addr, unsigned char dat);
void bc7278_spi_write_data(unsigned char addr, unsigned char dat);
void SetDisplaySeg(unsigned char Seg, unsigned char OnOff);
void SetDisplayReg(unsigned char DG, unsigned char Seg);
void SetSegFlash(unsigned char DG, unsigned char Seg);
void SetDgFlash(unsigned char DG);
void SetFlashSpeed(unsigned char Speed);
void ClearAll(void);
void ClearBit(unsigned char DG);
void DisplayChar(unsigned char DG, unsigned char Dat);
void DisplayFloat(float num);
void DisplayInt(unsigned int num);
unsigned char ReadKeyValue(void);                   

#endif
