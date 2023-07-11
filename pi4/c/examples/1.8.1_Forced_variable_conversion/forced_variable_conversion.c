/*
 This example works with the Mosiwi Basic learning board.

 Learning target:
	1. Forced variable conversion
	2. Loop statement: while(1)

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-3-20
 */

#include <stdio.h>
#include <wiringPi.h>

int main(void){
  printf("Start ...\n");
  delay(1000);
  printf("(char)97.3 = %c \n", (char)97.3);
  printf("(char)97 = %c \n", (char)97);
  printf("(int)97.3 = %d \n", (int)97.3);
  printf("(int)'a' = %d \n", (int)'a');
  printf("float(97) = %f \n", (float)97);
  printf("float('a') = %f \n", (float)'a');

  while(1);
}
