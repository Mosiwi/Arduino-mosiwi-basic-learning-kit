/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  
 * This example uses the Arduino top-level pin function（digitalRead(), digitalWrite()).
 
 Learning target:
  1. Single bus protocol
  2. Class private variable initialization (in constructor)
  3. CRC
  
 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-30
*/

// Single bus protocol
class OneWire{
  public:
    OneWire(uint8_t p): pin(p){}

    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    uint8_t reset(void);
	
    void write_bit(uint8_t v);    // Write a bit. 
    uint8_t read_bit(void);       // Read a bit.
	   
    void write_byte(uint8_t v);   // Write a byte.
    uint8_t read_byte(void);      // Read a byte.	
  private:
    uint8_t pin;	
};

// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
uint8_t OneWire::reset(void){
	uint8_t r;
	uint8_t retries = 125;
	noInterrupts();                                  // Disable the interrupt
	pinMode(pin, INPUT);
	// wait until the wire is high... just in case
	do {
		if (--retries == 0) return 0;
		delayMicroseconds(2);
	} while ( !digitalRead(pin));

	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);                          // drive output low
	delayMicroseconds(480);
	pinMode(pin, INPUT);                             // allow it to float
	delayMicroseconds(70);
	r = !digitalRead(pin);                           // DS18B0 exists by pulling low level accordingly
	interrupts();                                    // Enable the interrupt
	delayMicroseconds(410);
	return r;
}

// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
void OneWire::write_bit(uint8_t v){
  noInterrupts();                                 // Disable the interrupt
	if (v & 1) {   // write bit 1	
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);                    	// drive output low
		delayMicroseconds(10);
		digitalWrite(pin, HIGH);    	            // drive output high
		delayMicroseconds(55);
	} else {      // write bit 0
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);                    	// drive output low
		delayMicroseconds(65);
		digitalWrite(pin, HIGH);    	            // drive output high
		delayMicroseconds(5);
	}
	interrupts();                                   // Enable the interrupt
}

// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
uint8_t OneWire::read_bit(void){
	uint8_t r;
	noInterrupts();                                  // Disable the interrupt
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);                    	     // drive output low
	delayMicroseconds(3);
	pinMode(pin, INPUT);                             // allow it to float
	delayMicroseconds(10);
	r = digitalRead(pin);
	interrupts();                                    // Enable the interrupt
	delayMicroseconds(53);
	return r;
}

// Write a byte. 
void OneWire::write_byte(uint8_t v) {
  uint8_t bitMask;
  for (bitMask = 0x01; bitMask; bitMask <<= 1) {
    write_bit( (bitMask & v)?1:0);
  }
	pinMode(pin, INPUT);   // Release the bus
}

// Read a byte
uint8_t OneWire::read_byte() {
  uint8_t bitMask;
  uint8_t r = 0;
  for (bitMask = 0x01; bitMask; bitMask <<= 1) {
    if (read_bit()) r |= bitMask;
  }
  return r;
}

// Define a temperature class
class EEPROM{
  public:
    EEPROM(uint8_t p)
	: _ow(OneWire(p)){   //Initialize the private variable "_ow" with "OneWire(p)"
	}
	
	// Check for the presence of a device on 1wire.
	uint8_t find_device(void){
		return _ow.reset();
	}

	// Read a byte of data from eeprom.
	uint8_t read(uint16_t address);

	// Write a byte of data to the eeprom.
	// Write a 8-byte row, must write 8 bytes at a time.
	bool write(uint16_t address, uint8_t *buf);
	
	// crc16 check
	uint16_t crc16(const uint8_t* input, uint16_t len);
	bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc);
	
  private:
    OneWire _ow;	
};

// Read a byte of data from eeprom.
uint8_t EEPROM::read(uint16_t address){
	_ow.reset();
	_ow.write_byte(0xCC);     // Skip ROM
	_ow.write_byte(0xF0);     // read memory
	_ow.write_byte(lowByte(address));
	_ow.write_byte(highByte(address));	
	return _ow.read_byte();
}

// Write a 8-byte row, must write 8 bytes at a time.
bool EEPROM::write(uint16_t address, uint8_t *buf){
	bool verify = false;
	uint8_t crc16[2];    // store value of crc
	uint8_t buffer[12];  // data)+command = 12bytes

	// 1.write scratchpad --> Write data to the scratchpad
	buffer[0] = 0x0F;                   // store commands --> write scratchpad
	buffer[1] = lowByte(address);       // address
	buffer[2] = highByte(address);
	memcpy(&buffer[3], buf, 8);         // 8 bytes data
 
	_ow.reset();                        // start
	_ow.write_byte(0xCC);               // CMD ---> Skip ROM	
	_ow.write_byte(buffer[0]);          // CMD ---> write scratchpad   
	_ow.write_byte(buffer[1]);          // address
	_ow.write_byte(buffer[2]);	
	
	for (uint8_t i = 3 ; i < 11; i++)  // write 8 bytes data to eeprom
		_ow.write_byte(buffer[i]);
	
	crc16[0] = _ow.read_byte();         // Read CRC-16
	crc16[1] = _ow.read_byte();
	if (!check_crc16(buffer, 11, crc16))
		verify = true; //CRC not matching, try to read again
    
  // 2.read scratchpad --> Read data from the scratchpad
  buffer[0] = 0xAA;                   // store commands --> read scratchpad
	_ow.reset();                        // start
	_ow.write_byte(0xCC);               // CMD ---> Skip ROM	
  _ow.write_byte(buffer[0]);          // CMD ---> read scratchpad
    
	for(uint8_t i=1;i<4;i++)            //Read TA1(Low address), TA2(High address) and E/S
		buffer[i] = _ow.read_byte();

  if (buffer[3] != 0x07)              // E/S must be equal to 0x07(8 bytes data)
    return false;

  if(verify){
    for (uint8_t i = 4 ; i < 12 ; i++) //Read the data of scratchpad(8 bytes)
	    buffer[i] = _ow.read_byte();
  
    crc16[0] = _ow.read_byte();        // Read CRC-16
    crc16[1] = _ow.read_byte();
    if (!check_crc16(buffer, 12, crc16))  // CRC not matching.
      return false;              
  }

  // 3.Copy scratchpad --> Write the data in the scratchpad to memory
  buffer[0] = 0x55;          // CMD --> Copy scratchpad
  _ow.reset();               // start
  _ow.write_byte(0xCC);      // CMD ---> Skip ROM
  for(uint8_t i=0;i<4;i++)   //Send authorization code (TA1, TA2, E/S)
    _ow.write_byte(buffer[i]);
  
  delay(15);                 // t_PROG = 12.5ms worst case.
  uint8_t res = _ow.read_byte();  // Read copy status, 0xAA = success
  if (res != 0xAA) {         
    return false;
  }
  return true;
}

bool EEPROM::check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc){
    uint16_t crc = ~crc16(input, len);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

uint16_t EEPROM::crc16(const uint8_t* input, uint16_t len){
  uint8_t i;
  uint16_t crc = 0x0000;   
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


// Define a temperature object eeprom, using pin 7
EEPROM eeprom(7);

void setup(){
	Serial.begin(9600);

  // 1.Find if there is a device on the 1-wire bus.
  if(!eeprom.find_device()){
    Serial.println("No device found on the 1-Wire bus.");
    return;
  }
  
  // 2.Read data from address 0-7.
  Serial.print("Read data from address 0-7: ");
  for(uint8_t i=0;i<8;i++){
	  Serial.print(eeprom.read(i));
	  Serial.print(" ");
  }
  Serial.println("");
  
  // 3.Write data to address 0-7.
  byte wData[] = {60,77,111,115,105,119,105,62};
  //The address must be : 8*n --> between 0 and 120
  uint16_t address = 0;
  // Write a 8-byte row, must write 8 bytes at a time.
  if(eeprom.write(address, wData)){
    Serial.println("Successfully wrote data to address 0-7: 60 77 111 115 105 119 105 62");
  }else{
    Serial.println("Failed wrote data to address 0-7!");
  }

  // 4.Reread data from address 0-7.
  Serial.print("Reread data from address 0-7: ");
  for(uint8_t i=0;i<8;i++){
	  Serial.print(eeprom.read(i));
	  Serial.print(" ");
  }
  Serial.println("");
  
  // 5.To convert numbers to characters.
  Serial.print("Our team: ");
  for(uint8_t i=0;i<8;i++){
	  Serial.print(char(eeprom.read(i)));
  }
  Serial.println("");
}

void loop() {
  // Nothing to do
}
