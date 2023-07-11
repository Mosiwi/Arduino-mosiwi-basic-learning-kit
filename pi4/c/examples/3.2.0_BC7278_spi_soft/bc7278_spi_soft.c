/*
 * Project: BC7278 spi soft
 * Function: Display 8 segments of nixie tube and read key values
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */
#include <stdio.h>
#include <wiringPi.h>

// define soft spi port
//#define cs    10       // not use
#define mosi  12     
#define miso  13        
#define sck   14        

// key value output flag pin. When the level of this pin changes, it indicates that a key is triggered.
const char ButtonPin = 21;    
unsigned char KeyValue = 0;

// Define the SIP communication clock period.
int T = 100;    //(15.6uS <= Time <= 28mS) = (35Hz <= fclk <= 64kHz) = (16 <= T <= 28000)

//               A
//           ----------
//          |          |
//        F |          | B
//          |    G     |
//           ----------   
//          |          |
//        E |          | C
//          |    D     |
//           ----------  o DP

// They correspond to 4-bit digital tube and can control 8 digital sections of the code tube.
// default = 0xff, bit: on = 0, off = 1
// D7  D6  D5  D4  D3  D2  D1  D0
// DP  G   F   E   D   C   B   A
#define DisReg0  0x00
#define DisReg1  0x01
#define DisReg2  0x02
#define DisReg3  0x03

// Corresponding to 4-bit digital tube, can control the flicker of 8 digital sections of the code tube.  
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// DP  G   F   E   D   C   B   A
#define SegFlaReg0  0x10
#define SegFlaReg1  0x11
#define SegFlaReg2  0x12
#define SegFlaReg3  0x13

// It can control the flicker of each digit.
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// --  --  --  --  b3  b2  b1  b0                
#define BitFlaReg   0x18
unsigned char BitFlashData = 0x00;     // Record bits to display data.

// Can control flashing speed.
// The larger the value, the slower the flicker, and the smaller the value, the faster the flicker.
#define FlaSpeReg   0x1A

// The user can input the value and get the digital display directly.
// D7  D6  D5  D4  D3  D2  D1  D0
// A3  A2  A1  A0  d3  d2  d1  d0
// A3:A0 controls which digit bits are displayed.
// d3:d0 = 0---F
#define DecReg  0x1B

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
#define SegAddReg  0x1C

// Clear the screen or light up all leds.
#define GloReg  0x1D

////////////////////////////////////////////
// Initializes the SPI communication pin mode.
void Start_Init(void){
  pinMode(mosi, OUTPUT);
  pinMode(miso, INPUT);
  pinMode(sck, OUTPUT);
  digitalWrite(sck, HIGH);
}

////////////////////////////////////////////
// The host reads slave data.
unsigned int BC7278_spi_read_data(unsigned char addr, unsigned char data){
  unsigned int returnData = 0;
  unsigned int sendData = addr;
  sendData = (sendData << 8) + data; 
  for(unsigned char i=0; i<16; i++){
    if((sendData & 0x8000) == 0x8000){
      digitalWrite(mosi, HIGH);
	}
    else{ 
      digitalWrite(mosi, LOW);
    }
    digitalWrite(sck, LOW);
    delayMicroseconds(T/2);
    digitalWrite(sck, HIGH);
    delayMicroseconds(T/2);
	
	returnData = returnData<<1;
    if(digitalRead(miso) == HIGH){
	  returnData = returnData | 0x0001;
	}
	sendData = sendData << 1;	
  } 
  delay(30);    // Necessary delay time.
  return returnData;
}

////////////////////////////////////////////
// The host writes data to the slave.
void BC7278_spi_write_data(unsigned char addr, unsigned char data){
  unsigned int sendData = addr;
  sendData = (sendData << 8) + data;
  delayMicroseconds(T/2);
  for(unsigned char i=0; i<16; i++){
    if((sendData & 0x8000) == 0x8000){
      digitalWrite(mosi, HIGH);
    }
    else{ 
      digitalWrite(mosi, LOW);
    }
    digitalWrite(sck, LOW);
    delayMicroseconds(T/2);
    digitalWrite(sck, HIGH);
    delayMicroseconds(T/2);
	sendData = sendData << 1;
  }
  delay(30);     // Necessary delay time.
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
  BC7278_spi_write_data(SegAddReg, Seg);
}

////////////////////////////////////////////
// display segment
// Bit: 0--3 
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=0=on, x=1=off)
void SetDisplayReg(unsigned char Bit, unsigned char Seg){
  unsigned char addr;
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
void SetSegFlash(unsigned char Bit, unsigned char Seg){
  unsigned char BitAddr;
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
void SetBitFlash(unsigned char Bit){
	if(Bit > 15)
		return;
	BC7278_spi_write_data(BitFlaReg, Bit);
}

////////////////////////////////////////////
// set flash speed： 0--255
void SetFlashSpeed(unsigned char Speed){
	Speed = 255 - Speed;
	BC7278_spi_write_data(FlaSpeReg, Speed);
}

////////////////////////////////////////////
// Clear the screen or light up all leds.
void ClearAll(void){
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
void ClearBit(unsigned char Bit){
	SetDisplayReg(Bit, 0xff);       // Closes all display segments of bit
	BitFlashData = BitFlashData & (0xfe << Bit); 
	SetBitFlash(BitFlashData);      // turn off one bit flash.  
	SetSegFlash(Bit, 0x00);         // Turn off all segment flashing for the corresponding bit.
}

////////////////////////////////////////////
// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// Bit    : 0, 1, 2, 3
void DisplayChar(unsigned char Bit, unsigned char Dat){
	if(Bit > 3 || Dat > 15)
		return;
	unsigned char dat = 0;
	dat = (Bit << 4)| Dat; 
	BC7278_spi_write_data(DecReg, dat);
}

////////////////////////////////////////////
// display: 0.0-999.9
void DisplayNumber(float num){
	if(num > 999.9)
		return;
	int dat = num*10;
	SetDisplaySeg(0x17, 1);          // Display the decimal point.
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
//            bit: 0 0 0 x x x x x
// Read key value: 0 0 0 U D L R OK
// x = 1, There's no button to press. 
// x = 0, There are buttons to press.
unsigned char ReadKeyValue(void){   
    // 0xff: pseudoinstruction
	// Gets 16 key values
	unsigned int AllKey = BC7278_spi_read_data(0xff, 0xff);
	AllKey = AllKey^0xffff;
	
	// After processing data, obtain the key values of S12-S15.
    unsigned char keyValue = (unsigned char)(AllKey >> 11);
	// Serial.println(keyValue, HEX);
	return keyValue;
}

////////////////////////////////////////////
// Interrupt function with no return value
void GetKeyValue(void){
  // return byte variable. Read key value: U = 16, D = 8, L = 4, R = 2, OK = 1
  KeyValue = ReadKeyValue();
}

////////////////////////////////////////////
int main(void){
	wiringPiSetup();
	// Use the external interrupt function to get the key value.
	wiringPiISR(ButtonPin, INT_EDGE_FALLING, GetKeyValue);

	// Initialize SPI communications.
	// Note: This function changes the system clock from 16Mhz to 8Mhz.
	Start_Init();

	// display segment
	// Seg   DP    G    F    E    D    C    B    A
	// Bit 
	//  0    7h    6h   5h   4h   3h   2h   1h   0h
	//  1    fh    eh   dh   ch   bh   ah   9h   8h
	//  2    17h   16h  15h  14h  13h  12h  11h  10h
	//  3    1fh   1eh  1dh  1ch  1bh  1ah  19h  18h
	//  
	// OnOff = 1 = on, OnOff = 0 = off  
	// SetDisplaySeg(unsigned char Seg, unsigned char OnOff);

	// display segment
	// Bit: 0--3 
	// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
	// SetDisplayReg(unsigned char Bit, unsigned char Seg);

	// set segment flash 
	// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
	// SetSegFlash(unsigned char Bit, unsigned char Seg);

	// set bit flash 
	// Bit = D7 D6 D5 D4 D3 D2 D1 D0
	//       -- -- -- -- b3 b2 b1 b0 (1=flash, 0=not flash)
	// SetBitFlash(unsigned char Bit);

	// set flash speed： 0--255
	// SetFlashSpeed(unsigned char Speed);

	// Clear the screen.
	ClearAll();

	// Clear one bit display.
	// Bit = 0--3
	// ClearBit(unsigned char Bit);

	// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
	// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	// Bit    : 0, 1, 2, 3
	// DisplayChar(unsigned char Bit, unsigned char Dat);

	while(1){
		// Force a byte variable to be converted to a float variable.
		float Num = (float)KeyValue;
		// prints data with ending line break     

		if(Num != 0){
			// function prototype: DisplayNumber(float num);
			// parameter: num = 0.0-999.9
			DisplayNumber(Num);
			// It is recommended that the refresh interval be at least 100ms
			delay(100); 

			// U = 16, D = 8, L = 4, R = 2, OK = 1
			printf("Key value: %d \n", KeyValue); 
			KeyValue = 0.0;
		}
	}
}



