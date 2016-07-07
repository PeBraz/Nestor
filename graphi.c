#include "nestor.h"
#include "graphi.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include <string.h>


#define NAME_TO_ATTR(nametable) (nametable + 960)

void  draw_tile_from_scanline(struct graphics*, int, int);
void sprite_evaluation(struct graphics*, int, int);
void init_pattern_table(struct graphics *);

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

    struct graphics g= (struct graphics)
    { .pallete = 
        {   
            333,014,006,326,403,503,510,420,320,120,031,040,022,000,000,000,
            555,036,027,407,507,704,700,630,430,140,040,053,044,000,000,000,
            777,357,447,637,707,737,740,750,660,360,070,276,077,000,000,000,
            777,567,657,757,747,755,764,772,773,572,473,276,467,000,000,000
        },
        .window = w,
        .pixel = {PIXEL_WIDTH, PIXEL_HEIGHT},
    };
    return g;


sdl_failure:
    return (struct graphics) {.window=NULL, .pallete={0}};
}



#define GREY_1_PIX 0x01000000
#define GREY_2_PIX 0x01494949
#define GREY_3_PIX 0x01BABABA
#define GREY_4_PIX 0x01111111

#define TILES_WIDTH 16
#define TILES_HEIGHT 16
#define PATT_TABLE_WIDTH 128 // 16 * 8
#define PATT_TABLE_HEIGHT 128 // 16 * 8
#define PATT_TILES_COUNT 256   // 4096B = 256tiles * 16 bytes each


//
// Initialize pattern tables, retrieving the tiles from ppu ram. 
//
void init_pattern_table(struct graphics *g) {

    g->pattern0 = malloc(sizeof(SDL_Surface*) * PATT_TILES_COUNT);
    g->pattern1 = malloc(sizeof(SDL_Surface*) * PATT_TILES_COUNT);
    int i;
    for (i=0; i < PATT_TILES_COUNT; i++) {
        g->pattern0[i] = SDL_CreateRGBSurface(0, 
                                                8 * g->pixel.width, 
                                                8 * g->pixel.height, 
                                                32, 0, 0, 0, 0);
        g->pattern1[i] = SDL_CreateRGBSurface(0, 
                                                8 * g->pixel.width, 
                                                8 * g->pixel.height, 
                                                32, 0, 0, 0, 0);
    }
}

void destroy_pattern_table(struct graphics *g) 
{ 
    int i;
    for (i=0; i < PATT_TILES_COUNT; i++) {
        SDL_FreeSurface(g->pattern0[i]);
        SDL_FreeSurface(g->pattern1[i]);
    }
    free(g->pattern0);
    free(g->pattern1);
}

void __update_pattern_table(struct graphics *, SDL_Surface **, int);
void update_pattern_table(struct graphics *g)
{
    __update_pattern_table(g, g->pattern0, 0x0000);
    __update_pattern_table(g, g->pattern1, 0x1000);
}

void __update_pattern_table(struct graphics *g, SDL_Surface **patt, int mem_offset) 
{
    uint32_t my_pixels[] = {GREY_1_PIX, GREY_2_PIX, GREY_3_PIX, GREY_4_PIX};

    int tile_i, byte_i, pix_i;
    for (tile_i=0; tile_i < PATT_TILES_COUNT; tile_i++) {
        for (byte_i=0; byte_i < 16; byte_i += 2) {
            uint8_t plane_1 = g->memory[mem_offset + tile_i+byte_i+1];
            uint8_t plane_2 = g->memory[mem_offset + tile_i+byte_i]; 
            /* For each bit*/
            for (pix_i=0; pix_i < 8; pix_i++) {

                int patt_id = ((plane_1 >> pix_i) & 0x1) | (((plane_2 >> pix_i) & 0x1) << 1);
                int y = byte_i/2 * g->pixel.height;  
                int x = (7-pix_i) * g->pixel.width; 

                SDL_Rect * rect = &(SDL_Rect){x, y, g->pixel.width, g->pixel.height}; 
                SDL_FillRect(patt[tile_i], rect, my_pixels[patt_id]);
            }
        }
    }
}

SDL_Surface *surface_from_patterns(struct graphics *g, SDL_Surface ** patterns) 
{

    int w, h;
    SDL_Surface *surface = SDL_CreateRGBSurface(0, PATT_TABLE_WIDTH * g->pixel.width, 
        PATT_TABLE_HEIGHT*g->pixel.height, 32,0,0,0,0);

    int tile_width = 8*g->pixel.width;
    int tile_height = 8*g->pixel.height;
    int i;
    for (i = 0; i < PATT_TILES_COUNT; i++) {
        SDL_Rect * dst_rect = 
            &(SDL_Rect){  
                .x=tile_width * (i % TILES_WIDTH),
                .y=tile_height * (i / TILES_WIDTH),
                .w=tile_width,
                .h=tile_height
            };
        SDL_BlitSurface(patterns[i], NULL, surface, dst_rect);
    }
    return surface;
}

//
// Creates a window with the patterns in memory
//
SDL_Window * ppu_mem_view(struct graphics *g) {

    int pixel_size = 3;

    g->pixel.width = g->pixel.height = pixel_size;
    int win_width = pixel_size * PATT_TABLE_WIDTH * 2;
    int win_height = pixel_size * PATT_TABLE_HEIGHT;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow( "PPU-VIEW",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        win_width,
        win_height,
        SDL_WINDOW_BORDERLESS);

   SDL_Surface *main_surface = SDL_GetWindowSurface(window);

    init_pattern_table(g);
    update_pattern_table(g);

    SDL_Surface *patt0_surface = surface_from_patterns(g, g->pattern0);
    SDL_Surface *patt1_surface = surface_from_patterns(g, g->pattern1);


    int tile_width = 8*g->pixel.width;
    int tile_height = 8*g->pixel.height;

    SDL_BlitSurface(patt0_surface, NULL, main_surface, 
        &(SDL_Rect){0,0, PATT_TABLE_WIDTH * g->pixel.width, PATT_TABLE_HEIGHT * g->pixel.height});

    SDL_BlitSurface(patt1_surface, NULL, main_surface, 
        &(SDL_Rect){PATT_TABLE_WIDTH * g->pixel.width, 0, PATT_TABLE_WIDTH * g->pixel.width, PATT_TABLE_HEIGHT * g->pixel.height});
   
    SDL_FreeSurface(patt0_surface);
    SDL_FreeSurface(patt1_surface);
    SDL_UpdateWindowSurface(window);

    destroy_pattern_table(g);
    return window;
}




void free_graphics(struct graphics * graphics)
{
    destroy_pattern_table(graphics);
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
        //sprite_evaluation(graphics, tile, line);
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

#define SCREEN_TILES_COUNT 960
void debug_surface(SDL_Surface *surface);
void draw_bg(struct graphics *g) {

    SDL_Surface *main_surface = SDL_GetWindowSurface(g->window);

    int tile_width = 8*g->pixel.width;
    int tile_height = 8*g->pixel.height;
    int i;
    for (i=0; i < SCREEN_TILES_COUNT; i++) {
        uint8_t bg_tile_off = g->memory[g->nametable + i];
        SDL_Surface * tile = (g->bg_addr?g->pattern1:g->pattern0)[bg_tile_off];
        SDL_Rect * dst_rect = 
            &(SDL_Rect){  
                .x=tile_width * (i % 32),
                .y=tile_height * (i / 32),
                .w=tile_width,
                .h=tile_height
            };
        SDL_BlitSurface(tile, NULL, main_surface, dst_rect);
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
    draw_bg(graphics);
    SDL_UpdateWindowSurface(graphics->window);
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
    SDL_Rect rect = (SDL_Rect){  x * graphics->pixel.width,
                                y * graphics->pixel.width,
                                graphics->pixel.width,
                                graphics->pixel.width};

    SDL_Surface * s = SDL_GetWindowSurface(graphics->window);

    SDL_FillRect(s, &rect, SDL_MapRGB(s->format,    (color / 100) % 10,
                                                    (color / 10) % 10,
                                                    color  % 10)
    );

}

