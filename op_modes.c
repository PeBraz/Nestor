#include <stdio.h>
#include "nestor.h"



void implied(struct nestor * nes, void(*operation)(struct nestor *))
{
    operation(nes);
}

void immediate(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{

    operation(nes, *nestor_load(nes, ++nes->regs.pc));
}

void absolute(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, nestor_load(nes, ++nes->regs.pc));
}


void accumulator(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{   
    operation(nes, &(nes->regs.acc));
}

void zero_page(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, nestor_load(nes, ++nes->regs.pc));
}

void absolute_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    operation(nes, nestor_load(nes, nes->regs.x + ++nes->regs.pc));
}

void absolute_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    operation(nes, nestor_load(nes, nes->regs.y + ++nes->regs.pc));
}

void zero_page_x(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, nestor_load(nes, nes->regs.x + ++nes->regs.pc));
}

void zero_page_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*))
{
    operation(nes, nestor_load(nes, nes->regs.y + ++nes->regs.pc));
}

//only for jump
void indirect(struct nestor * nes, void(*operation)(struct nestor *,uint16_t));
{
    operation(nes, nestor_load(nes, (++nes->regs.pc) << 8) 
                                | nestor_load(++nes->regs.pc));
}

void indirect_x(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
    //x + val must wrap after 0xFF, not 0xFFFF
    int pt_zero_page = (nes->regs.x + ++nes->regs.pc) & 0xFF;
    operation(nes, (nes->memory[pt_zero_page] << 8)
                    | nes->memory[++pt_zero_page  & 0xFF ]);
}

void indirect_y(struct nestor * nes, void(*operation)(struct nestor *,uint16_t))
{
    int pt_page = (nes->memory[++nes->regs.pc] << 8) 
                    | (nes->memory[++nes->regs.pc]);
    operation(nes, nes->memory[pt_page + nes->regs.y]);
}

//not really a requirement, it is the mode used by branch operators
void relative(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
    operation(nes, ++nes->regs.pc);
} 
