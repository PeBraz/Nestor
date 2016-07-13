#include <string.h>
#include <assert.h>
#include "ppu_registers.h"

#define PPUCTRL_NAMETABLE_BITS 0x03 
#define PPUCTRL_VRAM_INC_BITS 0x04
#define PPUCTRL_SPRITE_ADDR_BITS 0x08
#define PPUCTRL_BG_ADDR_BITS 0x10
#define PPUCTRL_SPRITE_SIZE_BITS 0x20
#define PPUCTRL_MS_SELECT_BITS 0x40
#define PPUCTRL_VBLANK_NMI 0x80


#define PPUMASK_GREYSCALE_MASK 0x01

#define PPUSTATUS_VBLANK 0x80


void access_ppuctrl (struct graphics *g, uint8_t ppuctrl) 
{
    g->nametable = (uint16_t[]){NAMETABLE0, NAMETABLE1, NAMETABLE2, NAMETABLE3}[ppuctrl & PPUCTRL_NAMETABLE_BITS];
    g->vram_inc = (ppuctrl & PPUCTRL_VRAM_INC_BITS)? 32 : 1;
    g->sprite_addr = (ppuctrl & PPUCTRL_SPRITE_ADDR_BITS)? PATTTABLE1: PATTTABLE0;
    g->bg_addr = (ppuctrl & PPUCTRL_BG_ADDR_BITS)? PATTTABLE1: PATTTABLE0;
    g->sprite_height = (ppuctrl & PPUCTRL_SPRITE_SIZE_BITS)? 16: 8;
    g->ppu_ms_select = (ppuctrl & PPUCTRL_MS_SELECT_BITS)? 1:0; //??? no ideia what this should do
    g->vblank_nmi = (ppuctrl & PPUCTRL_VBLANK_NMI)? 1:0;
}


void access_ppumask (struct graphics *g, uint8_t ppumask)
{
    //probably won't care with none of these
    //g->greyscale = (ppumask & PPUMASK_GREYSCALE_MASK)? 1:0;
    //,,,

}


void update_ppustatus (struct graphics *g, uint8_t *ppustatus) 
{
    //sprite overflow (more than 8 sprites in a scanline)
    // bit 7  for vblank state, set during scanline 241
    // Reads by cpu must clear bit 7
}

void access_oamaddr(struct graphics *g, uint8_t *addr)
{
    //provides index to first sprite during sprite evaluation

}

//
// Program is going to read controller register, quick update it!!
//
void nes_pre_read(struct nestor *nes, uint16_t mem_addr) 
{
    if (!(mem_addr == 0x4016)) return;
    nestor_input_read(nes);
}

void nes_check_read(struct nestor *nes, uint16_t mem_addr)
{
    // If not reading from a ppu register 0x2000 - 0x3FFF or 0x4014

   /* if (mem_addr == 0x0015 ) {
        if (nes->memory[0x15] & 0x10){
        printf("mem access (pc: %x) %X: %x\n",nes->regs.pc, mem_addr, nes->memory[mem_addr]);
        getchar();
        }
    }*/
    if (!((mem_addr >= 0x2000 && mem_addr < 0x4000) || mem_addr == 0x4014 || mem_addr == 0x4016)) return;
    //printf("%x - %x\n", mem_addr, nes->memory[mem_addr]);
    if (mem_addr == 0x4014) {
        uint16_t mem_off = (nes->memory[0x4014] << 8);
        /*if (memcmp(nes->video.oam, nes->memory + mem_off, NES_OAM_MEM_SIZE) != 0)
            printf("writing new data\n"); -*/ 
        memcpy(nes->video.oam, nes->memory + mem_off, NES_OAM_MEM_SIZE);
        return;
    }

    //controller
    if (mem_addr == 0x4016) {
        nestor_input_write(nes);
        return;
    }

    switch(mem_addr & 0x7) {
        // Writing to ppuctrl
        case 0x0: 
            access_ppuctrl(&nes->video, nes->memory[mem_addr]);
            break;

        case 0x1: break;

        //Reading from ppustatus
        case 0x2: 
            nes->memory[0x2002] &= 0x7F;
            nes->memory[0x2005] = 0;
            nes->memory[0x2006] = 0;
            nes->video.ppuaddr_writes = 0;
            break;
        case 0x3: 
            break;
        case 0x4: 
            //printf("OAM_DATA: %x\n", nes->memory[mem_addr]);
            break;
        case 0x5: break;
        case 0x6: 
            if (nes->video.ppuaddr_writes) 
                nes->video.vram_addr |= nes->memory[0x2006];
            else {
                nes->video.vram_addr = 0;
                nes->video.vram_addr |= (nes->memory[0x2006] << 8);
            }
            nes->video.ppuaddr_writes = !nes->video.ppuaddr_writes;

            break;
        case 0x7: 
            nes->video.memory[nes->video.vram_addr] = nes->memory[0x2007];
            nes->video.vram_addr += nes->video.vram_inc;
        break;
    }

}