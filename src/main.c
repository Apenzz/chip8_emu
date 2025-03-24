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
	
	SDL_Delay(2000);

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
