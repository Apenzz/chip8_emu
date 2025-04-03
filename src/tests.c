#include <SDL3/SDL.h>
#include <stdio.h>

#include "chip8.h"
#include "display.h"

static void generate_checkboard_pattern(void)
{
	for (int y=0; y<DISPLAY_HEIGHT; y++) {
		for (int x=0; x<DISPLAY_WIDTH; x++) {
			if ((x+y) % 2 == 0) {
				chip8.gpx[y * DISPLAY_WIDTH + x] = 0x00; // solid black
			} else {
				chip8.gpx[y * DISPLAY_WIDTH + x] = 0xff; // solid white
			}
		}
	}
}

// fill gpx with fixed RGBA color
static void generate_solid_pattern(uint8_t color)
{
	for (int y=0; y<DISPLAY_HEIGHT; y++) {
		for (int x=0; x<DISPLAY_WIDTH; x++) {
			chip8.gpx[y * DISPLAY_WIDTH + x] = color;
		}
	}
}

static void clean_array(void)
{
	for (int i=0; i<(DISPLAY_WIDTH*DISPLAY_HEIGHT); i++) {
		chip8.gpx[i] = 0;
	}
}

int test_gpx(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Monochrome render", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	
	uint8_t black = 0x00;
	uint8_t white = 0xff;
	clean_array();
	generate_checkboard_pattern();
	// generate_solid_pattern(white);
	
	// convert monochrome pixels to RGB representation	
	uint32_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
	
	

	// update the texture
	SDL_UpdateTexture(texture, NULL, chip8.gpx, DISPLAY_WIDTH * sizeof(uint32_t));

	int running = 1;
	SDL_Event e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	// cleanup
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
