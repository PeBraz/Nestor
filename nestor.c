#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define NES_DEF(OP, MODE)\
				void nes_call_ ## OP ## _ ## MODE (struct nestor * nes) {\
					MODE(nes, OP);\
	 			}\


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
#define LSR_ABSOLUTE 0x4E
#define BVC 0x50
#define EOR_INDIRECT_Y 0x51
#define EOR_ZERO_PAGE_X 0x55
#define LSR_ZERO_PAGE_X 0x56
#define CLI 0x58
#define EOR_ABSOLUTE_Y 0x59
#define EOR_ABSOLUTE_X 0x5D //confirm
#define LSR_ABSOLUTE_X 0x5E
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
#define STA_ABSOLUTE_Y
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
#define SBC_ABSOLUTE_Y 0xF9
#define SBC_ABSOLUTE_X 0xFD
#define INC_ABSOLUTE_X 0xFE


struct nestor {
	void (*opcodes[0xFF])(struct nestor *); 
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


void nestor_st_push(struct nestor *, uint8_t);
uint8_t nestor_st_pop(struct nestor *);



uint8_t nestor_load(struct nestor * nes, uint16_t address)
{
	//get as a little endian address
	return nes->memory[(address & 0xFFFE) + ((address & 0x1)^0x1)]; 
}

void implied(struct nestor * nes, void(*operation)(struct nestor *)
{
	operation(nes);
}

void immediate(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
	operation(nes, nestor_load(nes, nes->regs.pc++));

}

void absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*), uint16_t val)
{
	operation(nes, &(nes->memory[val]));
}


void accumulator(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{	
	operation(nes, &(nes->regs.acc));
}

void zero_page(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*), uint8_t val)
{
	operation(nes, &(nes->memory[val]));
}

void absolute_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint16_t val) 
{
	operation(nes, &(nes->memory[nes->regs.x + val]));
}

void absolute_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint16_t val) 
{
	operation(nes, &(nes->memory[nes->regs.y + val]));
}

void zero_page_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint8_t val)
{
	operation(nes, &(nes->memory[nes->regs.x + val]));
}

void zero_page_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*), uint8_t val)
{
	operation(nes, &(nes->memory[nes->regs.y + val]));
}

//only for jump
void indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint16_t val)
{
	operation(nes, (nes->memory[val + 2] << 8) | (nes->memory[val + 1]));
}

void indirect_x(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint8_t val)
{
	//x + val must wrap after 0xFF, not 0xFFFF
	int pt_zero_page = (nes->regs.x + val) & 0xFF;
	operation(nes, (nes->memory[(pt_zero_page + 1 ) & 0xFF] << 8)
					| nes->memory[pt_zero_page]);
}

void indirect_y(struct nestor * nes, void(*operation)(struct nestor *,uint16_t),  uint8_t val)
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
int dec_borrow_8(uint8_t a, uint8_t b)
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

	nes->regs.pc = (nes->memory[0xFFFF] << 8) | nes->memory[0xFFFE];
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


NES_DEF(adc, immediate);
NES_DEF(adc, zero_page);
NES_DEF(adc, zero_page_x);
NES_DEF(adc, absolute);
NES_DEF(adc, absolute_x);
NES_DEF(adc, absolute_y);
NES_DEF(adc, indirect_x);
NES_DEF(adc, indirect_y);

NES_DEF(and, immediate);
NES_DEF(and, zero_page);
NES_DEF(and, zero_page_x);
NES_DEF(and, absolute);
NES_DEF(and, absolute_x);
NES_DEF(and, absolute_y);
NES_DEF(and, indirect_x);
NES_DEF(and, indirect_y);

NES_DEF(asl, accumulator);
NES_DEF(asl, zero_page);
NES_DEF(asl, zero_page_x);
NES_DEF(asl, absolute);
NES_DEF(asl, absolute_x);

NES_DEF(bcc, relative);
NES_DEF(bcs, relative);
NES_DEF(beq, relative);
NES_DEF(bmi, relative);
NES_DEF(bne, relative);
NES_DEF(bpl, relative);
NES_DEF(bvc, relative);
NES_DEF(bvs, relative);

NES_DEF(brk, implied);

NES_DEF(bit, zero_page);
NES_DEF(bit, absolute);

NES_DEF(clc, implied);
NES_DEF(cld, implied);
NES_DEF(cli, implied);
NES_DEF(clv, implied);

NES_DEF(cmp, immediate);
NES_DEF(cmp, zero_page);
NES_DEF(cmp, zero_page_x);
NES_DEF(cmp, absolute);
NES_DEF(cmp, absolute_x);
NES_DEF(cmp, absolute_y);
NES_DEF(cmp, indirect_x);
NES_DEF(cmp, indirect_y);

NES_DEF(cpx, immediate);
NES_DEF(cpx, zero_page);
NES_DEF(cpx, absolute);

NES_DEF(cpy, immediate);
NES_DEF(cpy, zero_page);
NES_DEF(cpy, absolute);

NES_DEF(dec, zero_page);
NES_DEF(dec, zero_page_x);
NES_DEF(dec, absolute);
NES_DEF(dec, absolute_x);

NES_DEF(dex, implied);
NES_DEF(dey, implied);

NES_DEF(eor, immediate);
NES_DEF(eor, zero_page);
NES_DEF(eor, zero_page_x);
NES_DEF(eor, absolute);
NES_DEF(eor, absolute_x);
NES_DEF(eor, absolute_y);
NES_DEF(eor, indirect_x);
NES_DEF(eor, indirect_y);

NES_DEF(inc, zero_page);
NES_DEF(inc, zero_page_x);
NES_DEF(inc, absolute);
NES_DEF(inc, absolute_x);

NES_DEF(inx, implied);
NES_DEF(iny, implied);

NES_DEF(jmp, absolute);
NES_DEF(jmp, indirect);

NES_DEF(jsr, absolute);

NES_DEF(lda, immediate);
NES_DEF(lda, zero_page);
NES_DEF(lda, zero_page_x);
NES_DEF(lda, absolute);
NES_DEF(lda, absolute_x);
NES_DEF(lda, absolute_y);
NES_DEF(lda, indirect_x);
NES_DEF(lda, indirect_y);

NES_DEF(ldx, immediate);
NES_DEF(ldx, zero_page);
NES_DEF(ldx, zero_page_y);
NES_DEF(ldx, absolute);
NES_DEF(ldx, absolute_y);

NES_DEF(ldy, immediate);
NES_DEF(ldy, zero_page);
NES_DEF(ldy, zero_page_y);
NES_DEF(ldy, absolute);
NES_DEF(ldy, absolute_y);


NES_DEF(lsr, accumulator);
NES_DEF(lsr, zero_page);
NES_DEF(lsr, zero_page_x);
NES_DEF(lsr, absolute);
NES_DEF(lsr, absolute_x);

NES_DEF(nop, implied);

NES_DEF(ora, immediate);
NES_DEF(ora, zero_page);
NES_DEF(ora, zero_page_x);
NES_DEF(ora, absolute);
NES_DEF(ora, absolute_x);
NES_DEF(ora, absolute_y);
NES_DEF(ora, indirect_x);
NES_DEF(ora, indirect_y);

NES_DEF(pha, implied);
NES_DEF(php, implied);

NES_DEF(pla, implied);
NES_DEF(plp, implied);

NES_DEF(rol, accumulator);
NES_DEF(rol, zero_page);
NES_DEF(rol, zero_page_x);
NES_DEF(rol, absolute);
NES_DEF(rol, absolute_x);

NES_DEF(ror, accumulator);
NES_DEF(ror, zero_page);
NES_DEF(ror, zero_page_x);
NES_DEF(ror, absolute);
NES_DEF(ror, absolute_x);

NES_DEF(rti, implied);
NES_DEF(rts, implied);

NES_DEF(sbc, immediate);
NES_DEF(sbc, zero_page);
NES_DEF(sbc, zero_page_x);
NES_DEF(sbc, absolute);
NES_DEF(sbc, absolute_x);
NES_DEF(sbc, absolute_y);
NES_DEF(sbc, indirect_x);
NES_DEF(sbc, indirect_y);

NES_DEF(sec, implied);
NES_DEF(sed, implied);
NES_DEF(sei, implied);

NES_DEF(sta, zero_page);
NES_DEF(sta, zero_page_x);
NES_DEF(sta, absolute);
NES_DEF(sta, absolute_x);
NES_DEF(sta, absolute_y);
NES_DEF(sta, indirect_x);
NES_DEF(sta, indirect_y);

NES_DEF(stx, zero_page);
NES_DEF(stx, zero_page_x);
NES_DEF(stx, absolute);

NES_DEF(sty, zero_page);
NES_DEF(sty, zero_page_x);
NES_DEF(sty, absolute);

NES_DEF(tax, implied);
NES_DEF(tay, implied);

NES_DEF(tsx, implied);
NES_DEF(txa, implied);
NES_DEF(txs, implied);
NES_DEF(tya, implied);


int main(int arg, char * argv[])
{



	/*
	struct nestor Nes = nestor_init();

	while (true) {
		emulate(Nes);
		graphics(Nes);
		sounds(Nes);
		events(Nes);
		//delay() game to 60 fps..
	}
	
	*/



	printf( "1 %s\n", test_immediate_lda() == 0? "SUCCESS" :"FAILURE");
	/*printf( "2 %s\n",  test_absolute_sta() == 0? "SUCCESS" :"FAILURE");
	printf( "3 %s\n",  test_adc() == 0? "SUCCESS" :"FAILURE");
	printf( "4 %s\n",  test_adc_zero() == 0? "SUCCESS" :"FAILURE");
	printf( "5 %s\n",  test_adc_negative() == 0? "SUCCESS" :"FAILURE");
	printf( "6 %s\n",  test_adc_overflow() == 0? "SUCCESS" :"FAILURE");
	printf( "7 %s\n",  test_adc_carry() == 0? "SUCCESS" :"FAILURE");
	printf( "8 %s\n",  test_and() == 0? "SUCCESS" :"FAILURE");
	printf( "9 %s\n",  test_asl() == 0? "SUCCESS" :"FAILURE");
	printf( "10 %s\n",  test_bit() == 0? "SUCCESS" :"FAILURE");
	*/return 0;
	}
}

void nestor_st_push(struct nestor * nes, uint8_t val)
{
	nes->memory[nes->regs.sp--] = val;
}
uint8_t nestor_st_pop(struct nestor * nes)
{
	return nes->memory[++nes->regs.sp];
}

struct nestor nestor_init()
{
	struct nestor nes = (struct nestor){
		.opcodes = {NULL},
		.regs = {
			.sp = 0x01FF
		}
	};

	nes.opcodes[BRK] = nes_call_brk;
	nes.opcodes[ORA_INDIRECT_X] = nes_call_ora_indirect_x;
	nes.opcodes[ORA_ZERO_PAGE] = nes_call_ora_zero_page;
	nes.opcodes[ASL_ZERO_PAGE] = nes_call_asl_zero_page;

	nes.opcodes[ADC_IMMEDIATE] = nes_call_adc_immediate;
	nes.opcodes[ADC_ZERO_PAGE] = nes_call_adc_zero_page;
	nes.opcodes[ADC_ZERO_PAGE_X] = nes_call_adc_zero_page_x;
	nes.opcodes[ADC_ABSOLUTE] = nes_call_adc_absolute;
	nes.opcodes[ADC_ABSOLUTE_X] = nes_call_adc_absolute_x;
	nes.opcodes[ADC_ABSOLUTE_Y] = nes_call_adc_absolute_y;
	nes.opcodes[ADC_INDIRECT_X] = nes_call_adc_indirect_x;
	nes.opcodes[ADC_INDIRECT_Y] = nes_call_adc_indirect_y;

	nes.opcodes[AND_IMMEDIATE] = nes_call_and_immediate;
	nes.opcodes[AND_ZERO_PAGE] = nes_call_and_zero_page;
	nes.opcodes[AND_ZERO_PAGE_X] = nes_call_and_zero_page_x;
	nes.opcodes[AND_ABSOLUTE] = nes_call_and_absolute;
	nes.opcodes[AND_ABSOLUTE_X] = nes_call_and_absolute_x;
	nes.opcodes[AND_ABSOLUTE_Y] = nes_call_and_absolute_y;
	nes.opcodes[AND_INDIRECT_X] = nes_call_and_indirect_x;
	nes.opcodes[AND_INDIRECT_Y] = nes_call_and_indirect_y;

	nes.opcodes[ASL_ACCUMULATOR] = nes_call_asl_accumulator;
	nes.opcodes[ASL_ZERO_PAGE] = nes_call_asl_zero_page;
	nes.opcodes[ASL_ZERO_PAGE_X] = nes_call_asl_zero_page_x;
	nes.opcodes[ASL_ABSOLUTE] = nes_call_asl_absolute;
	nes.opcodes[ASL_ABSOLUTE_X] = nes_call_asl_absolute_x;

	nes.opcodes[BCC] = nes_call_bcc_relative;
	nes.opcodes[BCS] = nes_call_bcs_relative;
	nes.opcodes[BEQ] = nes_call_beq_relative;
	nes.opcodes[BMI] = nes_call_bmi_relative;
	nes.opcodes[BNE] = nes_call_bne_relative;
	nes.opcodes[BPL] = nes_call_bpl_relative;
	nes.opcodes[BVC] = nes_call_bcv_relative;
	nes.opcodes[BVS] = nes_call_bvs_relative;

	nes.opcodes[BRK] = nes_call_brk;
	
	nes.opcodes[BIT_ABSOLUTE] = nes_call_bit_absolute;
	nes.opcodes[BIT_ZERO_PAGE] = nes_call_bit_zero_page;

	nes.opcodes[CLC] = nes_call_clc_implied;
	nes.opcodes[CLD] = nes_call_cld_implied;
	nes.opcodes[CLI] = nes_call_cli_implied;
	nes.opcodes[CLV] = nes_call_clv_implied;

	nes.opcodes[CMP_IMMEDIATE] = nes_call_cmp_immediate;
	nes.opcodes[CMP_ZERO_PAGE] = nes_call_cmp_zero_page;
	nes.opcodes[CMP_ZERO_PAGE_X] = nes_call_cmp_zero_page_x;
	nes.opcodes[CMP_ABSOLUTE] = nes_call_cmp_absolute;
	nes.opcodes[CMP_ABSOLUTE_X] = nes_call_cmp_absolute_x;
	nes.opcodes[CMP_ABSOLUTE_Y] = nes_call_cmp_absolute_y;
	nes.opcodes[CMP_INDIRECT_X] = nes_call_cmp_indirect_x;
	nes.opcodes[CMP_INDIRECT_Y] = nes_call_cmp_indirect_y;

	nes.opcodes[CPX_IMMEDIATE] = nes_call_cpx_immediate; 
	nes.opcodes[CPX_ZERO_PAGE] = nes_call_cpx_zero_page;
	nes.opcodes[CPX_ABSOLUTE] = nes_call_cpx_absolute;

	nes.opcodes[CPY_IMMEDIATE] = nes_call_cpy_immediate; 
	nes.opcodes[CPY_ZERO_PAGE] = nes_call_cpy_zero_page;
	nes.opcodes[CPY_ABSOLUTE] = nes_call_cpy_absolute;

	nes.opcodes[DEC_ZERO_PAGE] = nes_call_dec_zero_page;
	nes.opcodes[DEC_ZERO_PAGE_x] = nes_call_dec_zero_page_x;
	nes.opcodes[DEC_ABSOLUTE] = nes_call_dec_absolute;
	nes.opcodes[DEC_ABSOLUTE_X] = nes_call_dec_absolute_x;

	nes.opcodes[DEX] = nes_call_dex_implied;
	nes.opcodes[DEY] = nes_call_dey_implied;

	nes.opcodes[EOR_IMMEDIATE] = nes_call_eor_immediate;
	nes.opcodes[EOR_ZERO_PAGE] = nes_call_eor_zero_page;
	nes.opcodes[EOR_ZERO_PAGE_X] = nes_call_eor_zero_page_x; 
	nes.opcodes[EOR_ABSOLUTE] = nes_call_eor_absolute;
	nes.opcodes[EOR_ABSOLUTE_X] = nes_call_eor_absolute_x;
	nes.opcodes[EOR_ABSOLUTE_Y] = nes_call_eor_absolute_y;
	nes.opcodes[EOR_INDIRECT_X] = nes_call_eor_indirect_x;
	nes.opcodes[EOR_INDIRECT_Y] = nes_call_eor_indirect_y;

	nes.opcodes[INC_ZERO_PAGE] = nes_call_inc_zero_page;
	nes.opcodes[INC_ZERO_PAGE_x] = nes_call_inc_zero_page_x;
	nes.opcodes[INC_ABSOLUTE] = nes_call_inc_absolute;
	nes.opcodes[INC_ABSOLUTE_X] = nes_call_inc_absolute_x;

	nes.opcodes[INX] = nes_call_inx_implied;
	nes.opcodes[INY] = nes_call_iny_implied;

	nes.opcodes[JMP_ABSOLUTE] = nes_call_jmp_absolute;
	nes.opcodes[JMP_INDIRECT] = nes_call_jmp_indirect;

	nes.opcodes[JSR] = nes_call_jsr_absolute;

	nes.opcodes[LDA_IMMEDIATE] = nes_call_lda_immediate;
	nes.opcodes[LDA_ZERO_PAGE] = nes_call_lda_zero_page;
	nes.opcodes[LDA_ZERO_PAGE_x] = nes_call_lda_zero_page_x;
	nes.opcodes[LDA_ABSOLUTE] = nes_call_lda_absolute;
	nes.opcodes[LDA_ABSOLUTE_X] = nes_call_lda_absolute_x;
	nes.opcodes[LDA_ABSOLUTE_Y] = nes_call_lda_absolute_y; 
	nes.opcodes[LDA_INDIRECT_X] = nes_call_lda_indirect_x;
	nes.opcodes[LDA_INDIRECT_Y] = nes_call_lda_indirect_y;

	nes.opcodes[LDX_IMMEDIATE] = nes_call_ldx_immediate;
	nes.opcodes[LDX_ZERO_PAGE] = nes_call_ldx_zero_page;
	nes.opcodes[LDX_ZERO_PAGE_Y] = nes_call_ldx_zero_page_x; 
	nes.opcodes[LDX_ABSOLUTE] = nes_call_ldx_absolute;
	nes.opcodes[LDX_ABSOLUTE_Y] = nes_call_ldx_absolute_y;

	nes.opcodes[LDY_IMMEDIATE] = nes_call_ldy_immediate;
	nes.opcodes[LDY_ZERO_PAGE] = nes_call_ldy_zero_page;
	nes.opcodes[LDY_ZERO_PAGE_Y] = nes_call_ldy_zero_page_x; 
	nes.opcodes[LDY_ABSOLUTE] = nes_call_ldy_absolute;
	nes.opcodes[LDY_ABSOLUTE_Y] = nes_call_ldy_absolute_y;

	nes.opcodes[LSR_ACCUMULATOR] = nes_call_lsr_accumulator;
	nes.opcodes[LSR_ZERO_PAGE] = nes_call_lsr_zero_page;
	nes.opcodes[LSR_ZERO_PAGE_X] = nes_call_lsr_zero_page_x; 
	nes.opcodes[LSR_ABSOLUTE] = nes_call_lsr_absolute;
	nes.opcodes[LSR_ABSOLUTE_X] = nes_call_lst_absolute_x;

	nes.opcodes[NOP] = nes_call_nop_implied;

	nes.opcodes[ORA_IMMEDIATE] = nes_call_ora_immediate;
	nes.opcodes[ORA_ZERO_PAGE] = nes_call_ora_zero_page;
	nes.opcodes[ORA_ZERO_PAGE_X] = nes_call_ora_zero_page_X; 
	nes.opcodes[ORA_ABSOLUTE] = nes_call_ora_absolute;
	nes.pcodes[ORA_ABSOLUTE_X] = nes_call_ora_absolute_x;
	nes.opcodes[ORA_ABSOLUTE_Y] = nes_call_ora_absolute_y;
	nes.opcodes[ORA_INDIRECT_X] = nes_call_ora_indrect_x;
	nes.opcodes[ORA_INDIRECT_Y] = nes_call_ora_indirect_y; 

	nes.opcodes[PHA] = nes_call_pha_implied;
	nes.opcodes[PHP] = nes_call_php_implied;

	nes.opcodes[PLA] = nes_call_pla_implied;
	nes.opcodes[PLP] = nes_call_plp_implied

	nes.opcodes[ROL_ACCUMULATOR] = nes_call_rol_accumulator;
	nes.opcodes[ROL_ZERO_PAGE] = nes_call_rol_zero_page; 
	nes.opcodes[ROL_ZERO_PAGE_X] = nes_call_rol_zero_page_x;
	nes.opcodes[ROL_ABSOLUTE] = nes_call_rol_absolute;
	nes.opcodes[ROL_ABSOLUTE_X] = nes_call_rol_absolute_x;

	nes.opcodes[ROR_ACCUMULATOR] = nes_call_ror_accumulator;
	nes.opcodes[ROR_ZERO_PAGE] = nes_call_ror_zero_page; 
	nes.opcodes[ROR_ZERO_PAGE_X] = nes_call_ror_zero_page_x;
	nes.opcodes[ROR_ABSOLUTE] = nes_call_ror_absolute;
	nes.opcodes[ROR_ABSOLUTE_X] = nes_call_ror_absolute_x;

	nes.opcodes[RTI] = nes_call_rti_implied;
	nes.opcodes[RTS] = nes_call_rts_implied;

	nes.opcodes[SBC_IMMEDIATE] = nes_call_sbc_immediate; 
	nes.opcodes[SBC_ZERO_PAGE] = nes_call_sbc_zero_page;
	nes.opcodes[SBC_ZERO_PAGE_X] = nes_call_sbc_zero_page_x;
	nes.opcodes[SBC_ABSOLUTE] = nes_call_sbc_absolute;
	nes.opcodes[SBC_ABSOLUTE_X] = nes_call_sbc_absolute_x;
	nes.opcodes[SBC_ABSOLUTE_Y] = nes_call_sbc_absolute_y;
	nes.opcodes[SBC_INDIRECT_X] = nes_call_sbc_indirect_x; 
	nes.opcodes[SBC_INDIRECT_Y] = nes_call_sbc_indirect_y;

	nes.opcodes[SEC] = nes_call_sec_implied;
	nes.opcodes[SED] = nes_call_sed_implied;
	nes.opcodes[SEI] = nes_call_sei_implied;

	nes.opcodes[STA_ZERO_PAGE] = nes_call_sta_zero_page;
	nes.opcodes[STA_ZERO_PAGE_X] = nes_call_sta_zero_page_x;
	nes.opcodes[STA_ABSOLUTE] = nes_call_sta_absolute;
	nes.opcodes[STA_ABSOLUTE_X] = nes_call_sta_absolute_x; 
	nes.opcodes[STA_ABSOLUTE_Y] = nes_call_sta_absolute_y;
	nes.opcodes[STA_INDIRECT_X] = nes_call_sta_indirect_x;
	nes.opcodes[STA_INDIRECT_Y] = nes_call_sta_indirect_y;

	nes.opcodes[STX_ZERO_PAGE] = nes_call_stx_zero_page;
	nes.opcodes[STX_ZERO_PAGE_Y] = nes_call_stx_zero_page_y;
	nes.opcodes[STX_ABSOLUTE] = nes_call_stx_absolute;

	nes.opcodes[STY_ZERO_PAGE] = nes_call_sty_zero_page;
	nes.opcodes[STY_ZERO_PAGE_X] = nes_call_sty_zero_page_x;
	nes.opcodes[STY_ABSOLUTE] = nes_call_sty_absolute; 

	nes.opcodes[TAX] = nes_call_tax_implied;
	nes.opcodes[TAY] = nes_call_tay_implied;
	nes.opcodes[TSX] = nes_call_tsx_implied; 
	nes.opcodes[TXA] = nes_call_txa_implied;
	nes.opcodes[TXS] = nes_call_txs_implied;
	nes.opcodes[TYA] = nes_call_tya_implied; 

	return nes;
}


void emulate(struct nestor * nes)
{
	//take instruction code

	uint8_t op = nes->memory[nes->regs.pc];

	if (nes->opcodes[op] != NULL) {
		nes->opcodes[op](nes);
	}
	//else  opcode doesn't exist

}


//tests

uint8_t nestor_set_byte(struct nestor * nes, uint16_t p_byte, uint8_t val)
{
	//get as a little endian address
	nes->memory[(p_byte & 0xFFFE) + ((p_byte & 0x1)^0x1)] = val; 
}


/*
*	TEST LDA
*
*/
int test_immediate_lda() {

	struct nestor Nes;

	Nes.regs.pc = 0x0601;
	nestor_set_byte(&Nes, 0x0601, 0x22);

	immediate(&Nes, lda);
	
	if (Nes.regs.acc != 0x22)	return -1;
	if (Nes.regs.status & ZERO_FLAG != ZERO_FLAG)	return -1;
	if (Nes.regs.status & NEGATIVE_FLAG != NEGATIVE_FLAG) return -1;

	return 0;

}

/*
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
*/