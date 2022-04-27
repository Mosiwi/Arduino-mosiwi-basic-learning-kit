/*
 This example works with the Mosiwi Basic learning board.
 This example can only be used with only one DS18B20 on the bus and with external power supply.  
 * This example uses the Arduino top-level pin function（digitalRead(), digitalWrite()).
 
 Learning target:
  1. Single bus protocol
  2. Class private variable initialization (in constructor)
  
 Web: http://mosiwi.com/
 Wiki: http://wiki.mosiwi.com/
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
class Temperature{
  public:
    Temperature(uint8_t p): oneWire(OneWire(p)){
		init();
	}
	
	// crc8 check
	uint8_t crc8(const uint8_t *addr, uint8_t len);
	
	// Initial Configuration
	void init(void);
	
	// Set alarm temperature
	void setAlarm(uint8_t tl, uint8_t th){
		alarmTL = tl;   // Low temperature alarm value
		alarmTH = th;   // High temperature alarm value
		init();
		delay(10);
	}
	// r : 9, 10, 11, 12 bit
	void setResolution(uint8_t r){
		switch (r){
			case  9: resolution = 0x1f; break;    // 0.5 C
			case 10: resolution = 0x3f; break;    // 0.25 C
			case 11: resolution = 0x5f; break;    // 0.125 C
			case 12: resolution = 0x7f; break;    // 0.0625 C
		}
		init();
		delay(10);
	}
    // Read the temperature
	float getTempC(void);
	
  private:
    OneWire oneWire;	
	uint8_t alarmTL    = 0x0a;  // Alarm TL, 10 C, complement code
	uint8_t alarmTH    = 0x24;  // Alarm TH, 36 C, complement code
	uint8_t resolution = 0x7f;  // configurations, set resolution 12-bit
};

// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but a little smaller, than the lookup table.
uint8_t Temperature::crc8(const uint8_t *addr, uint8_t len){
	uint8_t crc = 0;
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
				inbyte >>= 1;
		}
	}
	return crc;
}

// Initial Configuration
void Temperature::init(void){
	if(oneWire.reset()){
		oneWire.write_byte(0xcc);          // Skip ROM
		oneWire.write_byte(0x4e);          // Write Scratchpad
		oneWire.write_byte(alarmTL);       // Alarm TL
		oneWire.write_byte(alarmTH);       // Alarm TH
		oneWire.write_byte(resolution);    // configurations
	}
	
	uint8_t buf[9];
	if(oneWire.reset()){
		oneWire.write_byte(0xcc);          // Skip ROM
		oneWire.write_byte(0xbe);          // Read Scratchpad	
		// Read 9 bytes of data
		for (uint8_t i = 0 ; i < 9 ; i++){
			buf[i] = oneWire.read_byte();
		}
	}
	if(crc8(buf, 8) != buf[8]){ return; }
	
	if(oneWire.reset()){
		oneWire.write_byte(0xcc);          // Skip ROM
		oneWire.write_byte(0x48);          // Copy Scratchpad
	}
}

// Initial Configuration
float Temperature::getTempC(void){
	if(oneWire.reset()){
		oneWire.write_byte(0xcc);          // Skip ROM
		oneWire.write_byte(0x44);          // CONVERT T
		while(!oneWire.read_bit());
	}
	
	uint8_t buf[9];
	if(oneWire.reset()){
		oneWire.write_byte(0xcc);          // Skip ROM
		oneWire.write_byte(0xbe);          // Read Scratchpad	
		// Read 9 bytes of data
		for (uint8_t i = 0 ; i < 9 ; i++){
			buf[i] = oneWire.read_byte();
		}
	}
	if(crc8(buf, 8) != buf[8]){ return 0.0; }
	
	// Calculate the temperature
	uint8_t lsb = buf[0];
    uint8_t msb = buf[1];
    switch (resolution){
        case 0x1f: lsb &= 0xF8; break;  // 9 bit
        case 0x3f: lsb &= 0xFC; break;  // 10 bit
        case 0x5f: lsb &= 0xFE; break;  // 11 bit
    }
    uint8_t sign = msb & 0x80;
    int16_t temp = (msb << 8) + lsb;
    if (sign) {
	    // The complement is converted to the original code, 
	    // and the original code is converted to a negative number
        temp = ((temp ^ 0xffff) + 1) * -1; 
    }
    return ((float)temp) * 0.0625;	
}

// Define a temperature object ds18b20, using pin 10
Temperature ds18b20(10);

void setup(){
	Serial.begin(9600);
	// Set alarm temperature
	//ds18b20.setAlarm(10, 36);
	// r : 9, 10, 11, 12 ---> 9:0.5C, 10:0.25C, 11:0.125C, default 12:0.0625C
	//ds18b20.setResolution(12);
}

void loop() {
	Serial.println(ds18b20.getTempC());
	delay(1000);
}
