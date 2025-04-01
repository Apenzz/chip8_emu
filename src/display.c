#include <stdbool.h>
#include <SDL3/SDL.h>

#include "display.h"
#include "chip8.h"

/* SDL_Window handler */
static SDL_Window *window = NULL;

/* SDL_Renderer handler */
static SDL_Renderer *renderer = NULL;

/* SDL_Surface handler */  
static SDL_Surface *surface = NULL;

/* black and white colors for our monochrome screen */
static const SDL_Color colors[2] = {
	{ .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff }, // black
	{ .r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff } // white
};

const unsigned char fonts[80] = {
	0xf0, 0x90, 0x90, 0x90, 0xf0,	// 0
	0x20, 0x60, 0x20, 0x20, 0x70,	// 1
	0xf0, 0x10, 0xf0, 0x80, 0xf0,	// 2
	0xf0, 0x10, 0xf0, 0x10, 0xf0, 	// 3
	0x90, 0x90, 0xf0, 0x10, 0x10,	// 4
	0xf0, 0x80, 0xf0, 0x10, 0xf0,	// 5
	0xf0, 0x80, 0xf0, 0x90, 0xf0,	// 6
	0xf0, 0x10, 0x20, 0x40, 0x40,	// 7
	0xf0, 0x90, 0xf0, 0x90, 0xf0, 	// 8
	0xf0, 0x90, 0xf0, 0x10, 0xf0,	// 9
	0xf0, 0x90, 0xf0, 0x90, 0x90,	// A
	0xe0, 0x90, 0xe0, 0x90, 0xe0, 	// B
	0xf0, 0x80, 0x80, 0x80, 0xf0,	// C
	0xe0, 0x90, 0x90, 0x90, 0xe0,	// D
	0xf0, 0x80, 0xf0, 0x80, 0xf0,	// E
	0xf0, 0x80, 0xf0, 0x80, 0x80,	// F
};

static void render_texture(SDL_Texture *texture)
{
	SDL_RenderClear(renderer); // clear the screen with the default color
	SDL_RenderTexture(renderer, texture, NULL, NULL); // render the full texture to the full screen
	SDL_RenderPresent(renderer); // show the rendered image on the screen
}

/* Initialize SDL graphics library */
bool graphics_initialize(void)
{
	bool success = true;
	// initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Could not initialize SDL library! Error: %s\n", SDL_GetError());
	success = false;
	} else {
		if (!(window = SDL_CreateWindow("Chip-8 Emulator", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0))) {
			SDL_Log("Window could not be created! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			// create renderer
			if (!(renderer = SDL_CreateRenderer(window, NULL))) {
				SDL_Log("Renderer could not be created! Error: %s\n", SDL_GetError());
				success = false;
			}
			// set renderer default draw color for RenderClear func
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			// create surface
			surface = SDL_CreateSurfaceFrom(DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_PIXELFORMAT_INDEX8, chip8.gpx, DISPLAY_WIDTH * sizeof(char)); 
			// create palette of 255 entries (because of SDL_PIXELFORMAT_INDEX8) all initialized to white
			SDL_Palette *palette = SDL_CreateSurfacePalette(surface);
			// set first two indexes of palette to black and white (we will only use the first two indexes; not very space efficient I know)		
			if (!SDL_SetPaletteColors(palette, colors, 0, 2)) {
				SDL_Log("Could not set palette colors! Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}
	return success;
}

/* main function for updating screen */
bool draw_frame()
{
	bool success = true;
	// create a texture from surface for exploiting hardware accelaration
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	// render the texture to the screen
	render_texture(texture);
	SDL_DestroyTexture(texture);
	return success;
}

/* Cleanup and quit SDL library */
void graphics_quit(void)
{
	SDL_DestroySurface(surface);
	// Free renderer
	SDL_DestroyRenderer(renderer);
	// Free window
	SDL_DestroyWindow(window);
	// Quit SDL subsystems
	SDL_Quit();
}


