#include <SDL3/SDL.h>
#include <stdio.h>

#define WIDTH 256 
#define HEIGHT 256 

static uint32_t gpx[WIDTH * HEIGHT];

// NEEDS UPDATING
static void generate_checkboard_pattern(void)
{
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			if ((x+y) % 2 == 0) {
				gpx[y * WIDTH + x] = 0x000000ff; 
			} else {
				gpx[y * WIDTH + x] = 0xffffffff;
			}
		}
	}
}

// fill gpx with fixed RGBA color
static void generate_solid_pattern(uint32_t color)
{
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			gpx[y * WIDTH + x] = color;
		}
	}
}

static void clean_array(void)
{
	for (int i=0; i<(WIDTH*HEIGHT); i++) {
		gpx[i] = 0;
	}
}

int test_gpx(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Monochrome render", WIDTH, HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	
	uint32_t color_red = 0x0000ffff; // solid red
	uint32_t color_green = 0x00ff00ff; // solid green
	uint32_t color_blue = 0xff0000ff; // solid blue 
	clean_array();
	generate_checkboard_pattern();
	// generate_solid_pattern(color_green);

	// update the texture
	SDL_UpdateTexture(texture, NULL, gpx, WIDTH * sizeof(uint32_t));

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
