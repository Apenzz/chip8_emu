#include <string.h>
#include <stdlib.h>
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
	unsigned char n = 0x000f & opcode; // get number of bytes of sprite (0<n<16)
				
}












