#include <stdio.h>
#include "nestor.h"
#include "ppu_registers.h"

void nes_check_read(struct nestor *nes, uint16_t mem_addr)
{
    // If not reading from a ppu register 0x2000 - 0x3FFF

   if (!(mem_addr & 0x2000) || !(mem_addr & (0x3000))) return;

    switch(mem_addr & 0x7) {
        // Writing to ppuctrl
        case 0x0: 
            //access_ppuctrl(nes->video, nes->memory[mem_addr]);
            break;
        case 0x1: break;

        //Reading from ppustatus
        case 0x2: 
            nes->memory[0x2002] &= 0x7F;
            nes->memory[0x2005] = 0;
            nes->memory[0x2006] = 0;
            nes->video.ppuaddr_writes = 0;
            break;
        case 0x3: break;
        case 0x4: break;
        case 0x5: break;
        case 0x6: 
            if (nes->video.ppuaddr_writes) 
                nes->video.vram_addr |= nes->memory[0x2006];
            else 
                nes->video.vram_addr |= (nes->memory[0x2006] << 8);
            nes->video.ppuaddr_writes = !nes->video.ppuaddr_writes;

            break;
        case 0x7: 
            // read vs write
            // read: fetches the memory address and increments vram
            // write:  
            nes->memory[0x2007] = nes->video.memory[nes->video.vram_addr];
            nes->video.vram_addr += 1;//nes->video.vram_inc;
        break;
    }

}


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
    uint16_t byte_pos = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8);
    operation(nes, &(nes->memory[nes->regs.x + byte_pos]));
    nes_check_read(nes, byte_pos);
}

void absolute_y(struct nestor * nes,void(*operation)(struct nestor *,uint8_t*)) 
{
    uint16_t byte_pos = nes->memory[++nes->regs.pc] | (nes->memory[++nes->regs.pc] << 8);
    operation(nes, &(nes->memory[nes->regs.y + byte_pos]));
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
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

void indirect_y(struct nestor * nes, void(*operation)(struct nestor *,uint8_t*))
{   uint16_t pt = nes->memory[++nes->regs.pc];
    uint16_t zero_pt = nes->memory[pt] | (nes->memory[pt + 1] << 8);
    uint16_t byte_pos = zero_pt + nes->regs.y;
    operation(nes, &(nes->memory[byte_pos]));
    nes_check_read(nes, byte_pos);
}

//not really a requirement, it is the mode used by branch operators
void relative(struct nestor * nes, void(*operation)(struct nestor *,uint8_t))
{
    operation(nes, nes->memory[++nes->regs.pc]);
} 
