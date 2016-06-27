#include <SDL2/SDL.h>

#include "nestor.h"

#ifndef GRAPH_H
#define GRAPH_H

//PPU control register -> write
#define PPUCTRL 0x2000
//PPU mask register -> write
#define PPUMASK 0x2001
//PPU status register <- read
#define PPUSTATUS 0x2002

#define OAMADDR 0x2003

#define OAMDATA 0x2004

#define PPUSCROLL 0x2005

#define PPUADDR 0x2006

#define PPUDATA 0x2007

#define OAMDMA 0x4014



//PPUCTRL FLAGS
#define CTRL_NAMETABLE_ADDRESS 0x03
#define CTRL_V_ADDR_INC 0x04
#define CTRL_SPRITE_ADDR 0x08
#define CTRL_BCKGRND_ADDR 0x10
#define CTRL_SPRITE_SIZE 0x20
#define CTRL_MS_SELECT 0x40
#define CTRL_GEN_NMI 0x80

//PPUMASK
#define MASK_GRAYSCALE 0x01
#define MASK_SHOW_BCKGRND_LEFT 0x02
#define MASK_SHOW_SPRITES_LEFT 0x04
#define MASK_SHOW_BCKGRND 0x08
#define MASK_SHOW_SPRITES 0x10
#define MASK_RED 0x20 //NTSC format
#define MASK_GREEN 0x40 //NTSC format
#define MASK_BLUE 0x80 //NTSC format


#define NES_V_MEM_SIZE 16384

enum nes_mirror {
    VERTICAL,
    HORIZONTAL,
    FOUR,
    SINGLE,
};

struct graphics
{
    int pallete[256];
    SDL_Window * window;
    enum nes_mirror mirror;
    uint8_t memory[NES_V_MEM_SIZE];

};

struct graphics init_graphics();
void free_graphics(struct graphics *);
int update_screen(struct graphics *);
void draw_tile(struct graphics*,int,int);
int get_pallete_color(struct graphics *, uint8_t,int);
void draw_pixel(struct graphics *,int,int,int);

#define INIT_GRAPHICS_FAILED(video) (video).window == NULL

#endif