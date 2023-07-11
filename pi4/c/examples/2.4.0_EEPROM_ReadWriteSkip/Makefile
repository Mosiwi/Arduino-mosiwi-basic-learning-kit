eeprom_ReadWriteSkip:eeprom_ReadWriteSkip.o
	gcc -Wall -o eeprom_ReadWriteSkip eeprom_ReadWriteSkip.c ../../src/GX2431/GX2431.c ../../src/OneWire/OneWire.c ../../src/CRC/crc.c -lwiringPi

.PHONY:clean
clean:
	rm -rf *.o eeprom_ReadWriteSkip
