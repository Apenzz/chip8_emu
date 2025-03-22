#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "chip8.h"

int main(void) {
	srand(time(NULL)); // random seed
	initialize();	
	printf("This works for now!\n");
	return 0;
}
