/*
 This example works with the Mosiwi Basic learning board.
 
 function: Used to drive the GX2431 EEPROM.
 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
 */

#include "GX2431.h"

GX2431::GX2431(OneWire &ow)
: _ow(ow)  //Initialize the private variable "_ow" with "&ow"
{
  _skiprom = true;
}

void GX2431::begin(uint8_t serialNumber[ONE_WIRE_MAC_SIZE]){
  memcpy(_serialNumber, serialNumber, ONE_WIRE_MAC_SIZE);
  _skiprom = false;
}

uint8_t GX2431::read(uint16_t address){
  uint8_t res = 0xFF;
  read(address, &res, 1);

  return res;
}

void GX2431::read(uint16_t address, uint8_t *buf, uint16_t len){
  _startTransmission();

  _ow.write(READ_MEMORY, 1);
  _ow.write(lowByte(address), 1);
  _ow.write(highByte(address), 1);

  for (int i = 0; i < len; i++)
  buf[i] = _ow.read();

  _ow.depower();
}

bool GX2431::write(uint16_t address, const uint8_t *buf, uint16_t count, bool verify /* = 0 */){
  bool ret = _write(address, buf, count, verify);
  _ow.depower();
  return ret;
}

bool GX2431::_write(uint16_t address, const uint8_t *buf, uint16_t count, bool verify){
  uint8_t error_count = 0;
  uint8_t buffer[GX2431_BUFFER_SIZE];
  uint8_t crc16[GX2431_CRC_SIZE];

  //Address has to be aligned on an 8-byte boundary
  if (address >= GX2431_EEPROM_SIZE || address % 8 != 0)
    return false;

  // Prepare buffer data
  buffer[0] = WRITE_SCRATCHPAD;
  buffer[1] = lowByte(address);
  buffer[2] = highByte(address);
  memcpy(&buffer[GX2431_CMD_SIZE], buf, count);

  //Write scratchpad with CRC check
  _startTransmission();
  _ow.write_bytes(buffer, GX2431_CMD_SIZE + count, 1); // Write CMD + LSB Adr + MSB Adr
  _ow.read_bytes(crc16, GX2431_CRC_SIZE); //Read CRC-16

  if (!_ow.check_crc16(buffer, GX2431_CMD_SIZE + count, crc16)){
    verify = true; //CRC not matching, try to read again
  }

  // Read verification
  // Prepare buffer data
  buffer[0] = READ_SCRATCHPAD;
  do {
    //Read scratchpad to compare with the data sent
    _startTransmission();
    _ow.write(buffer[0], 1); // Write CMD
    _ow.read_bytes(&buffer[1], GX2431_CMD_SIZE); //Read TA1, TA2, E/S, scratchpad

    if (buffer[3] != GX2431_PF_MASK) {
      verify = true;
    }

    if(verify) {
      _ow.read_bytes(&buffer[4], count); //Read scratchpad
      _ow.read_bytes(crc16, GX2431_CRC_SIZE); //Read CRC-16

      if (!_ow.check_crc16(buffer, 12, crc16)) {
        error_count++; //CRC not matching.
        continue;
      }

      if (address != ((buffer[2] << 8) + buffer[1])) {
        return false; //Address not matching
      }

      if (buffer[3] != GX2431_PF_MASK) {
        return false; //Invalid transfer or data already copied (wrong value for E/S).
      }

      if (memcmp(&buffer[4], buf, 8) != 0) {
        return false; //Data in the scratchpad is invalid.
      }
    }

    break;
  } while(error_count < GX2431_READ_RETRY);

  // Prepare buffer data
  buffer[0] = COPY_SCRATCHPAD;

  //Copy scratchpad
  _startTransmission();
  _ow.write_bytes(buffer, GX2431_CMD_SIZE + 1, 1); //Send authorization code (TA1, TA2, E/S)
  delay(15); // t_PROG = 12.5ms worst case.

  uint8_t res = _ow.read();

  if (res != GX2431_WRITE_MASK) {
    return false;
  }

  return true;
}
