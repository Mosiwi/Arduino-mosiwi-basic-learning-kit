/*
 The circuit: No external hardware needed.

 Learning target:
	1. Terminal
	2. main function
	3. Variable: char, string
	4. Line break: \n

 Web: http://mosiwi.com/
 Wiki: https://mosiwi-wiki.readthedocs.io
 Designer: jalen
 Date：2022-2-12
*/
#include <wiringPi.h>
#include <stdio.h>

char a  = 'a';
char str[]="The program is running...\n";
char string[]=" ASCII-encoded decimal: ";

int main(void){
	printf(str);

	printf("Print character: %c,", a);        // Prints title with no ending line break     
	printf("%s%d \n", string, a);             // prints data with ending line break 
	return 0;
}


