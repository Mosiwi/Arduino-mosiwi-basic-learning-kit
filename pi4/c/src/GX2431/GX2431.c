/*
 This example works with the Mosiwi Basic learning board.
 
 function: Used to drive the GX2431 EEPROM.
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
 */

#include "GX2431.h"
#include "../OneWire/OneWire.h"
#include "../CRC/crc.h"

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






