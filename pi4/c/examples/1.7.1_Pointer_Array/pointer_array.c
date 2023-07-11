/*
 The circuit: No external hardware needed.

 Learning target:
	1. Variable: array, pointer

 Web: http://mosiwi.com
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <stdio.h>
#include <wiringPi.h>

char *p1 = NULL;                  // Define a character pointer
char *p2 = NULL;                  // Define a byte pointer

char str1[3] ={'1', '2', '3'};    // Define a character array
char num[3]  ={4, 5, 6};          // Define a character array

int main(void) {
  printf("Print str1 by array name: %s \n", str1);

  printf("Print str1 by array index: %c%c%c \n", str1[0], str1[1], str1[2]);

  p1 = str1;    // Reassign to pointer.
  printf("Print str1 by pointer name: %s \n", p1);

  printf("Print str1 by pointer index: %c%c%c \n", p1[0], p1[1], p1[2]);

  printf("Print num by array index: %d%d%d \n", num[0], num[1], num[2]);

  p2 = num;     // Reassign to pointer.
  printf("Print num by pointer index: %d%d%d \n", p2[0], p2[1], p2[2]);

  return 0;
}


