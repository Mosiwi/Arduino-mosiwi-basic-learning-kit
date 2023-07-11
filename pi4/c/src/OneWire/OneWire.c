/*
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2023-5-5
 */
 
#include "OneWire.h"

struct{
	unsigned char pin;
    // global search state
    unsigned char ROM_NO[8];
    unsigned char LastDiscrepancy;
    unsigned char LastFamilyDiscrepancy;
    bool LastDeviceFlag;
} ow;

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





