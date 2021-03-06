#include <stdio.h>
#include "nestor.h"
#include "ppu_registers.h"



void implied(struct nestor * nes, void(*operation)(struct nestor *))
{
    operation(nes);
}

void immediate(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, &(nes->memory[++nes->regs.pc]));
}


void absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    //loads 2 bytes after the instruction
    uint16_t byte_pos = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8);
    nes_pre_read(nes, byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

void jmp_absolute(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
    uint16_t byte_pos = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8);
    operation(nes, byte_pos);
    nes->regs.pc--;
}


void accumulator(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{   
    operation(nes, &(nes->regs.acc));
}

void zero_page(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    uint8_t byte_pos = nes->memory[++nes->regs.pc];
    operation(nes, &(nes->memory[byte_pos]));
}

void absolute_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    uint16_t byte_pos = (nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8)) + nes->regs.x;
    nes_pre_read(nes, byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

void absolute_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    uint16_t byte_pos = (nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8)) + nes->regs.y;
    nes_pre_read(nes, byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

void zero_page_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    uint8_t byte_pos = nes->memory[++nes->regs.pc] + nes->regs.x; 
    operation(nes, &(nes->memory[byte_pos]));
}

void zero_page_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    uint8_t byte_pos = nes->memory[++nes->regs.pc] + nes->regs.y; 
    operation(nes, &(nes->memory[byte_pos]));
    //operation(nes, &(nes->memory[ nes->regs.y + ++nes->regs.pc]));
}

//only for jump
void indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
//http://www.6502.org/tutorials/6502opcodes.html
// AN INDIRECT JUMP MUST NEVER USER A VECTOR BEGINNING ON THE LAST BYTE OF A PAGE
    uint16_t addr_pt = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8); 

    uint16_t highbyte_pt = (addr_pt & 0xFF) == 0xFF? addr_pt & 0xFF00 : addr_pt + 1;  
    operation(nes, (nes->memory[addr_pt] | (nes->memory[highbyte_pt] << 8 )) - 1);

}

void indirect_x(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    //x + val must wrap after 0xFF, not 0xFFFF
    uint16_t pt_zero_page = (nes->regs.x + nes->memory[++nes->regs.pc]) & 0xFF;
    uint16_t byte_pos = nes->memory[pt_zero_page] | (nes->memory[++pt_zero_page  & 0xFF] <<8);
    nes_pre_read(nes, byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

void indirect_y(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{   uint16_t pt = nes->memory[++nes->regs.pc];
    uint16_t zero_pt = nes->memory[pt] | (nes->memory[pt + 1] << 8);
    uint16_t byte_pos = zero_pt + nes->regs.y;
    nes_pre_read(nes, byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

//not really a requirement, it is the mode used by branch operators
void relative(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
    operation(nes, nes->memory[++nes->regs.pc]);
} 
