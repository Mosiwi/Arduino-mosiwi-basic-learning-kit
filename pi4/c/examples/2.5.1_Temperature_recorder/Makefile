temperature_recorder:temperature_recorder.o
	gcc -Wall -o temperature_recorder temperature_recorder.c ../../src/AHT20/AHT20.c ../../src/BC7278/BC7278.c ../../src/GX2431/GX2431.c ../../src/OneWire/OneWire.c ../../src/CRC/crc.c -lwiringPi -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o temperature_recorder
