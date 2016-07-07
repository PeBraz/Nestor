#ifndef PPU_REGISTERS_H
#define PPU_REGISTERS_H

#include "graphi.h"
#include "nestor.h"

void nes_check_read(struct nestor *nes, uint16_t mem_addr);

void access_ppuctrl (struct graphics *g, uint8_t ppuctrl);
void access_ppumask (struct graphics *g, uint8_t ppumask);
void update_ppustatus (struct graphics *g, uint8_t *ppustatus); 
void access_oamaddr(struct graphics *g, uint8_t *addr);
#endif 