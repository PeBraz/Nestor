#include <stdio.h>
#include "nestor.h"


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
    printf("$%d\n", byte_pos);
    operation(nes, &(nes->memory[byte_pos]));
}

void jmp_absolute(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
    uint16_t byte_pos = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8);
    printf("$%d\n", byte_pos);
    operation(nes, byte_pos);
}


void accumulator(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{   
    operation(nes, &(nes->regs.acc));
}

void zero_page(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, &(nes->memory[++nes->regs.pc]));
}

void absolute_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    operation(nes, &(nes->memory[nes->regs.x + ++nes->regs.pc]));
}

void absolute_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    operation(nes, &(nes->memory[nes->regs.y + ++nes->regs.pc]));
}

void zero_page_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, &(nes->memory[nes->regs.x + ++nes->regs.pc]));
}

void zero_page_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, &(nes->memory[ nes->regs.y + ++nes->regs.pc]));
}

//only for jump
void indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
    operation(nes, nes->memory[++nes->regs.pc]  
                                | (nes->memory[++nes->regs.pc] << 8 ));
}


//??? review this piece 
void indirect_x(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    //x + val must wrap after 0xFF, not 0xFFFF
    uint16_t pt_zero_page = (nes->regs.x + ++nes->regs.pc) & 0xFF;
    operation(nes, &(nes->memory[ nes->memory[pt_zero_page] | (nes->memory[++pt_zero_page  & 0xFF] <<8)]));
}

void indirect_y(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    uint16_t pt_page = nes->memory[++nes->regs.pc] |  (nes->memory[++nes->regs.pc] << 8);
    operation(nes, &(nes->memory[pt_page + nes->regs.y]));
}

//not really a requirement, it is the mode used by branch operators
void relative(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
    operation(nes, nes->memory[++nes->regs.pc]);
} 
