#include <stdlib.h>
#include <string.h>
#include "chip8.h"
#include "display.h"

// global variables
unsigned short opcode = 0; // initialize opcode as global variable set to 0
Chip8 chip8; 

void chip8_initialize(void)
{
	chip8.pc = 0x200; // initialize program counter
	chip8.sp = 0; // initialize stack pointer
	chip8.I = 0; // set special-purpose register to 0
	chip8.delay_timer = 0; // initialize delay timer to 0
	chip8.sound_timer = 0; // initialize sound timer to 0
	memset(chip8.memory, 0, MEMORY_SIZE); // reset memory
	memset(chip8.V, 0, REGISTER_NUM); // reset registers
	memset(chip8.stack, 0, STACK_SIZE); // reset stack
	memset(chip8.gpx, 0, DISPLAY_SIZE); // reset pixels
	memset(chip8.key, 0, KEY_NUM); // reset keyboard

    // load fontset into interpreter address space (from 0x00 to 0x50)
	memcpy(chip8.memory, fonts, 0x50);	

    // load the program into memory (todo)
}


void emulate_cycle(void)
{
    // fetch opcode
    opcode = chip8.memory[chip8.pc] << 8 | chip8.memory[chip8.pc + 1];

    // decode opcode
    
    // execute opcode

    // update timers 
}
