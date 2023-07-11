/*
 The circuit: No external hardware needed.

 Learning target:
	1. Arithmetic operator: +, -, *, /, %

 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <wiringPi.h>
#include <stdio.h>

char num = 0;

int main(void) {
	num = 10+1;
	printf("10+1 = %d \n", num);

	num = 10-1;
	printf("10-1 = %d \n", num);

	num = 2*5;
	printf("2*5 = %d \n", num);

	num = 5/2;
	printf("5/2 = %d \n", num);

	num = 5%2;
	printf("5%%2 = %d \n", num);

	printf("\n");
	return 0;
}


