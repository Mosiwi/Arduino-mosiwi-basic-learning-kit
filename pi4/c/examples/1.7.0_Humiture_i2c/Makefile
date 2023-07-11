humiture_i2c:humiture_i2c.o
	gcc -Wall -o humiture_i2c humiture_i2c.c ../../src/AHT20/AHT20.c ../../src/CRC/crc.c -lwiringPi -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o humiture_i2c
