#ifndef OPCODES_H
#define OPCODES_H

/*
	no operation
	this exists for retro-compatibility but modern interpreters do not use this opcode
	hexcode: 0nnn
*/
void nop(void);

/*
	CLS 
	clear the display 
	hexcode: 00E0
*/
void cls(void);

/*
 	RET
	The interpreter sets the program counter to the address at the top of the stack, then substract 1 from the stack pointer
	hexcode: 00EE
*/
void ret(void);

/*
 	JMP addr	
	The interpreter sets the program counter to nnn
	hexcode: 1nnn
*/
void jmp(void);

/*
	CALL addr	
	The interpreter increments the stack pointer, then puts the current program counter on the top of the stack. The pc is then set to nnn.
	hexcode: 2nnn
*/
void call(void);

/*
	SE Vx, kk
	The interpreter compares Vx to kk, and if they are equal, increments the program counter by 2.
	hexcode: 3xkk 
*/
void sek(void);

/*
	SNE Vx, kk
	The interpreter compares Vx to kk, and if they are not equal, increments the program counter by 2.
	hexcode: 4xkk
*/
void sne(void);

/*
	SE Vx, Vy
	The interpreter compares Vx to Vy, and if they are equal, increments the program counter by 2.
	hexcode: 5xy0
*/
void ser(void);

/*
	LD Vx, kk
	The interpreter puts the value kk into register Vx.
	hexcode: 6xkk
*/
void ldi(void);

/*
	ADD Vx, kk
	Adds the value kk to Vx, then stores the result in Vx
	hexcode: 7xkk
*/
void add(void);

/*
	LD Vx, Vy
	Stores the value of Vy into Vx
	hexcode: 8xy0
*/
void ldr(void);

/*
 	OR Vx, Vy
	set Vx = Vx OR Vy
	hexcode: 8xy1
*/
void orr(void);

/*
 	AND Vx, Vy
	set Vx = Vx AND Vy
	hexcode: 8xy2
*/
void andr(void);

/*
 	XOR Vx, Vy
	set Vx = Vx XOR Vy
	hexcode: 8xy3
*/
void xorr(void);

/*
 	ADD Vx, Vy
	set Vx = Vx + Vy, set VF = carry.
	The values of Vx and Vy are added together. If the result is greater than 8 bits (>255) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept and stored in Vx.
	hexcode: 8xy4
*/
void add_carry(void);

/*
 	SUB Vx, Vy
	set Vx = Vx - Vy, set VF = not borrow.
	if Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx and the result stored in Vx
	hexcode: 8xy5
*/
void subr(void);

/*
 	SHR Vx {, Vy}
	set Vx = Vx SHR 1.
	if the least significant bit of Vx is 1, the VF is set to 1, otherwise 0. The Vx is divided by 2.
	hexcode: 8xy6
*/
void shr(void);

/*
 	SUBN Vx, Vy
	set Vx = Vy - Vx, set VF = NOT borrow.
	if Vy > Vx then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy and the result stored in Vx.
	hexcode: 8xy7
*/
void subn(void);

/*
 	SHL Vx {, Vy}
	set Vx = Vx SHL 1.
	if the most significant bit of Vx is 1, set Vf to 1 otherwise 0. Then Vx is multiplied by 2.
	hexcode: 8xyE
*/
void shl(void);

/*
 	SNE Vx, Vy
	skip next instruction if Vx != Vy
	if the values of Vx and Vy are not equal the pc is increased by 2.
	hexcode: 9xy0
*/
void sner(void);

/*
	LD I, addr
	set I = nnn.
	hexcode: Annn
*/
void ld(void);

/*
	JP V0, addr
	jump to location nnn + V0
	pc is set to nnn plus the value of V0
	hexcode: Bnnn
*/
void jpa(void);

/*
 	RND Vx, kk
	set Vx = random byte AND kk
	Generate a random number from 0 to 255, which is ANDed with the value kk. The result is stored in Vx.
	hexcode: Cxkk
*/
void rnd(void);

/*
 	DRW Vx, Vy, nibble
	Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	The interpreter read n bytes from memory, starting at address stored in I. These bytes are then displayed as sprites on screen at coordinated (Vx, Vy).
	Sprites are XORed onto the existing screen. If this cause any pixels to be erased, VF is set to 1, otherwise is it set to 0. If the sprite is positioned so that part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
	hexcode: Dxyn
*/
void drw(void);

/*
 	SKP Vx
	Skip next instruction if key with the value of Vx is pressed.
	Checks the keyboard and if the key correspoinding with the value of Vx is currently in the down position increase the pc by 2.
	hexcode: Ex9E
*/
void skp(void);

/*
 	SKNP Vx
	Skip the next instruction if key with the value of Vx is not pressed.
	Checks the keyboard and if the key corresponding with the value of Vx is currently in the up position increase the pc by 2.
	hexcode: ExA1
*/
void skpn(void);

/*
 	LD Vx, DT
	set Vx = delay timer value.
	hexcode: Fx07
*/
void ltd(void);

/*
 	LD Vx, K
	Wait for a key press, then store the value of the key in Vx.
	All execution stops until a key is pressed, then the value of that key is store in Vx.
	hexcode: Fx0A
*/
void ldk(void);

/*
 	LD DT, Vx
	set delay timer = Vx.
	hexcode: Fx15
*/
void ldt(void);

/*
 	LD ST, Vx
	set sound timer = Vx
	hexcode: Fx18
*/
void lst(void);

/*
 	ADD I, Vx
	set I = I + Vx
	hexcode: Fx1E
*/
void addi(void);

/*
 	LD F, Vx
	set I = location of sprite for digit Vx.
	The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
	hexcode: Fx29 
*/
void ldf(void);

/*
 	LD B, Vx
	Store BCD representation of Vx in memory locations I, I+1, I+2. 
	The interpreter takes the decimal value of Vx, and places the hundreds digits in memory at location I, the tens digit at location I+1, and the units digit at location I+2.
	hexcode: Fx33
*/
void ldb(void);

/*
  	LD [I], Vx
	Store register V0 through Vx in memory starting at location I.
	hexcode: Fx55
*/
void strs(void);

/*
 	LD Vx, [I]
	read registers V0 through Vx from memory starting at location I.
	hexcode: Fx65
*/
void ldrs(void);






#endif
