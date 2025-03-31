#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
/*
	Collection of constants and functions related to the screen and drawing functionalities.

 	This implementation of the Chip-8 language uses a 64x32 pixel monochorme display with this format:
	
		------------------
		|(0,0)    (63, 0)|
		|                |
		|(0, 31) (63, 31)|
		------------------
	
	Chip-8 graphics are drawn to the screen using sprites.
	A sprite is a group of bytes which are a binary representation of an image.
	Sprites may be up to 15 bytes, for a possible maximum size of 8x15 bits.

	Programs may refer to sprites representing the hexadecimal digits 0 through F. These are 5 bytes long thus 8x5 pixels.

	The data should be stored in the memory space 0x000 to 0x1ff.
*/


/* constants definitions */

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT 

/* Font sprits for hexadecimal digits 0 through F */
extern const unsigned char fonts[80];

/* Initialize SDL graphics library */
bool graphics_initialize(void);

/* Cleanup and quit SDL library */
void graphics_quit(void);
#endif
