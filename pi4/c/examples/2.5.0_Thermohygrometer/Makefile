thermohygrometer:thermohygrometer.o
	gcc -Wall -o thermohygrometer thermohygrometer.c ../../src/AHT20/AHT20.c ../../src/BC7278/BC7278.c ../../src/CRC/crc.c -lwiringPi -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o thermohygrometer
