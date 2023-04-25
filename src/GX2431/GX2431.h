/*
 This example works with the Mosiwi Basic learning board.
 
 function: Used to drive the GX2431 EEPROM.
 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-2-21
 */

#ifndef _GX2431_H
#define _GX2431_H

#include "../OneWire/OneWire.h"

class GX2431 {
public:
  const static uint8_t ONE_WIRE_MAC_SIZE = 8;
  const static uint8_t ONE_WIRE_FAMILY_CODE = 0x2D;
  const static uint8_t GX2431_EEPROM_SIZE = 128;
  const static uint8_t GX2431_ROW_SIZE = 8;

  GX2431(OneWire &ow); // OneWire class

  /* family code, 48bit serial number and CRC as returned by OneWire search function */
  void begin(uint8_t serialNumber[ONE_WIRE_MAC_SIZE]); 

  /* Single byte read */
  uint8_t read(uint16_t address);

  /* Multiple byte read. */
  void read(uint16_t address, uint8_t *buf, uint16_t len);

  /* Multiple byte write.
    Please note : address must be a multiple of 8. Write up to 8 bytes
    Return true if operation was successful.
    The OneWire bus should be de-powered after calling this function.
  */
  bool write(uint16_t address, const uint8_t *buf, uint16_t count, bool verify = false);

private:
  const static uint8_t GX2431_PF_MASK = 0x07;
  const static uint8_t GX2431_WRITE_MASK = 0xAA;
  const static uint8_t GX2431_CMD_SIZE = 3;
  const static uint8_t GX2431_CRC_SIZE = 2;
  const static uint8_t GX2431_READ_RETRY = 2;
  const static uint16_t GX2431_BUFFER_SIZE = GX2431_ROW_SIZE + GX2431_CMD_SIZE + GX2431_CRC_SIZE;

  OneWire &_ow;
  uint8_t _serialNumber[ONE_WIRE_MAC_SIZE];
  bool _skiprom;

  enum Commands {
    WRITE_SCRATCHPAD = 0x0F,
    READ_SCRATCHPAD = 0xAA,
    COPY_SCRATCHPAD = 0x55,
    READ_MEMORY = 0xF0
  };

  bool _write(uint16_t address, const uint8_t *buf, uint16_t count, bool verify);

  inline void _startTransmission(){
    _ow.reset();
    if (_skiprom)
    _ow.skip();
    else
    _ow.select(_serialNumber);
  }
};

#endif // _GX2431_H
