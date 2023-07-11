/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  
 * This example uses the Arduino top-level pin function（digitalRead(), digitalWrite()).
 
 Learning target:
  1. Single bus protocol
  2. Class private variable initialization (in constructor)
  3. CRC8 and CRC16
  
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-30
*/
#include<wiringPi.h>
#include <stdbool.h>
#include<string.h>
#include <stdio.h>

//onewire////////////////////////////////////////////////////////////////////////////
struct{
	unsigned char pin;
    // global search state
    unsigned char ROM_NO[8];
    unsigned char LastDiscrepancy;
    unsigned char LastFamilyDiscrepancy;
    bool LastDeviceFlag;
} ow;

//
// You need to use this function to start a search again from the beginning.
// You do not need to do it for the first search, though you could.
//
void ow_reset_search(){
  // reset the search state
  ow.LastDiscrepancy = 0;
  ow.LastDeviceFlag = false;
  ow.LastFamilyDiscrepancy = 0;
  for(int i = 7; ; i--) {
    ow.ROM_NO[i] = 0;
    if ( i == 0) break;
  }
}

void ow_begin(unsigned char pin){
	ow.pin = pin;
	wiringPiSetup();
	pinMode(ow.pin, INPUT);

	ow_reset_search();
}


// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//
unsigned char ow_reset(void){
	unsigned char r;
	unsigned char retries = 125;

	pinMode(ow.pin, INPUT);
	// wait until the wire is high... just in case
	do {
		if (--retries == 0) return 0;
		delayMicroseconds(2);
	} while ( !digitalRead(ow.pin));

	pinMode(ow.pin, OUTPUT);
	digitalWrite(ow.pin, LOW);     	// drive output low
	delayMicroseconds(480);
	pinMode(ow.pin, INPUT);        	// allow it to float
	delayMicroseconds(70);
	r = !digitalRead(ow.pin);

	delayMicroseconds(410);
	return r;
}

//
// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
void ow_write_bit(unsigned char v){
	if (v & 1) {
		pinMode(ow.pin, OUTPUT);
		digitalWrite(ow.pin, LOW);  	// drive output low
		delayMicroseconds(10);
		digitalWrite(ow.pin, HIGH);  	// drive output high
		delayMicroseconds(55);
	} else {
		pinMode(ow.pin, OUTPUT);
		digitalWrite(ow.pin, LOW);  	// drive output low
		delayMicroseconds(65);
		digitalWrite(ow.pin, HIGH); 	// drive output high
		delayMicroseconds(5);
	}
}

//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
unsigned char ow_read_bit(void){	
	unsigned char r;
	pinMode(ow.pin, OUTPUT);
	digitalWrite(ow.pin, LOW);                    	     // drive output low
	delayMicroseconds(3);
	pinMode(ow.pin, INPUT);                             // allow it to float
	delayMicroseconds(10);
	r = digitalRead(ow.pin);

	delayMicroseconds(53);
	return r;
}

//
// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//
void ow_write(unsigned char v, unsigned char power /* = 0 */) {
    unsigned char bitMask;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
		ow_write_bit( (bitMask & v)?1:0);
    }
    if ( !power) {
	pinMode(ow.pin, INPUT);
    }
}

void ow_write_bytes(unsigned char *buf, unsigned int count, bool power /* = 0 */) {
  for (unsigned int i = 0 ; i < count ; i++){
	ow_write(buf[i], 1);
  }
  if (!power) {
    pinMode(ow.pin, INPUT);
  }
}

//
// Read a byte
//
unsigned char ow_read() {
    unsigned char bitMask;
    unsigned char r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
		if ( ow_read_bit()) r |= bitMask;
    }
    return r;
}

void ow_read_bytes(unsigned char *buf, unsigned int count) {
  for (unsigned int i = 0 ; i < count ; i++)
    buf[i] = ow_read();
}

// Do a ROM select
void ow_select(unsigned char rom[8]){
    unsigned char i;

    ow_write(0x55, 1);           // Choose ROM

    for (i = 0; i < 8; i++) ow_write(rom[i], 1);
}

// Do a ROM skip
void ow_skip(){
    ow_write(0xCC, 1);           // Skip ROM
}

void ow_depower(){
	pinMode(ow.pin, INPUT);
}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
void ow_target_search(unsigned char family_code){
   // set the search state to find SearchFamily type devices
   ow.ROM_NO[0] = family_code;
   for (unsigned char i = 1; i < 8; i++)
      ow.ROM_NO[i] = 0;
   ow.LastDiscrepancy = 64;
   ow.LastFamilyDiscrepancy = 0;
   ow.LastDeviceFlag = false;
}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
bool ow_search(unsigned char *newAddr, bool search_mode /* = true */){
   unsigned char id_bit_number;
   unsigned char last_zero, rom_byte_number;
   bool    search_result;
   unsigned char id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = false;

   // if the last call was not the last one
   if (!ow.LastDeviceFlag) {
      // 1-Wire reset
      if (!ow_reset()) {
         // reset the search
         ow.LastDiscrepancy = 0;
         ow.LastDeviceFlag = false;
         ow.LastFamilyDiscrepancy = 0;
         return false;
      }

      // issue the search command
      if (search_mode == true) {
        ow_write(0xF0, 1);   // NORMAL SEARCH
      } else {
        ow_write(0xEC, 1);   // CONDITIONAL SEARCH
      }

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = ow_read_bit();
         cmp_id_bit = ow_read_bit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1)) {
            break;
         } else {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit) {
               search_direction = id_bit;  // bit write value for search
            } else {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < ow.LastDiscrepancy) {
                  search_direction = ((ow.ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               } else {
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == ow.LastDiscrepancy);
               }
               // if 0 was picked then record its position in LastZero
               if (search_direction == 0) {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     ow.LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ow.ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ow.ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            ow_write_bit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0) {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      } while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65)) {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         ow.LastDiscrepancy = last_zero;

         // check for last device
         if (ow.LastDiscrepancy == 0) {
            ow.LastDeviceFlag = true;
         }
         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ow.ROM_NO[0]) {
      ow.LastDiscrepancy = 0;
      ow.LastDeviceFlag = false;
      ow.LastFamilyDiscrepancy = 0;
      search_result = false;
   } else {
      for (int i = 0; i < 8; i++) newAddr[i] = ow.ROM_NO[i];
   }
   return search_result;
}

//crc////////////////////////////////////////////////////////////////////////////
unsigned char crc8_r(unsigned char *message, unsigned char len){
	unsigned char crc = 0;
	while (len--) {
		unsigned char inbyte = *message++;
		for (unsigned char i = 8; i; i--) {
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

unsigned int crc16(unsigned char* input, unsigned int len){
  unsigned char i;
  unsigned int crc = 0x0000;   
  while(len--){
    crc ^= *input++;
    for (i = 0; i < 8; ++i){   
		  // Anti-order CRC16
		  // 1. X16+X15+X2+1 = 11000000000000101 		  
		  // 2. The calculation of reverse XOR is used : 11000000000000101 ---> 10100000000000011
		  // 3. The lowest bit of data is not processed : 10100000000000011 ---> 1010000000000001
		  //    (Move (discard) one bit if the lowest bit of both the data and the polynomial is 1)
		  // 4. 1010000000000001 = 0xA001
      if (crc & 0x01)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }
  }
  return crc;
}

bool check_crc16(unsigned char* input, unsigned int len, unsigned char* inverted_crc){
    unsigned int crc = ~crc16(input, len);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

//gx4231////////////////////////////////////////////////////////////////////////////
#define MAC_SIZE  8
#define FAMILY_CODE  0x2D
#define EEPROM_SIZE  128
#define ROW_SIZE  8

#define PF_MASK  0x07
#define WRITE_MASK  0xAA
#define CMD_SIZE  3
#define CRC_SIZE  2
#define READ_RETRY  2
#define BUFFER_SIZE  ROW_SIZE + CMD_SIZE + CRC_SIZE

#define WRITE_SCRATCHPAD  0x0F
#define READ_SCRATCHPAD  0xAA
#define COPY_SCRATCHPAD  0x55
#define READ_MEMORY  0xF0

unsigned char serialNumber[MAC_SIZE];
bool skiprom;

char eeprom_init(unsigned char pin){
	ow_begin(pin);
	skiprom = true;
	
	return ow_reset();
}

/* family code, 48bit serial number and CRC as returned by OneWire search function */
bool eeprom_begin(unsigned char *_serialNumber){
	skiprom = false;

	ow_target_search(FAMILY_CODE);
	if(!ow_search(_serialNumber, true))
		return false;
	
	if (crc8_r(_serialNumber, 7) != _serialNumber[7])
		return false;
	
	for(unsigned char i = 0; i < MAC_SIZE; i++){
		serialNumber[i] = _serialNumber[i];
	}
	return true;
}

void eeprom_startTransmission(void){
    ow_reset();
    if (skiprom)
		ow_skip();
    else
		ow_select(serialNumber);
}

void eeprom_m_read(unsigned int address, unsigned char *buf, unsigned int len){
	eeprom_startTransmission();

	ow_write(READ_MEMORY, 1);
	ow_write(address & 0x0f, 1);
	ow_write((address >> 4) & 0x0f, 1);

	for (unsigned int i = 0; i < len; i++)
		buf[i] = ow_read();

	ow_depower();
}

unsigned char eeprom_read(unsigned int address){
	unsigned char res = 0xFF;
	eeprom_m_read(address, &res, 1);

	return res;
}

bool eeprom_m_write(unsigned int address, unsigned char *buf, unsigned int count){
  unsigned char error_count = 0;
  unsigned char buffer[BUFFER_SIZE];
  unsigned char crc16[CRC_SIZE];
  bool verify = false;

  //Address has to be aligned on an 8-byte boundary
  if (address >= EEPROM_SIZE || address % 8 != 0)
    return false;

  // Prepare buffer data
  buffer[0] = WRITE_SCRATCHPAD;
  buffer[1] = address & 0x0f;
  buffer[2] = (address >> 4) & 0x0f;
  memcpy(&buffer[CMD_SIZE], buf, count);

  //Write scratchpad with CRC check
  eeprom_startTransmission();
  ow_write_bytes(buffer, CMD_SIZE + count, 1); // Write CMD + LSB Adr + MSB Adr
  ow_read_bytes(crc16, CRC_SIZE); //Read CRC-16

  if (!check_crc16(buffer, CMD_SIZE + count, crc16)){
    verify = true; //CRC not matching, try to read again
  }

  // Read verification
  // Prepare buffer data
  buffer[0] = READ_SCRATCHPAD;
  do {
    //Read scratchpad to compare with the data sent
    eeprom_startTransmission();
    ow_write(buffer[0], 1); // Write CMD
    ow_read_bytes(&buffer[1], CMD_SIZE); //Read TA1, TA2, E/S, scratchpad

    if (buffer[3] != PF_MASK) {
      verify = true;
    }

    if(verify) {
      ow_read_bytes(&buffer[4], count); //Read scratchpad
      ow_read_bytes(crc16, CRC_SIZE); //Read CRC-16

      if (!check_crc16(buffer, 12, crc16)) {
        error_count++; //CRC not matching.
        continue;
      }

      if (address != ((buffer[2] << 8) + buffer[1])) {
        return false; //Address not matching
      }

      if (buffer[3] != PF_MASK) {
        return false; //Invalid transfer or data already copied (wrong value for E/S).
      }

      if (memcmp(&buffer[4], buf, 8) != 0) {
        return false; //Data in the scratchpad is invalid.
      }
    }

    break;
  } while(error_count < READ_RETRY);

  // Prepare buffer data
  buffer[0] = COPY_SCRATCHPAD;

  //Copy scratchpad
  eeprom_startTransmission();
  ow_write_bytes(buffer, CMD_SIZE + 1, 1); //Send authorization code (TA1, TA2, E/S)
  delay(15); // t_PROG = 12.5ms worst case.

  unsigned char res = ow_read();

  if (res != WRITE_MASK) {
    return false;
  }

  return true;
}


bool eeprom_write(unsigned int address, unsigned char *buf, unsigned int count){
  bool ret = eeprom_m_write(address, buf, count);
  ow_depower();
  return ret;
}


//main////////////////////////////////////////////////////////////////////////////
// One Wire pin, change according to your needs. 
const int ONE_WIRE_PIN = 27; 

int main(void){
	// 1.Find if there is a device on the 1-wire bus.
	if(!eeprom_init(ONE_WIRE_PIN)){
		printf("No GX2431 found on the 1-Wire bus. \n");
		return 0;
	}

	// 2.Check serial number CRC
	unsigned char serialNb[8];
	if (!eeprom_begin(serialNb)){
		printf("A GX2431 was found but the serial number is invalid. \n");
		return 0;
	}
	printf("GX2431 found with serial number :");
	for(unsigned char i=0; i<8; i++){
		printf("%d ", serialNb[i]);
	}
	printf("\n");
	
	// 3.Read all memory content
	unsigned char data[128];
	eeprom_m_read(0, data, sizeof(data));
	printf("Memory contents : \n");
	for(unsigned char i=0;i<128;i++){
		if((i%8 == 0) && (i != 0))
		  printf("\n");
		  
		printf(" %d", data[i]);
	}
	printf("\n");

	// 4.Write data to address 0-7.
	unsigned char newData[] = {60,77,111,115,105,119,105,62};
	// The address must be : 0+8*n --> between 0 and 120
	int address = 0;
	// Write a 8-byte row, must write 8 bytes at a time.
	if (eeprom_m_write(address, newData, sizeof(newData))){
		printf("Successfully wrote data to address 0-7: 60 77 111 115 105 119 105 62 \n");
	}
	else{
		printf("Failed wrote data to address 0-7! \n");
	}

	// 5.Reread memory content
	eeprom_m_read(0, data, sizeof(data));
	printf("Reread memory contents : \n");
	for(unsigned char i=0;i<128;i++){
		if((i%8 == 0) && (i != 0))
		  printf("\n");
		  
		printf(" %d", data[i]);
	}
	printf("\n");

	// 6.To convert numbers to characters.
	printf("Our team: ");
	for(unsigned char i=0;i<8;i++){
		printf("%c", data[i]);
	}
	printf("\n");

	return 0;
}




































