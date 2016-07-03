#include "nestor.h"
#include "graphi.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include <string.h>

#define SCREEN_WIDTH_PIXEL 256
#define SCREEN_HEIGHT_PIXEL 240

#define SCREEN_WIDTH_TILES 32
#define SCREEN_HEIGHT_TILES 30

#define NAMETABLE1 0x2000
#define ATTRTABLE1 0x23C0
#define PATTTABLE1 0x0000
#define PATTTABLE2 0x1000

#define BCKGRND_PALLETE0 0x3F00
#define BCKGRND_PALLETE1 0x3F05
#define BCKGRND_PALLETE2 0x3F09
#define BCKGRND_PALLETE3 0x3F0D

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


#define PIXEL_WIDTH 4
#define PIXEL_HEIGHT 2

#define NAME_TO_ATTR(nametable) (nametable + 960)

void  draw_tile_from_scanline(struct graphics*, int, int);
void sprite_evaluation(struct graphics*, int, int);

struct graphics init_graphics()
{

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Unable to initialize SDL:  %s\n", SDL_GetError());
        goto sdl_failure;
    }

    SDL_Window * w = SDL_CreateWindow("Nestor -- 0.0 ",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!w) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        goto sdl_failure;
    }

    return (struct graphics)
    { .pallete = 
        {   
            333,014,006,326,403,503,510,420,320,120,031,040,022,000,000,000,
            555,036,027,407,507,704,700,630,430,140,040,053,044,000,000,000,
            777,357,447,637,707,737,740,750,660,360,070,276,077,000,000,000,
            777,567,657,757,747,755,764,772,773,572,473,276,467,000,000,000
        },
        .window = w
    };


sdl_failure:
    return (struct graphics) {.window=NULL, .pallete={0}};
}

//
// Creates a window with the patterns in memory
//

#define GREY_1_PIX 0x01000000
#define GREY_2_PIX 0x01494949
#define GREY_3_PIX 0x01BABABA
#define GREY_4_PIX 0x01111111

SDL_Window * ppu_mem_view(struct graphics *g) {

    uint32_t my_pixels[] = {GREY_1_PIX, GREY_2_PIX, GREY_3_PIX, GREY_4_PIX};

    SDL_Window *window;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow( "PPU-VIEW",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        16 * 8 * 2, // 2 tables 16 tiles 8 pixels
        16 * 8,
        SDL_WINDOW_BORDERLESS);

    /* For each right/left tile*/
    for (int tile_i=0; tile_i < 256; tile_i++) {
        /* For each byte in tile*/
        for (int byte_i=0; byte_i < 8; byte_i++) {
            uint8_t patt_1 = g->memory[tile_i+byte_i];
            uint8_t patt_2 = g->memory[tile_i+byte_i+8]; 

            /* For each bit*/
            for (int pix_i=0; pix_i < 8; pix_i++) {

                int patt_id = ((patt_0 >> pix_i) & 0x1) | (((patt_1 >> pix_i) & 0x1) << 1);

                SDL_LockSurface(window);
                uint32_t *pixels = (uint32_t*)surface->pixels;
                pixels[(y * surface->w) + x] = my_pixels[patt_id];
                SDL_UnlockSurface(window);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
    return window;
}



void free_graphics(struct graphics * graphics)
{
    SDL_DestroyWindow(graphics->window);
    SDL_Quit();
}


void scanline(struct graphics* graphics, int line) {

    if (line >= 0 && line <= 19) {
        //do nothing
    }
    else if (line == 20) {
        //dummy line -- do nothing
    } 
    else if (line >= 21 && line <= 260) {
        // during sequential execution disallow program from accessing ppu memory, 
        //1 scanline - 32 tiles (30 from current and 2 for next scanline)
        int tile;
        sprite_evaluation(graphics, tile, line);
        for (tile = 2; tile < 32; tile++)
            draw_tile_from_scanline(graphics, tile, line); //should return data

        for (tile = 0; tile < 2; tile++)
             draw_tile_from_scanline(graphics, tile, line + 1);
    }
    else if (line == 261) {
        // do nothing
    } else {
        DBG("Incorrect scanline line given.");
    }
}

// 0 <= x < 32   - 1 for tile
// 0 <= y < 240  - 1 for scanline

void  draw_tile_from_scanline(struct graphics* graphics, int x, int y) 
{

    // fetch
    //http://wiki.nesdev.com/w/index.php/PPU_nametables

    //pick a tile byte (0-959)
    int tile_offset = ((y/8) * SCREEN_WIDTH_TILES) + x;

    uint16_t mem_off = graphics->nametable + tile_offset;

    assert(mem_off >= NAMETABLE1 && mem_off < ATTRTABLE1
            && mem_off < NES_V_MEM_SIZE);

    uint8_t tile_pt = graphics->memory[mem_off^0x1];    // xor the address

    uint16_t attr_table_pt = NAME_TO_ATTR(graphics->nametable) + (tile_offset/16);

    uint8_t pallete_picker = graphics->memory[attr_table_pt^0x1] >> (((tile_offset % 16) / 4) & 0x3);

    assert(pallete_picker >= 0x0 && pallete_picker <= 0x3);

    uint8_t tile_pt_low = graphics->bg_pattern_table + tile_pt;
    uint8_t tile_pt_high = tile_pt_low + 8;
    

    // this defines a sprite and colors, this shouldnt be done on runtime, 
    // the sprite should have already been created
    // Im doing this 1 line at a time, which is slower, but closer to the hardware

    uint8_t tile_line_low = graphics->memory[tile_pt_low + (y % 30)]; // only needs 1 line from the tile
    uint16_t tile_line_high = graphics->memory[tile_pt_high + (y % 30)];
    int tile_line = (tile_line_high << 8) | tile_line_low;


    uint16_t palette;
    switch (pallete_picker)
    {
        case 0x0:   palette = BCKGRND_PALLETE0; break;
        case 0x1:   palette = BCKGRND_PALLETE1; break;
        case 0x2:   palette = BCKGRND_PALLETE2; break;
        case 0x3:   palette = BCKGRND_PALLETE3; break;
    }

    int color = 0, color_num;
    int i;
    for (i=0; i<8; i++) {// move in width
        color_num =  ((tile_line >> i) & 0x1) << 1 | ((tile_line >> i) & 0x1);
        color = get_pallete_color(graphics, palette, color_num);
        draw_pixel(graphics, x+i, y, color);
    }
}


//
//      For each scanline evaluates the sprites in oam memory, taking up to 8 sprites it finds,
//  copying them to the secondary oam (needs to check the pixels to draw).
//
//  - sprite overflow flag is not checked
//

void sprite_evaluation(struct graphics *graphics, int x, int y)
{
    #define OAM_SPRITE_SIZE 4

    //clear secondary oam
    memset(graphics->oam2, 0, NES_OAM2_MEM_SIZE);
    int oam2_i = 0;

    int sprite_i; 
    for (sprite_i=0; sprite_i < 64 || oam2_i < 8; sprite_i++){
        //y_addr taken from byte 0
        uint8_t y_addr = graphics->oam[sprite_i * OAM_SPRITE_SIZE];
        if (y_addr >= y && y_addr + 8 < y)  //sprite has 8 pixel range?
            memcpy(&graphics->oam2[oam2_i++], &graphics->oam[sprite_i], 4);
    }

}

int update_screen(struct graphics * graphics)
{

    int y, x;

    for (y = 0 ; y < SCREEN_HEIGHT_TILES; y++) {
        for (x = 0; x < SCREEN_WIDTH_TILES; x++) {
            draw_tile(graphics, x, y);
        }
    }
    SDL_UpdateWindowSurface(graphics->window);
//    SDL_Delay(1000);

    return 0;
}


//
//  0 <= x <= 29
//  0 <= y <= 31
//  Draws one 8x8 tile to the screen
void draw_tile(struct graphics * graphics, int x, int y)
{
    //get offset for tile pointer in nametable
    int offset = (y * SCREEN_WIDTH_TILES) + x;

    uint16_t mem_off = graphics->nametable + offset;

    assert(mem_off >= graphics->nametable && mem_off < ATTRTABLE1
            && mem_off < NES_V_MEM_SIZE);

    uint8_t tile_pt = graphics->memory[mem_off];
    //check register for patter table location, right now assume $0000
    // 1 tile 16 bytes, 

    uint8_t tile_p1 = graphics->memory[tile_pt];
    uint8_t tile_p2 = graphics->memory[tile_pt + 1];
    //... continue getting bytes, 16 total, overlay them to get color

    //get upper pallete from attribue table
    //a byte holds info for 16 tiles
    mem_off = ATTRTABLE1 + offset / 16; //need to find 2 bits inside a byte(i think,. check table again)
    // take 2 bits for  palete number
    // shifts the byte, until it reaches the corresponded position to mask, (16 possible bytes for 4 positions)
    // this determines the background pallete to use
    mem_off >>= (( offset % 16 ) / 4) & 0x3;

    uint16_t palette;
    switch (mem_off)
    {
        case 0x1:   palette = BCKGRND_PALLETE0; break;
        case 0x2:   palette = BCKGRND_PALLETE1; break;
        case 0x3:   palette = BCKGRND_PALLETE2; break;
        case 0x4:   palette = BCKGRND_PALLETE3; break;
    }

    int color = 0;
    int i, j;
    for (i=0; i<8; i++) {//move in height
        for (j=0; j<8; j++) {// move in width
            color = get_pallete_color(graphics, palette,
                        ((graphics->memory[tile_pt] >> j) & 0x1) << 1
                        | ((graphics->memory[tile_pt + 8] >>j) & 0x1)
                    );
            draw_pixel(graphics, x+j, y+i, color);
        }
        tile_pt++;
    }

}

int get_pallete_color(struct  graphics * graphics, uint8_t pallete, int color_num)
{
    return graphics->pallete[graphics->memory[pallete + (color_num & 0x3)]];
}




void draw_pixel(struct graphics * graphics, int x, int y, int color)
{
    SDL_Rect rect = (SDL_Rect){  x * PIXEL_WIDTH,
                                y * PIXEL_HEIGHT,
                                PIXEL_WIDTH,
                                PIXEL_HEIGHT};

    SDL_Surface * s = SDL_GetWindowSurface(graphics->window);

    SDL_FillRect(s, &rect, SDL_MapRGB(s->format,    (color / 100) % 10,
                                                    (color / 10) % 10,
                                                    color  % 10)
    );

}



