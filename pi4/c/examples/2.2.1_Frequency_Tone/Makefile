frequency_tone:frequency_tone.o
	gcc -Wall -o frequency_tone frequency_tone.c ../../src/BC7278/BC7278.c -lwiringPi  -lpthread -lbcm2835

.PHONY:clean
clean:
	rm -rf *.o frequency_tone
