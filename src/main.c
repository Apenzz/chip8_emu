#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "chip8.h"
#include "display.h"

int main(void) {
	int exit_code = 0;

	srand(time(NULL)); // random seed
	chip8_initialize();	
	if (!graphics_initialize()) {
		SDL_Log("Unable to load SDL\n");
		exit_code = 1;
	} else {
		// load media	
	}
	
	while (!done) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}
		}
		// do game logic, present a frame, etc.
	}

	SDL_DestroyWindow(window);
	
	// quit SDL
	graphics_quit();

	return 0;
}
