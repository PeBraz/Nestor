#include <SDL2/SDL.h>

#include "nestor.h"

#ifndef GRAPH_H
#define GRAPH_H


#define NAMETABLE0 0x2000
#define NAMETABLE1 0x2400
#define NAMETABLE2 0x2800
#define NAMETABLE3 0x2C00

#define ATTRTABLE0 0x23C0
#define ATTRTABLE1 0x27C0
#define ATTRTABLE2 0x2BC0
#define ATTRTABLE3 0x2FC0

#define PATTTABLE0 0x0000
#define PATTTABLE1 0x1000

#define BCKGRND_PALLETE0 0x3F00
#define BCKGRND_PALLETE1 0x3F05
#define BCKGRND_PALLETE2 0x3F09
#define BCKGRND_PALLETE3 0x3F0D

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SCREEN_WIDTH_PIXEL 256
#define SCREEN_HEIGHT_PIXEL 240

#define SCREEN_WIDTH_TILES 32
#define SCREEN_HEIGHT_TILES 30

#define SCREEN_TILES_COUNT 960

#define PIXEL_WIDTH 1
#define PIXEL_HEIGHT 1

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

#define STATUS_VBLANK 0x80


#define NES_V_MEM_SIZE 16384
#define NES_OAM_MEM_SIZE 256 // (64 * 4)
#define NES_OAM2_MEM_SIZE 32 // (8 * 4)



enum nes_mirror {
    VERTICAL,
    HORIZONTAL,
    FOUR,
    SINGLE,
};

struct graphics
{
    SDL_Color pallete[64];
    SDL_Window * window;
    SDL_Renderer * renderer;
    enum nes_mirror mirror;
    uint8_t memory[NES_V_MEM_SIZE];
    uint8_t oam[NES_OAM_MEM_SIZE];
    uint8_t oam2[NES_OAM2_MEM_SIZE];
    
    uint16_t bg_pattern_table;

    SDL_Surface **pattern0;
    SDL_Surface **pattern1;
    
    struct{
    	int width;
    	int height;
    } pixel;

    /* Keeps tile information cached, preventing unnecessary updates*/
    struct{
        uint8_t id; //nametable offset
        uint8_t patt[16];
        SDL_Surface *tile;
    } cache[SCREEN_TILES_COUNT];

    // PPUCTRL
    uint16_t nametable;
    int vram_inc;
    uint16_t sprite_addr;
    uint16_t bg_addr;
    int sprite_height;
    int ppu_ms_select;
    int vblank_nmi;

    // PPUMASK
    int greyscale;

    // PPUADDR
    int ppuaddr_writes; // number of writes, can only be 0 or 1, reset on ppustatus read

    uint16_t vram_addr;

};



struct graphics init_graphics();
void free_graphics(struct graphics *);
int update_screen(struct graphics *);
void draw_tile(struct graphics*,int,int);
int get_pallete_color(struct graphics *, uint8_t,int);
void draw_pixel(struct graphics *,int,int,int);

void init_pattern_table(struct graphics *);
void update_pattern_table(struct graphics *);

SDL_Window *ppu_mem_view(struct graphics *);
SDL_Window *ppu_nametable_view(struct graphics *);

#define INIT_GRAPHICS_FAILED(video) (video).window == NULL

#endif