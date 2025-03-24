#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "chip8.h"
#include "display.h"

int main(void) {
	srand(time(NULL)); // random seed
	initialize();	

	SDL_Window *window;
	bool done = false;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"Chip-8 Emulator",
		DISPLAY_WIDTH,
		DISPLAY_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	// check if window was successfully craeted
	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
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

	SDL_Quit();
	return 0;
}
