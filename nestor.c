#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define NES_MEM_SIZE 65536

#define IS_NEGATIVE 0x80

#define NEGATIVE_FLAG 0x80//or sign flag
#define OVERFLOW_FLAG 0x40
#define DECIMAL_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01


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
	return a > 0x99 - b;
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



	int (*carry_check) (uint8_t,uint8_t) = 
				(nes->regs.status & DECIMAL_FLAG) ? dec_carry_8 : carry_8;

	//reset flags
	nes->regs.status &= ~OVERFLOW_FLAG;
	nes->regs.status &= ~NEGATIVE_FLAG;
	nes->regs.status &= ~ZERO_FLAG;

	if (nes->regs.status & CARRY_FLAG) {

		if (!carry_check(nes->regs.acc, 1)) 
			nes->regs.status &= ~CARRY_FLAG; 
		
		if (overflow_8(nes->regs.acc, 1)) 
			nes->regs.status |= OVERFLOW_FLAG;

		nes->regs.acc += 1;
	}

	if (carry_check(nes->regs.acc, val)) 
		nes->regs.status |= CARRY_FLAG;

	if (overflow_8(nes->regs.acc, val))
		nes->regs.status |= CARRY_FLAG;


	nes->regs.acc += val;


	if (nes->regs.acc & IS_NEGATIVE)
		nes->regs.status |= NEGATIVE_FLAG;

	if (!nes->regs.acc)
		nes->regs.status |= ZERO_FLAG;

}




int main(int arg, char * argv[])
{

	printf( "1 %s\n", test_immediate_lda() == 0? "SUCCESS" :"FAILURE");
	printf( "2 %s\n",  test_absolute_sta() == 0? "SUCCESS" :"FAILURE");
	printf( "3 %s\n",  test_adc() == 0? "SUCCESS" :"FAILURE");
	printf( "4 %s\n",  test_adc_zero() == 0? "SUCCESS" :"FAILURE");
	printf( "5 %s\n",  test_adc_negative() == 0? "SUCCESS" :"FAILURE");
	printf( "6 %s\n",  test_adc_overflow() == 0? "SUCCESS" :"FAILURE");
	printf( "7 %s\n",  test_adc_carry() == 0? "SUCCESS" :"FAILURE");
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

	struct nestor nes;

	nes.regs.acc = 45;
	absolute(&nes, sta, 0x3F43); // store 45 in memory position
	
	if (nes.memory[0x3F43] != 45)	return -1;
	return 0;

}

int test_adc() {

	struct nestor nes;
	nes = (struct nestor){.regs={0}};

	immediate(&nes, adc, 45);	//add 45 to acc

	if (nes.regs.acc != 45) return -1;
	if (nes.regs.status & ZERO_FLAG) return -1;
	if (nes.regs.status & CARRY_FLAG) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;	//failing here
	if (nes.regs.status & OVERFLOW_FLAG) return -1;
	if (nes.regs.status & DECIMAL_FLAG) return -1;

	return 0;
}

int test_adc_zero()
{
	struct nestor nes;
	nes = (struct nestor){.regs={0}};
	immediate(&nes, adc, 0);	

	if (!(nes.regs.status & ZERO_FLAG)) return -1;

	if (nes.regs.acc != 0) return -1;
	if (nes.regs.status & CARRY_FLAG) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;
	if (nes.regs.status & OVERFLOW_FLAG) return -1;

	return 0;
}

int test_adc_negative()
{
	struct nestor nes;
	//negative
	nes = (struct nestor){.regs={0}};
	nes.regs.acc = 128;		// -1 (1000)
	immediate(&nes, adc, 1);	// -2 (1001)

	if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;

	if (nes.regs.acc != 129) return -1;
	if (nes.regs.status & CARRY_FLAG) return -1;
	if (nes.regs.status & ZERO_FLAG) return -1;
	if (nes.regs.status & OVERFLOW_FLAG) return -1;

	return 0;
}

int test_adc_overflow()
{
	struct nestor nes;
	//over +127 overflow
	nes = (struct nestor){.regs={0}};
	nes.regs.acc = 127;		
	immediate(&nes, adc, 1);	// 128 -> -1 (1000)

	if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
	if (!(nes.regs.status & OVERFLOW_FLAG)) return -1;

	if (nes.regs.acc != 128) return -1;
	if (nes.regs.status & CARRY_FLAG) return -1;
	if (nes.regs.status & ZERO_FLAG) return -1;

	return 0;
}

int test_adc_carry()
{
	struct nestor nes;
	//under -128 overflow and carry 
	nes = (struct nestor){.regs={0}};
	nes.regs.acc = 255;	//-128
	immediate(&nes, adc, 1); // 0

	if (!(nes.regs.status & OVERFLOW_FLAG)) return -1;
	if (!(nes.regs.status & CARRY_FLAG)) return -1;

	if (nes.regs.acc != 0) return -1;
	if (nes.regs.status & ZERO_FLAG) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;

	//add with carr

	return 0;
}