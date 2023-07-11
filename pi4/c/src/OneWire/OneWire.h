/*
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-5
 */
 
#ifndef OneWire_h
#define OneWire_h
#include <wiringPi.h>
#include <stdbool.h>

// You can exclude certain features from OneWire.  In theory, this
// might save some space.  In practice, the compiler automatically
// removes unused code (technically, the linker, using -fdata-sections
// and -ffunction-sections when compiling, and Wl,--gc-sections
// when linking), so most of these will not result in any code size
// reduction.  Well, unless you try to use the missing features
// and redesign your program to not need them!  ONEWIRE_CRC8_TABLE
// is the exception, because it selects a fast but large algorithm
// or a small but slow algorithm.

void ow_begin(unsigned char pin);

// Perform a 1-Wire reset cycle. Returns 1 if a device responds
// with a presence pulse.  Returns 0 if there is no device or the
// bus is shorted or otherwise held low for more than 250uS
unsigned char ow_reset(void);

// Write a bit. The bus is always left powered at the end, see
// note in write() about that.
void ow_write_bit(unsigned char v);

// Read a bit.
unsigned char ow_read_bit(void);

// Write a byte. If 'power' is one then the wire is held high at
// the end for parasitically powered devices. You are responsible
// for eventually depowering it by calling depower() or doing
// another read or write.
void ow_write(unsigned char v, unsigned char power);

void ow_write_bytes(unsigned char *buf, unsigned int count, bool power);

// Read a byte.
unsigned char ow_read(void);

void ow_read_bytes(unsigned char *buf, unsigned int count);

// Issue a 1-Wire rom select command, you do the reset first.
void ow_select(unsigned char rom[8]);

// Issue a 1-Wire rom skip command, to address all on bus.
void ow_skip(void);

// Stop forcing power onto the bus. You only need to do this if
// you used the 'power' flag to write() or used a write_bit() call
// and aren't about to do another read or write. You would rather
// not leave this powered if you don't have to, just in case
// someone shorts your bus.
void ow_depower(void);

// Clear the search state so that if will start from the beginning again.
void ow_reset_search(void);

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
void ow_target_search(unsigned char family_code);

// Look for the next device. Returns 1 if a new address has been
// returned. A zero might mean that the bus is shorted, there are
// no devices, or you have already retrieved all of them.  It
// might be a good idea to check the CRC to make sure you didn't
// get garbage.  The order is deterministic. You will always get
// the same devices in the same order.
bool ow_search(unsigned char *newAddr, bool search_mode);

#endif // OneWire_h
