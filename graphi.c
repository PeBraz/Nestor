#include "nestor.h"
#include "graphi.h"

#include <SDL2/SDL.h>
#include <assert.h>

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
        // render a tile
    }
    else if (line == 261) {
        // do nothing
    } else {
        DBG("Incorrect scanline line given.");
    }
}

// 0 <= x <= 32   - 1 for tile
// 0 <= y <= 240  - 1 for scanline

void draw_tile_from_scanline(struct graphics* graphics, int x, int y) 
{

    // fetch
    //http://wiki.nesdev.com/w/index.php/PPU_nametables


    int offset = ((x/8) * SCREEN_WIDTH_TILES) + x;

    uint16_t mem_off = NAMETABLE1 + offset;

    assert(mem_off >= NAMETABLE1 && mem_off < ATTRTABLE1
            && mem_off < NES_V_MEM_SIZE);

    uint8_t tile_pt = graphics->memory[mem_off];
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
    int i;
    for (i=0; i<8; i++) {// move in width
        color = get_pallete_color(graphics, palette,
                    ((graphics->memory[tile_pt + (y % 32)] >> i) & 0x1) << 1
                    | ((graphics->memory[tile_pt + (y % 32)] >> i) & 0x1)
                    );
            draw_pixel(graphics, x+i, y, color);
    //tile_pt++;  // ?? need to calculate remainder directly depends on y (y % 32)
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



