#ifndef CHIP8_H
#define CHIP8_H

#include "display.h"

#define MEMORY_SIZE 4096
#define REGISTER_NUM 16
#define STACK_SIZE 16
#define KEY_NUM 16

typedef struct 
{
	unsigned char memory[MEMORY_SIZE];
	unsigned char V[REGISTER_NUM];
	unsigned short stack[STACK_SIZE];
	unsigned char gpx[DISPLAY_SIZE];
	unsigned char key[KEY_NUM];
	unsigned short pc;
	unsigned short sp;
	unsigned short I; // Special puropse 16-bit register used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
	unsigned char delay_timer;
	unsigned char sound_timer;
} Chip8;

extern Chip8 chip8;

extern unsigned short opcode;

void run_emulator(void);

#endif
