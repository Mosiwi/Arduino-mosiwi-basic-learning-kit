range_measurement:range_measurement.o
	gcc -Wall -o range_measurement range_measurement.c ../../src/BC7278/BC7278.c -lwiringPi -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o range_measurement
