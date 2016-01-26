#include "nestor.h"


uint8_t * nestor_load(struct nestor * nes, uint16_t address) 
{
    //get as a little endian address
    return &(nes->memory[(address & 0xFFFE) + ((address & 0x1)^0x1)]); 
}


void nestor_st_push(struct nestor * nes, uint8_t val)
{
    nes->memory[nes->regs.sp--] = val;
}
uint8_t nestor_st_pop(struct nestor * nes)
{
    return nes->memory[++nes->regs.sp];
}

int overflow_8 (uint8_t a, uint8_t b)
{
    return (a & SIGNED_BIT) == (b & SIGNED_BIT)?
             ((a+b) & SIGNED_BIT) != (a & SIGNED_BIT) : false;
//  return  (a > 0xFF - b) || (a >0x7F - b);
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
*   Store accumulator in memory
*   A -> M
*
*   flags: --
*/

void sta(struct nestor * nes, uint8_t * pt_mem)
{   
    * pt_mem = nes->regs.acc;
}

/*
 * Adds memory to accumulator with carry
 *
 */
void adc(struct nestor * nes, uint16_t mem_pt)
{
    uint8_t val = *nestor_load(nes, mem_pt)


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
 *  branch on not zero
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
 *  Branch Overflow Clear (overflow 0)
 *
 */
void bvc(struct nestor * nes, uint8_t val)
{
    if (!(nes->regs.status & NEGATIVE_FLAG)) 
        nes->regs.pc = (uint8_t)((int8_t)nes->regs.pc + (int8_t)val);
}

/*
 *  Branch Overflow Set (overflow 1)
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
    nestor_st_push(nes, (uint8_t)(nes->regs.pc + 2));   //program counter low
    nestor_st_push(nes, nes->regs.status);  //status register

    nes->regs.pc = (nes->memory[0xFFFF] << 8) | nes->memory[0xFFFE];
    nes->regs.status |= BREAK_FLAG;
}
/*
 *  Clear Carry flag 
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


