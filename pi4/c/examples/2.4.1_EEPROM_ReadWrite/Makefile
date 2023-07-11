eeprom_ReadWrite:eeprom_ReadWrite.o
	gcc -Wall -o eeprom_ReadWrite eeprom_ReadWrite.c ../../src/GX2431/GX2431.c ../../src/OneWire/OneWire.c ../../src/CRC/crc.c -lwiringPi

.PHONY:clean
clean:
	rm -rf *.o eeprom_ReadWrite
