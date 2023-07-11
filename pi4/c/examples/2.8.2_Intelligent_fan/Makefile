intelligent_fan:intelligent_fan.o
	cc -o intelligent_fan intelligent_fan.c ../../src/AHT20/AHT20.c ../../src/BC7278/BC7278.c ../../src/CRC/crc.c -lwiringPi -lpthread -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o intelligent_fan
