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
#define BREAK_FLAG 0x10
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01

#define BRK 0x00
#define ORA_INDIRECT_X 0x01
#define ORA_ZERO_PAGE 0x05
#define ASL_ZERO_PAGE 0x06
#define PHP 0x08
#define ORA_IMMEDIATE 0x09
#define ASL_ACCUMULATOR 0x0A
#define ORA_ABSOLUTE 0x0D
#define ASL_ABSOLUTE 0x0E
#define BPL 0x10
#define ORA_INDIRECT_Y 0x11
#define	ORA_ZERO_PAGE_X 0x15 
#define ASL_ZERO_PAGE_X 0x16
#define CLC 0x18
#define ORA_ABSOLUTE_Y 0x19
#define	ORA_ABSOLUTE_X 0x1D
#define ASL_ABSOLUTE_X 0x1E
#define JSR 0x20
#define AND_INDIRECT_X 0x21
#define BIT_ZERO_PAGE 0x24
#define AND_ZERO_PAGE 0x25
#define ROL_ZERO_PAGE 0x26
#define PLP 0x28
#define AND_IMMEDIATE 0x29
#define ROL_ACCUMULATOR 0x2A
#define BIT_ABSOLUTE 0x2C
#define AND_ABSOLUTE 0x2D
#define ROL_ABSOLUTE 0x2E
#define BMI 0x30
#define AND_INDIRECT_Y 0x31
#define AND_ZERO_PAGE_X 0x35
#define ROL_ZERO_PAGE_X 0x36
#define SEC 0x38
#define AND_ABSOLUTE_Y 0x39
#define AND_ABSOLUTE_X 0x3D
#define ROL_ABSOLUTE_X 0x3E
#define RTI 0x40
#define EOR_INDIRECT_X 0x41
#define EOR_ZERO_PAGE 0x45
#define LSR_ZERO_PAGE 0x46
#define PHA 0x48
#define EOR_IMMEDIATE 0x49
#define LSR_ACCUMULATOR 0x4A
#define JMP_ABSOLUTE 0x4C
#define EOR_ABSOLUTE 0x4D
#define LSR_ABOLUTE 0x4E
#define BVC 0x50
#define EOR_INDIRECT_Y 0x51
#define EOR_ZERO_PAGE_X 0x55
#define LSR_ZERO_PAGE_X 0x56
#define CLI 0x58
#define EOR_ABSOLUTE_Y 0x59
#define EOR_ABSOLUTE_X 0x5D //confirm
#define LSR_ABOLUTE_X 0x5E
#define RTS 0x60
#define ADC_INDIRECT_X 0x61
#define ADC_ZERO_PAGE 0x65
#define ROR_ZERO_PAGE 0x66
#define PLA 0x68
#define ADC_IMMEDIATE 0x69 
#define ROR_ACCUMULATOR 0x6A
#define JMP_INDIRECT 0x6C
#define ADC_ABSOLUTE 0x6D
#define ROR_ABSOLUTE 0x6E
#define BVS 0x70
#define ADC_INDIRECT_Y 0x71
#define ADC_ZERO_PAGE_X 0x75
#define ROR_ZERO_PAGE_X 0x76
#define SEI 0x78
#define ADC_ABSOLUTE_Y 0x79
#define ADC_ABSOLUTE_X 0x7D
#define ROR_ABSOLUTE_X 0x7E
#define STA_INDIRECT_X 0x81
#define STY_ZERO_PAGE 0x84 
#define STA_ZERO_PAGE 0x85
#define STX_ZERO_PAGE 0x86
#define DEY 0x88
#define TXA 0x8A
#define STY_ABSOLUTE 0x8C
#define STA_ABSOLUTE 0x8D
#define STX_ABSOLUTE 0x8E
#define BCC 0x90 
#define STA_INDIRECT_Y 0x91
#define STY_ZERO_PAGE_X 0x94
#define STA_ZERO_PAGE_X 0x95
#define STX_ZERO_PAGE_Y 0x96
#define TYA 0x98
#define TXS 0x9A
#define STA_ABSOLUTE_X 0x90
#define LDY_IMMEDIATE 0xA0
#define LDA_INDIRECT_X 0xA1
#define LDX_IMMEDIATE 0xA2
#define LDY_ZERO_PAGE 0xA4 
#define LDA_ZERO_PAGE 0xA5
#define LDX_ZERO_PAGE 0xA6
#define TAY 0xA8
#define LDA_IMMEDIATE 0xA9
#define TAX 0xAA
#define LDY_ABSOLUTE 0xAC
#define LDA_ABSOLUTE 0xAD  
#define LDX_ABSOLUTE 0xAE
#define BCS 0xB0
#define LDA_INDIRECT_Y 0xB1
#define LDY_ZERO_PAGE_X 0xB4 
#define LDA_ZERO_PAGE_x 0xB5
#define LDX_ZERO_PAGE_Y 0xB6
#define CLV 0xB8
#define LDA_ABSOLUTE_Y 0xB9 
#define TSX 0xBA
#define LDY_ABSOLUTE_X 0xBC 
#define LDA_ABSOLUTE_X 0xBD
#define LDX_ABSOLUTE_Y 0xBE
#define CPY_IMMEDIATE 0xC0
#define CMP_INDIRECT_X 0xC1
#define CPY_ZERO_PAGE 0xC4
#define CMP_ZERO_PAGE 0xC5
#define DEC_ZERO_PAGE 0xC6
#define INY 0xC8
#define CMP_IMMEDIATE 0xC9
#define DEX 0xCA
#define CPY_ABSOLUTE 0xCC
#define CMP_ABSOLUTE 0xCD
#define DEC_ABSOLUTE 0xCE
#define BNE 0xD0
#define CMP_INDIRECT_Y 0xD1
#define CMP_ZERO_PAGE_X 0xD5 
#define DEC_ZERO_PAGE_x 0xD6
#define CLD 0xD8
#define CMP_ABSOLUTE_Y 0xD9
#define CMP_ABSOLUTE_X 0xDD
#define DEC_ABSOLUTE_X 0xDE
#define CPX_IMMEDIATE 0xE0
#define SBC_INDIRECT_X 0xE1
#define CPX_ZERO_PAGE 0xE4
#define SBC_ZERO_PAGE 0xE5
#define INC_ZERO_PAGE 0xE6
#define INX 0x08
#define SBC_IMMEDIATE 0xE9
#define NOP 0xEA 
#define CPX_ABSOLUTE 0xEC
#define SBC_ABSOLUTE 0xED
#define INC_ABSOLUTE 0xEE
#define BEQ 0xF0 
#define SBC_INDIRECT_Y 0xF1
#define SBC_ZERO_PAGE_X 0xF5
#define INC_ZERO_PAGE_X 0xF6
#define SED 0xF8
#define SBC_ABSOLUTE_X 0xFD
#define INC_ABSOLUTE_X 0xFE


static void (*nes_calls[0xFF])()


struct nestor {
	uint8_t memory[NES_MEM_SIZE];
	struct {
		uint8_t acc;
		uint8_t x;
		uint8_t y;
		uint8_t status;
		uint16_t pc;
		uint16_t sp;
	} regs;
};


void immediate(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
	nes->memory[nes->pc ]
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

void zero_page(struct nestor * nes, void(*operation)(struct nestor *,uint8_t), uint8_t val)
{
	operation(nes, &(nes->memory[val]));
}

void indexed_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint16_t val) 
{
	operation(nes, &(nes->memory[nes->regs.x + val]));
}

void indexed_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint16_t val) 
{
	operation(nes, &(nes->memory[nes->regs.y + val]));
}

void zero_page_indexed_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint8_t val)
{
	operation(nes, &(nes->memory[nes->regs.x + val]));
}

void zero_page_indexed_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint8_t val)
{
	operation(nes, &(nes->memory[nes->regs.y + val]));
}

//only for jump
void indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint16_t val)
{
	operation(nes, (nes->memory[val + 2] << 8) | (nes->memory[val + 1]));
}

void pre_indexed_indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint8_t val)
{
	//x + val must wrap after 0xFF, not 0xFFFF
	int pt_zero_page = (nes->regs.x + val) & 0xFF   
	operation(nes, ((nes->memory[(pt_zero_page + 1 ) & 0xFF] << 8) )
					| ((nes->memory[pt_zero_page]));
}

void post_indexed_indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint8_t val)
{
	int pt_page = (nes->memory[val] << 8) | (nes->memory[val + 1]);
	operation(nes, nes->memory[pt_page + nes->regs.y]);
}

//not really a requirement, it is the mode used by branch operators
void relative(struct nestor * nes, void(*operation)(struct nestor *,uint8_t), uint8_t val)
{
	operation(nes, val);
} 




int overflow_8 (uint8_t a, uint8_t b)
{
	return (a & SIGNED_BIT) == (b & SIGNED_BIT)?
			 ((a+b) & SIGNED_BIT) != (a & SIGNED_BIT) : false;
//	return  (a > 0xFF - b) || (a >0x7F - b);
}

int borrow_8 (uint8_t a, uint8_t b)
{
	return a < 0xFF - b;
}
int dec_borrow_8()
{
	return a < 0x99 - b;
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

	if (!(nes->regs.status & CARRY_FLAG)) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}

/*
 * Branch on carry flag  set
 */
void bcs(struct nestor * nes, uint8_t val) 
{

	if (nes->regs.status & CARRY_FLAG) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}
/*
 * Branch on zero flag set
 */
void beq(struct nestor * nes, uint8_t val) 
{
	if (nes->regs.status & ZERO_FLAG) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}

/*
 *	branch on not zero
 *
 */
void bne(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & ZERO_FLAG)) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
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
	if (nes->regs.status & NEGATIVE_FLAG) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}
/*
 * branch on not negative (plus)
 *
 */
void bpl(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG))
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}

/*
 *	Branch Overflow Clear (overflow 0)
 *
 */
void bvc(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG)) 
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}

/*
 *	Branch Overflow Set (overflow 1)
 *
 */
void bvs(struct nestor * nes, uint8_t val)
{
	if (!(nes->regs.status & NEGATIVE_FLAG))
		nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}


void brk(struct nestor * nes) 
{
	nestor_st_push(nes, (uint8_t)((nes->regs.pc + 2) >> 8 )); //program counter high
	nestor_st_push(nes, (uint8_t)(nes->regs.pc + 2));	//program counter low
	nestor_st_push(nes, nes->regs.status);	//status register
	nes->regs.pc = 0xFFFFFFFE;	//interrupt vector

	nes->regs.status |= BREAK_FLAG;
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
	if (borrow_8(nes->regs.acc, val))
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	zero_update(nes, nes->regs.acc - val);
	negative_update(nes, nes->regs.acc - val);
}

void cpx(struct nestor * nes, uint8_t val)
{
	if (borrow_8(nes->regs.x, val))
		nes->regs.status |= CARRY_FLAG;
	else
		nes->regs.status &= ~CARRY_FLAG;

	zero_update(nes, nes->regs.x - val);
	negative_update(nes, nes->regs.x - val);
}

void cpy(struct nestor * nes, uint8_t val)
{
	if (borrow_8(nes->regs.y, val))
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

void jmp(struct nestor * nes, uint16_t val) 
{
	nes->regs.pc = val;
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

void rti(struct nestor * nes)
{
	nes->regs.status = nestor_st_pop(nes);
	nes->regs.pc = nestor_st_pop(nes) | (nestor_st_pop(nes) << 8);
}

void rts(struct nestor * nes)
{
	nes->regs.pc = nestor_st_pop(nes) | (nestor_st_pop(nes) << 8);
	nes->regs.pc +=1;
}

void sbc(struct nestor * nes, uint8_t val)
{

	int (*borrow_check) (uint8_t,uint8_t) = 
				(nes->regs.status & DECIMAL_FLAG) ? dec_borrow_8 : borrow_8;

	//reset flags
	nes->regs.status &= ~OVERFLOW_FLAG;
	nes->regs.status &= ~NEGATIVE_FLAG;
	nes->regs.status &= ~ZERO_FLAG;

	if (nes->regs.status & CARRY_FLAG) {

		if (!borrow_check(nes->regs.acc, 1)) 
			nes->regs.status &= ~CARRY_FLAG; 
		
		if (overflow_8(nes->regs.acc, 1)) 
			nes->regs.status |= OVERFLOW_FLAG;

		nes->regs.acc -= 1;
	}

	if (borrow_check(nes->regs.acc, val)) 
		nes->regs.status |= CARRY_FLAG;

	if (overflow_8(nes->regs.acc, val))
		nes->regs.status |= OVERFLOW_FLAG;


	nes->regs.acc -= val;


	if (nes->regs.acc & IS_NEGATIVE)
		nes->regs.status |= NEGATIVE_FLAG;

	if (!nes->regs.acc)
		nes->regs.status |= ZERO_FLAG;
}

void sec(struct nestor * nes)
{
	nes->regs.status |= CARRY_FLAG;
}

void sed(struct nestor * nes)
{
	nes->regs.status |= DECIMAL_FLAG;
}

void sei(struct nestor * nes)
{
	nes->regs.status |= INTERRUPT_FLAG;
}

void stx(struct nestor * nes, uint8_t * mem)
{
	*mem = nes->regs.x;
}

void sty(struct nestor * nes, uint8_t * mem)
{
	*mem = nes->regs.y;
}
void tax(struct nestor * nes)
{
	nes->regs.x = nes->regs.acc;
	zero_update(nes, nes->regs.x);
	negative_update(nes, nes->regs.x);
}

void tay(struct nestor * nes)
{
	nes->regs.y = nes->regs.acc;
	zero_update(nes, nes->regs.y);
	negative_update(nes, nes->regs.y);
}
void tsx(struct nestor * nes)
{
	nes->regs.x = nes->regs.sp;
	zero_update(nes, nes->regs.x);
	negative_update(nes, nes->regs.x);
}

void txa(struct nestor * nes)
{
	nes->regs.acc = nes->regs.x;
	zero_update(nes, nes->regs.acc);
	negative_update(nes, nes->regs.acc);
}

void txs(struct nestor * nes)
{
	nes->regs.sp = nes->regs.x;
}

void tya(struct nestor * nes)
{
	nes->regs.acc = nes->regs.y;
	zero_update(nes, nes->regs.acc);
	negative_update(nes, nes->regs.acc);
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
