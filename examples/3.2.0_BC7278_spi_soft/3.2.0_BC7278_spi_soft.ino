/*
 * Project: BC7278 spi soft
 * Function: Display 8 segments of nixie tube and read key values
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

// define soft spi port
//byte cs   = 10;       // not use
byte mosi = 11;         // Use the 11-pin M328 PRO
byte miso = 12;         // Use the 12-pin M328 PRO
byte sck  = 13;         // Use the 13-pin M328 PRO

// key value output flag pin. When the level of this pin changes, it indicates that a key is triggered.
const byte ButtonPin = 3;    
byte KeyValue = 0;

// Define the SIP communication clock period.
int T = 100;    //(15.6uS <= Time <= 28mS) = (35Hz <= fclk <= 64kHz) = (16 <= T <= 28000)

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
byte DisReg0 = 0x00;
byte DisReg1 = 0x01;
byte DisReg2 = 0x02;
byte DisReg3 = 0x03;

// Corresponding to 4-bit digital tube, can control the flicker of 8 digital sections of the code tube.  
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// DP  G   F   E   D   C   B   A
byte SegFlaReg0 = 0x10;
byte SegFlaReg1 = 0x11;
byte SegFlaReg2 = 0x12;
byte SegFlaReg3 = 0x13;

// It can control the flicker of each digit.
// default = 0x00, bit: flash = 1, not flash = 0
// D7  D6  D5  D4  D3  D2  D1  D0
// --  --  --  --  b3  b2  b1  b0                
byte BitFlaReg  = 0x18;
byte BitFlashData = 0x00;     // Record bits to display data.

// Can control flashing speed.
// The larger the value, the slower the flicker, and the smaller the value, the faster the flicker.
byte FlaSpeReg  = 0x1A;

// The user can input the value and get the digital display directly.
// D7  D6  D5  D4  D3  D2  D1  D0
// A3  A2  A1  A0  d3  d2  d1  d0
// A3:A0 controls which digit bits are displayed.
// d3:d0 = 0---F
byte DecReg = 0x1B;

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
byte SegAddReg = 0x1C;

// Clear the screen or light up all leds.
byte GloReg = 0x1D;

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
unsigned int BC7278_spi_read_data(byte addr, byte data){
  unsigned int returnData = 0;
  unsigned int sendData = addr;
  sendData = (sendData << 8) + data; 
  for(byte i=0; i<16; i++){
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
void BC7278_spi_write_data(byte addr, byte data){
  unsigned int sendData = addr;
  sendData = (sendData << 8) + data;
  delayMicroseconds(T/2);
  for(byte i=0; i<16; i++){
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
void SetDisplaySeg(byte Seg, byte OnOff){
  if (OnOff != 0 && OnOff != 1)
	  return;
  Seg = ((~OnOff) << 7) + Seg;
  BC7278_spi_write_data(SegAddReg, Seg);
}

////////////////////////////////////////////
// display segment
// Bit: 0--3 
// Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=0=on, x=1=off)
void SetDisplayReg(byte Bit, byte Seg){
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
void SetSegFlash(byte Bit, byte Seg){
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
void SetBitFlash(byte Bit){
  if(Bit > 15)
    return;
  BC7278_spi_write_data(BitFlaReg, Bit);
}

////////////////////////////////////////////
// set flash speed： 0--255
void SetFlashSpeed(byte Speed){
  Speed = map(Speed, 0, 255, 255, 0);
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
void ClearBit(byte Bit){
  SetDisplayReg(Bit, 0xff);       // Closes all display segments of bit
  BitFlashData = BitFlashData & (0xfe << Bit); 
  SetBitFlash(BitFlashData);      // turn off one bit flash.  
  SetSegFlash(Bit, 0x00);         // Turn off all segment flashing for the corresponding bit.
}

////////////////////////////////////////////
// display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
// Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// Bit    : 0, 1, 2, 3
void DisplayChar(byte Bit, byte Dat){
  if(Bit > 3 || Dat > 15)
    return;
  byte dat = 0;
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
byte ReadKeyValue(void){   
    // 0xff: pseudoinstruction
	// Gets 16 key values
	unsigned int AllKey = BC7278_spi_read_data(0xff, 0xff);
	// Serial.println(AllKey, HEX);
	
	// After processing data, obtain the key values of S12-S15.
    byte keyValue = byte((~AllKey) >> 11);
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
void setup() {
  // initialize serial communications.  
  Serial.begin(9600);
  
  // Initialize interrupt pins with a pull resistor.
  pinMode(ButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ButtonPin), GetKeyValue, CHANGE);
  
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
  // SetDisplaySeg(byte Seg, byte OnOff);
  
  // display segment
  // Bit: 0--3 
  // Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
  // SetDisplayReg(byte Bit, byte Seg);
	
  // set segment flash 
  // Seg = xxxxxxxx = DP, G, F, E, D, C, B, A (x=1=on, x=0=off)
  // SetSegFlash(byte Bit, byte Seg);

  // set bit flash 
  // Bit = D7 D6 D5 D4 D3 D2 D1 D0
  //       -- -- -- -- b3 b2 b1 b0 (1=flash, 0=not flash)
  // SetBitFlash(byte Bit);

  // set flash speed： 0--255
  // SetFlashSpeed(byte Speed);
  
  // Clear the screen.
  ClearAll();

  // Clear one bit display.
  // Bit = 0--3
  // ClearBit(byte Bit);
	
  // display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
  // Dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  // Bit    : 0, 1, 2, 3
  // DisplayChar(byte Bit, byte Dat);
}

////////////////////////////////////////////
void loop() {
  // Force a byte variable to be converted to a float variable.
  float Num = float(KeyValue);
  // prints data with ending line break     

  if(Num != 0){
    // function prototype: DisplayNumber(float num);
    // parameter: num = 0.0-999.9
    DisplayNumber(Num);
	// It is recommended that the refresh interval be at least 100ms
	delay(100); 
	
	// U = 16, D = 8, L = 4, R = 2, OK = 1
    Serial.println(KeyValue); 
  }
}



