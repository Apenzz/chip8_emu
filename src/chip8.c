#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <SDL3/SDL.h>

#include "chip8.h"
#include "display.h"
#include "opcodes.h"

#define CPU_CLOCK_SPEED 500
#define TIMER_CLOCK_SPEED 60
#define OPCODE_EXECUTION_DELAY (1/CPU_CLOCK_SPEED) 
#define TIMER_DELAY (1/TIMER_CLOCK_SPEED)

// global variables
unsigned short opcode = 0; // initialize opcode as global variable set to 0
Chip8 chip8; 

static void op_0xxx(void);
static void op_1nnn(void);
static void op_2nnn(void);
static void op_3xkk(void);
static void op_4xkk(void);
static void op_5xy0(void);
static void op_6xkk(void);
static void op_7xkk(void);
static void op_8xxx(void);
static void op_9xy0(void);
static void op_annn(void);
static void op_bnnn(void);
static void op_cxkk(void);
static void op_dxyn(void);
static void op_exxx(void);
static void op_fxxx(void);

static void chip8_initialize(void);
static void emulate_cycle(void);

/* function pointer handler */
typedef void (*OpcodeHandler)(void);

/* jump table (first nibble lookup) */
static const OpcodeHandler opcode_table[16] = {
	op_0xxx, op_1nnn, op_2nnn, op_3xkk,
	op_4xkk, op_5xy0, op_6xkk, op_7xkk,
	op_8xxx, op_9xy0, op_annn, op_bnnn,
	op_cxkk, op_dxyn, op_exxx, op_fxxx,
};

static void chip8_initialize(void)
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

    // load the program into memory (TODO)
	
	// initialize graphics libraray
	graphics_initialize();	
}

void run_emulator(void)
{
	uint64_t last_cycle_time = SDL_GetTicks();
	uint64_t last_timer_time = SDL_GetTicks();

	chip8_initialize();	

	bool running = true;
	SDL_Event e;
	
	// main loop
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}	
	}

	// get current time
	uint64_t current_time = SDL_GetTicks();
	// execute one cpu cycle every 200 ms
	if (current_time - last_cycle_time >= CPU_CLOCK_SPEED) {
		emulate_cycle(); // fetch & execute opcode		
		last_cycle_time = current_time;
	}	
	// update delay and sound timers every 16.7 ms
	if (current_time - last_timer_time >= TIMER_CLOCK_SPEED) {
		chip8.delay_timer = (chip8.delay_timer <= 0) ? 0 : chip8.delay_timer--;
		chip8.sound_timer = (chip8.sound_timer <= 0) ? 0 : chip8.sound_timer--;
	}
}

static void emulate_cycle(void)
{
    // fetch opcode
    opcode = chip8.memory[chip8.pc] << 8 | chip8.memory[chip8.pc + 1];
    // decode & execute opcode
	opcode_table[(opcode & 0xf000) >> 12]();	
}

static void op_0xxx(void)
{
	switch (opcode) {
		case 0x00e0:
			cls();
			break;
		case 0x00ee:
			ret();
			break;
		default:
			nop();
	}
}

static void op_1nnn(void)
{
	jmp();	
}

static void op_2nnn(void)
{
	call();
}

static void op_3xkk(void)
{
	sek();
}

static void op_4xkk(void)
{
	sne();
}

static void op_5xy0(void)
{
	ser();
}

static void op_6xkk(void)
{
	ldi();
}

static void op_7xkk(void)
{
	add();
}

static void op_8xxx(void)
{
	unsigned char n = (opcode & 0x000f);
	
	switch (n) {
		case 1:
			orr();	
			break;
		case 2:
			andr();
			break;
		case 3:
			xorr();
			break;
		case 4:
			add_carry();
			break;
		case 5:
			subr();
			break;
		case 6:
			shr();
			break;
		case 7:
			subn();
			break;
		case 0xe:
			shl();
			break;
		default:
			printf("Unknown opcode %#6x\n", opcode);
	}
}

static void op_9xy0(void)
{
	sner();
}

static void op_annn(void)
{
	ld();
}

static void op_bnnn(void)
{
	jpa();
}

static void op_cxkk(void)
{
	rnd();
}

static void op_dxyn(void)
{
	drw();
}

static void op_exxx(void)
{
	switch (opcode & 0x00ff) {
		case 0x9e:
			skp();
			break;
		case 0xa1:
			skpn();
			break;
		default:
			printf("Unknown opcode %#6x\n", opcode);
	}
}

static void op_fxxx(void)
{
	switch (opcode & 0x00ff) {
		case 0x07:
			ltd();
			break;
		case 0x0a:
			ldk();
			break;
		case 0x15:
			ldt();
			break;
		case 0x18:
			lst();
			break;
		case 0x1e:
			addi();
			break;
		case 0x29:
			ldf();
			break;
		case 0x33:
			ldb();
			break;
		case 0x55:
			strs();
			break;
		case 0x65:
			ldrs();
			break;
		default:
			printf("Unknown opcode %#6x\n", opcode);
	}
}




