#include <SDL3/SDL.h>
#include <stdio.h>

#define WIDTH 256 
#define HEIGHT 256 

static uint8_t gpx[WIDTH * HEIGHT];

static void generate_checkboard_pattern(void)
{
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			gpx[y * WIDTH + x] = (x ^ y) & 1; // checkboard pattern
		}
	}
}

static void generate_white_pattern(void)
{
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			gpx[y * WIDTH + x] = 1;
		}
	}
}

int test_gpx(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Monochrome render", WIDTH, HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_INDEX8, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	generate_white_pattern();

	SDL_Palette *palette = SDL_CreatePalette(2); // create a 2-color palette
	SDL_Color colors[2] = {
		{ 0, 0, 0, 255 }, // index 0: black
		{ 255, 255, 255, 255 }, // index 1: white
	};
	SDL_SetPaletteColors(palette, colors, 0, 2);
	SDL_SetTexturePalatte(texture, palette); // applay palette to texture

	// convert monochrome to RGB pattern
	uint8_t pixels[WIDTH * HEIGHT];
	for (int i=0; i < WIDTH * HEIGHT; i++) {
		pixels[i] = gpx[i] ? 1 : 0;
	}

	// update the texture
	SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));

	int running = 1;
	SDL_Event e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = 0;
			}
		}
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
