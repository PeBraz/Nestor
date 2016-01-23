#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define NES_MEM_SIZE 65536

#define IS_NEGATIVE 0x80
#define SIGNED_BIT 0x80

#define NEGATIVE_FLAG 0x80//or sign flag
#define OVERFLOW_FLAG 0x40
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
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

void absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*), uint16_t val)
{
	operation(nes, &(nes->memory[val]));
}

void accumulator(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{	
	operation(nes, &(nes->regs.acc));
}

void zero_page_absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t), uint8_t val)
{

}


int overflow_8 (uint8_t a, uint8_t b)
{
	return (a & SIGNED_BIT) == (b & SIGNED_BIT)?
			 ((a+b) & SIGNED_BIT) != (a & SIGNED_BIT) : false;
//	return  (a > 0xFF - b) || (a >0x7F - b);
}

int burrow_8 (uint8_t a, uint8_t b)
{
	return a < b;
}

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

void negative_update(struct nestor * nes, uint8_t val)
{
	if (val & IS_NEGATIVE)
		nes->regs.status |=  NEGATIVE_FLAG;
	else 
		nes->regs.status &= ~NEGATIVE_FLAG;
}

void zero_update(struct nestor * nes, uint8_t val)
{
	if (val == 0)
		nes->regs.status |=  ZERO_FLAG;
	else 
		nes->regs.status &= ~ZERO_FLAG;
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

	zero_update(nes, val);
	negative_update(nes, val);
}

void ldx(struct nestor * nes, uint8_t val)
{	
	nes->regs.x = val;

	zero_update(nes, val);
	negative_update(nes, val);
}

void ldy(struct nestor * nes, uint8_t val)
{	
	nes->regs.y = val;

	zero_update(nes, val);
	negative_update(nes, val);
}


/*
*	Store accumulator in memory
*	A -> M
*
* 	flags: --
*/

void sta(struct nestor * nes, uint8_t * pt_mem)
{	
	* pt_mem = nes->regs.acc;
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
		nes->regs.status |= OVERFLOW_FLAG;


	nes->regs.acc += val;


	if (nes->regs.acc & IS_NEGATIVE)
		nes->regs.status |= NEGATIVE_FLAG;

	if (!nes->regs.acc)
		nes->regs.status |= ZERO_FLAG;

}

void and(struct nestor * nes, uint8_t val)
{
	nes->regs.acc &= val;

	negative_update(nes, val);
	zero_update(nes, val);
}
/*
* shift left once
*/
void asl(struct nestor * nes, uint8_t * pt)
{
	if (*pt & SIGNED_BIT)
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	*pt <<= 1;

	negative_update(nes, *pt);
	zero_update(nes, *pt);

}

/*
 * Shift right once
*/
void lsr(struct nestor * nes, uint8_t * pt)
{
	if (*pt & 0x1)
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	*pt >>= 1;

	nes->regs.status &= ~NEGATIVE_FLAG;
	zero_update(nes, *pt);

}

/*
 * Branch on carry flag not set
 */
void bcc(struct nestor * nes, uint8_t val) 
{

	if (!(nes->regs.status & CARRY_FLAG)) nes->regs.pc += val;
}

/*
 * Branch on carry flag  set
 */
void bcs(struct nestor * nes, uint8_t val) 
{

	if (nes->regs.status & CARRY_FLAG) nes->regs.pc += val;
}
/*
 * Branch on zero flag set
 */
void beq(struct nestor * nes, uint8_t val) 
{
	if (nes->regs.status & ZERO_FLAG) nes->regs.pc += val;
}

/*
 *	branch on not zero
 *
 */
void bne(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & ZERO_FLAG)) nes->regs.pc += val;
}

void bit(struct nestor * nes, uint8_t * val)
{
	nes->regs.status = (*val & NEGATIVE_FLAG ) | (*val & OVERFLOW_FLAG);

	zero_update(nes, * val & nes->regs.acc);
}
/*
 * branch on negative (minus)
 *
 */
void bmi(struct nestor * nes, uint8_t val)
{
	if (nes->regs.status & NEGATIVE_FLAG) nes->regs.pc += val;
}
/*
 * branch on not negative (plus)
 *
 */
void bpl(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG)) nes->regs.pc += val;
}

/*
 *	Branch Overflow Clear (overflow 0)
 *
 */
void bvc(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG)) nes->regs.pc += val;
}

/*
 *	Branch Overflow Set (overflow 1)
 *
 */
void bvs(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG)) nes->regs.pc += val;
}


void brk(struct nestor * nes) 
{
	nestor_st_push(nes, (uint8_t)((nes->regs.pc + 2) >> 8 )); //program counter high
	nestor_st_push(nes, (uint8_t)(nes->regs.pc + 2));	//program counter low
	nestor_st_push(nes, nes->regs.status);	//status register
	nes->regs.pc = 0xFFFFFFFE;	//interrupt vector

	//need to put something in stack
	nes->regs.status |= INTERRUPT_FLAG;
}
/*
 *	Clear Carry flag 
 *
 */
void clc(struct nestor * nes)
{
	nes->regs.status &= ~CARRY_FLAG;
}
/*
 * Clear decimal flag
 */
void cld(struct nestor * nes)
{
	nes->regs.status &= ~DECIMAL_FLAG;
}

void cli(struct nestor * nes)
{
	nes->regs.status &= ~INTERRUPT_FLAG;
}
void clv(struct nestor * nes)
{
	nes->regs.status &= ~OVERFLOW_FLAG;
}

void cmp(struct nestor * nes, uint8_t val)
{
	if (burrow_8(nes->regs.acc, val))
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	zero_update(nes, nes->regs.acc - val);
	negative_update(nes, nes->regs.acc - val);
}

void cpx(struct nestor * nes, uint8_t val)
{
	if (burrow_8(nes->regs.x, val))
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	zero_update(nes, nes->regs.x - val);
	negative_update(nes, nes->regs.x - val);
}

void cpy(struct nestor * nes, uint8_t val)
{
	if (burrow_8(nes->regs.y, val))
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	zero_update(nes, nes->regs.y - val);
	negative_update(nes, nes->regs.y - val);
}

void dec(struct nestor * nes, uint8_t *pt)
{
	*pt -= 1;
	negative_update(nes, *pt);
	zero_update(nes, *pt);
}

void dex(struct nestor * nes)
{
	nes->regs.x -= 1;
	negative_update(nes, nes->regs.x);
	zero_update(nes, nes->regs.x);
}

void dey(struct nestor * nes)
{
	nes->regs.y -= 1;
	negative_update(nes, nes->regs.y);
	zero_update(nes, nes->regs.y);
}

void eor(struct nestor * nes, uint8_t val)
{
	nes->regs.acc ^= val;
	negative_update(nes, nes->regs.acc);
	zero_update(nes, nes->regs.acc);
}

void inc(struct nestor * nes, uint8_t * pt)
{
	*pt += 1;
	negative_update(nes, *pt);
	zero_update(nes, *pt);
}

void inx(struct nestor * nes)
{
	nes->regs.x += 1;
	negative_update(nes, nes->regs.x);
	zero_update(nes, nes->regs.x);
}

void iny(struct nestor * nes)
{
	nes->regs.y += 1;
	negative_update(nes, nes->regs.y);
	zero_update(nes, nes->regs.y);
}

void jmp(struct nestor * nes, uint8_t val) 
{
	nes->regs.pc = (nes->memory[nes->regs.pc + 2] << 8) 
					| (nes->memory[nes->regs.pc + 1]); 
}

/*
 * Jump and save return address
 */
void jsr(struct nestor * nes, uint8_t val)
{
	nestor_st_push(nes, nes->regs.pc + 2);
	jmp(nes, val);
}

void nop(struct nestor * nes) {} //just because

void ora(struct nestor * nes, uint8_t val)
{
	nes->regs.acc |= val;
	negative_update(nes, nes->regs.acc);
	zero_update(nes, nes->regs.acc);
}

/*
 * push acc to stack
 */
void pha(struct nestor * nes)
{
	nestor_st_push(nes, nes->regs.acc);
}

void php(struct nestor * nes)
{
	nestor_st_push(nes, nes->regs.status);
}
void pla(struct nestor * nes)
{
	nes->regs.acc = nestor_st_pop(nes); 
}

void plp(struct nestor * nes)
{
	nes->regs.status = nestor_st_pop(nes);
}


void rol(struct nestor * nes, uint8_t * pt)
{
	int carry = nes->regs.status & CARRY_FLAG;

	if (*pt & SIGNED_BIT)
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	*pt <<= 1;
	if (carry) *pt += 1;

	negative_update(nes, *pt);
	zero_update(nes, *pt);

}
void ror(struct nestor * nes, uint8_t * pt)
{
	int carry = nes->regs.status & CARRY_FLAG;

	if (*pt & 0x1)
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	*pt <<= 1;
	if (carry) *pt += 0x80; //add old carry to bit 7

	negative_update(nes, *pt);
	zero_update(nes, *pt);


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
	printf( "8 %s\n",  test_and() == 0? "SUCCESS" :"FAILURE");
	printf( "9 %s\n",  test_asl() == 0? "SUCCESS" :"FAILURE");
	printf( "10 %s\n",  test_bit() == 0? "SUCCESS" :"FAILURE");
	return 0;
}

void nestor_st_push(struct nestor * nes, uint8_t val)
{
	nes->memory[nes->regs.sp--] = val;
}
uint8_t nestor_st_pop(struct nestor * nes, uint8_t val)
{
	return nes->memory[++nes->regs.sp]
}

struct nestor nestor_init()
{
	return (struct nestor){
		.regs = {
			.sp = 0x01FF;
		}
	};
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
	// carry 
	nes = (struct nestor){.regs={0}};
	nes.regs.acc = 255;	//-128
	immediate(&nes, adc, 1); // 0

	if (!(nes.regs.status & CARRY_FLAG)) return -1;
	if (!(nes.regs.status & ZERO_FLAG)) return -1;

	if (nes.regs.acc != 0) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;
	if (nes.regs.status & OVERFLOW_FLAG) return -1;

	return 0;
}

int test_and()
{
	struct nestor nes;

	nes = (struct nestor){.regs={0}};
	nes.regs.acc = 0;	//-128
	immediate(&nes, and, 0); // 0

	if (!(nes.regs.status & ZERO_FLAG)) return -1;
	if (nes.regs.acc != 0) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;

	nes.regs.acc = 0x80;
	immediate(&nes, and, 0x80);

	if (nes.regs.status & ZERO_FLAG) return -1;
	if (nes.regs.acc != 0x80) return -1;
	if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;


	return 0;

}

int test_asl()
{
	struct nestor nes = (struct nestor){.regs={0}};

	nes.regs.acc = 0xFF;
	accumulator(&nes, asl);

	if (nes.regs.acc != 0xFE) return -1;
	if (!(nes.regs.status & CARRY_FLAG)) return -1;
	if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
	if (nes.regs.status & ZERO_FLAG) return -1;

	nes.regs.acc = 0;
	accumulator(&nes, asl);

	if (!(nes.regs.status & ZERO_FLAG)) return -1;
	if (nes.regs.acc != 0) return -1;
	if (nes.regs.status & NEGATIVE_FLAG) return -1;
	if (nes.regs.status & CARRY_FLAG) return -1;

	return 0;
}

int test_bit()
{
	struct nestor nes;

	nes.regs.acc = 0x0F;
	nes.memory[0x1111] = 0xF0;	// will set bit 7 and 6
	absolute(&nes, bit, 0x1111);

	if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
	if (!(nes.regs.status & OVERFLOW_FLAG)) return -1;
	if (!(nes.regs.status & ZERO_FLAG)) return -1;
	
	return 0;
}
