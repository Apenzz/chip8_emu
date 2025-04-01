#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "display.h"
#include "chip8.h"
#include "opcodes.h"

// no operation
void nop(void)
{
   // do nothing 
}


// CLS: clear the display
// set gpx frame buffer to 0
void cls()
{
    memset(chip8.gpx, 0, DISPLAY_SIZE);
}

// ret
void ret(void)
{
	chip8.pc = chip8.sp;
	--chip8.sp;
}

// jmp: set the pc to nnn
void jmp(void)
{
	chip8.pc = 0x0fff & opcode;
}

// call addr
void call(void)
{
	chip8.stack[++chip8.sp] = chip8.pc;
	chip8.pc = 0x0fff & opcode;
}

// sek
void sek(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char kk = 0x00ff & opcode; 
	if (chip8.V[x] == kk) {
		chip8.pc += 2;
	}
}

// sne
void sne(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char kk = 0x00ff & opcode;
	if (chip8.V[x] != kk) {
		chip8.pc += 2;
	}
}

// ser 
void ser(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode)	>> 4;
	if (chip8.V[x] == chip8.V[y]) {
		chip8.pc += 2;
	}
}

// ldi - 6xkk
void ldi(void)
{
	unsigned char x = (0x0f00 & opcode)	>> 8;
	unsigned char kk = 0x00ff & opcode;
	chip8.V[x] = kk;
}

// add - 7xkk 
void add(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char kk = 0x00ff & opcode;
	chip8.V[x] = chip8.V[x] + kk;
}

// ldr - 8xy0 
void ldr(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[x] = chip8.V[y];
}

// or - 8xy1
void orr(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[x] = chip8.V[x] | chip8.V[y];
}

// and - 8xy2 
void andr(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[x] = chip8.V[x] & chip8.V[y];
}

// xor - 8xy3 
void xorr(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[x] = chip8.V[x] ^ chip8.V[y];
}

// add carry - 8xy4 
void add_carry(void)
{	
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	unsigned int result = chip8.V[x] + chip8.V[y];
	chip8.V[x] = (unsigned char) result; // keep only the lower 8 bits
	chip8.V[0xf] = (result > 0xff) ? 1 : 0; // set carry flag if overflow
}

// sub - 8xy5
void subr(void)
{
	unsigned char x = (0x0f00 & opcode)	>> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[0xf] = (chip8.V[x] > chip8.V[y]) ? 1: 0; // if V[x] > V[y] set NOT_BORROW bit to 1, otherwise 0
	chip8.V[x] -= chip8.V[y];
}

// shr - 8xy6
void shr(void)
{
	unsigned char x = (0x0f00 & opcode)	>> 8;
	chip8.V[0xf] = 0x1 & chip8.V[x];
	chip8.V[x] >>= 1;
}

// subn - 8xy7 
void subn(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;	
	unsigned char y = (0x00f0 & opcode) >> 4;
	chip8.V[0xf] = (chip8.V[y] > chip8.V[x]) ? 1 : 0;
	chip8.V[x] = chip8.V[y] - chip8.V[x];
}

// shl - 8xyE
void shl(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.V[0xf] = 0x8000 & chip8.V[x];
	chip8.V[x] <<= 1;
}

// sner - 9xy0
void sner(void)
{
	unsigned char x = (0x0f00 & opcode)	>> 8;
	unsigned char y = (0x00f0 & opcode) >> 4;
	if (chip8.V[x] != chip8.V[y]) {
		chip8.pc += 2;
	}
}

// ld - Annn 
void ld(void)
{
	unsigned short addr = 0x0fff & opcode;
	chip8.I = addr;
}

// jpa - Bnnn
void jpa(void)
{
	unsigned short addr = 0x0fff & opcode;
	chip8.pc = addr + chip8.V[0];
}

// rnd - Cxkk
void rnd(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;	
	unsigned char kk = 0x00ff & opcode;
	chip8.V[x] = (rand() % 256) & kk;
}

// drw - Dxyn
void drw(void)
{
	unsigned char x = chip8.V[(0x0f00 & opcode) >> 8] % DISPLAY_WIDTH; // pixel x position
	unsigned char y = chip8.V[(0x00f0 & opcode) >> 4] % DISPLAY_HEIGHT; // pixel y position 
	unsigned char height = 0x000f & opcode; // get number of bytes of sprite (0<n<16)
	unsigned char pixel;

	chip8.V[0xf] = 0; // reset collision flag

	// loops through rows of the sprite
	for (int row=0; row<height; row++) {
		pixel = chip8.memory[chip8.I + row];
		// loops through columns of the sprite 
		for (int col=0; col<8; col++) {
			// if this pixel is set
			if ((pixel & (0x80 >> col)) != 0) {
				// pixel will be erased so set VF to 1
				unsigned int pixel_pos = ((x + col) % DISPLAY_WIDTH) + (((y + row) % DISPLAY_HEIGHT) * DISPLAY_WIDTH);
				if (chip8.gpx[pixel_pos] != 0) {
					chip8.V[0xf] = 1;
				}
				chip8.gpx[pixel_pos] ^= 0xff; // XOR the pixel
			}
		}
	}
}

// skp - Ex9E
void skp(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8; 
	// if key[V[x]] is currently in the down position skip next instruction
	if (chip8.key[chip8.V[x]]) {
		chip8.pc += 2;
	}
}

// skpn - ExA1
void skpn(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	if (!chip8.key[chip8.V[x]]) {
		chip8.pc += 2;
	}
}

// ldt - Fx07
void ltd(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.delay_timer = chip8.V[x];
}

// ldk - Fx0A
// TODO - this works but it's better if it uses a callback function whenever a key is pressed
void ldk(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	bool key_pressed = false; 
	int i = 0;
	// stops all execution until a key is pressed
	while (!key_pressed) {
		for (i=0; i<KEY_NUM; i++) {
			if (chip8.key[i]) {
				key_pressed = true;
			}
		}
	}	
	chip8.V[x] = i;
}

// ltd - Fx15
void ldt(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.delay_timer = chip8.V[x];
}

// lst - Fx18
void lst(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.sound_timer = chip8.V[x];
}

// addi - Fx1E
void addi(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.I += chip8.V[x];
}

// ldf - Fx29
void ldf(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	chip8.I = 5 * chip8.V[x];
}

// ldb - Fx33
void ldb(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	unsigned char number = chip8.V[x];			

	for (int i=0; i<3; i++) {
		chip8.memory[chip8.I + 2 - i] = number % 10;	
		number /= 10;
	}
}

// strs - Fx55
void strs(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	memcpy(&chip8.memory[chip8.I], chip8.V, x+1);
}

// ldrs - Fx65
void ldrs(void)
{
	unsigned char x = (0x0f00 & opcode) >> 8;
	memcpy(chip8.V, &chip8.memory[chip8.I], x + 1);
}
