#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define NES_MEM_SIZE 65536

#define IS_NEGATIVE 0xA0

#define NEGATIVE_FLAG 0xA0//or sign flag
#define ZERO_FLAG 0x02

#define IMMEDIATE_LDA 0xA9

struct nestor {
	uint8_t memory[NES_MEM_SIZE];
	struct {
		uint8_t acc;
		uint8_t x;
		uint8_t y;
		uint8_t status;
		uint16_t pc;
		uint8_t sp;
	} regs;
};


void immediate(struct nestor * nes, void(*operation)(struct nestor *,uint8_t), uint8_t val)
{
	operation(nes, val);
}

void absolute(struct nestor * nes, void(*operation)(struct nestor *,uint16_t), uint16_t val)
{
	operation(nes, val);
}

void zero_page_absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t), uint8_t val)
{

}


int overflow_8 (uint8_t a, uint8_t b)
{
	return (a > 0xFF - b) || (a >0x7F - b);
}
//http://teaching.idallen.com/dat2343/10f/notes/040_overflow.txt
// for binary carry (up to 255)
int carry_8 (uint8_t a, uint8_t b)
{	
	return a > 0xFF - b;
}
// for decimal carry (up to 99)
int dec_carry_8 (uint8_t a, uint8_t b) 
{
	return a > 99 - b;
}

/*
*
* Load accumualtor with Memory
* M -> A
* 
* flags: Z, N
*/
void lda(struct nestor * nes, uint8_t val)
{	
	nes->regs.acc = val;
	if (val) {
		nes->regs.status &= ~ZERO_FLAG;
	}
	else nes->regs.status &= ZERO_FLAG;

	if (val & IS_NEGATIVE) {
		nes->regs.status &= NEGATIVE_FLAG;
	}
	else nes->regs.status &= ~NEGATIVE_FLAG;

}

/*
*	Store accumulator in memory
*	A -> M
*
* 	flags: --
*/

void sta(struct nestor * nes, uint16_t pt_mem)
{	
	nes->memory[pt_mem] = nes->regs.acc;
}

/*
 * Adds memory to accumulator with carry
 *
 */
void adc(struct nestor * nes, uint8_t val)
{
/*
	uint16_t new_acc = (uint16_t) val;
	if (nes->regs.status & CARRY) {
		new_acc +=1;
	}

	if (carry_8(nes->regs.acc, 1) 
		|| carry_8(nes->regs.acc + 1, val)) {
		nes->regs.status &= ~CARRY;
	}
	nes->regs.acc += val + 1;


	if (overflow_8(val, nes->regs.acc)){
		nes->regs.status |= OVERFLOW;
	}
	//if carry not set and next add provokes carry
	if (!(nes->regs.status & CARRY) && !carry_8(nes->regs.acc, val)) {
		nes->regs.status &= CARRY
	}


	if (nes->regs.status < 0) {
		nes->regs.status &= STATUS
	}
	else{
		nes->regs.status &= (~STATUS)
	}
	//
*/
}




int main(int arg, char * argv[])
{

	struct nestor Nes;

	puts( test_immediate_lda() == 0? "SUCCESS" :"FAILURE");
	puts( test_absolute_sta() == 0? "SUCCESS" :"FAILURE");
	return 0;
}

void emulate(struct nestor * nes)
{
	//take instruction code
	uint8_t opcode = nes->memory[nes->regs.pc];

	switch (opcode){
		case IMMEDIATE_LDA:
			immediate(nes, lda, 69);
			break;
	}


}

/*
*	TEST LDA
*
*/
int test_immediate_lda() {

	struct nestor Nes;

	immediate(&Nes, lda, 0);
	
	if (Nes.regs.acc != 0)	return -1;
	if (Nes.regs.status & ZERO_FLAG == ZERO_FLAG)	return -1;
	if (Nes.regs.status & NEGATIVE_FLAG != NEGATIVE_FLAG) return -1;

	return 0;

}

int test_absolute_sta() {

	struct nestor Nes;

	immediate(&Nes, lda, 45); //load accumulator with 45
	absolute(&Nes, sta, 0x3F43); // store 45 in memory position
	
	if (Nes.memory[0x3F43] != 45)	return -1;
	return 0;

}