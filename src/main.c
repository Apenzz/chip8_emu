#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "chip8.h"
#include "display.h"

#include "tests.h"

int main(void) {

	srand(time(NULL)); // random seed
	// test_gpx();
	run_emulator();

	return 0;
}
