#include "nestor.h"
#include "video.h"

#define NAME_TABLE_1 0x2000
#define NAME_TABLE_2 0x2400
#define NAME_TABLE_3 0x2800
#define NAME_TABLE_4 0x2C00

#define NAMETABLE_WIDTH 32

#define OAMDMA 0x4014

static int palette = {
    333,014,006,326,403,503,510,420,320,120,031,040,022,000,000,000,
    555,036,027,407,507,704,700,630,430,140,040,053,044,000,000,000,
    777,357,447,637,707,737,740,750,660,360,070,276,077,000,000,000,
    777,567,657,757,747,755,764,772,773,572,473,276,467,000,000,000,
    };

#define BCKGRND_COLOR_ADDR 0x3F00

int video(struct nestor * nes)
{
    uint8_t control = nes->v_memory[PPUCTRL];

    //0 -> 8x8
    //1 -> 8x16
    nes->video.sprite_size = (control & PPUCTRL_SPRITE_SIZE)?1:0;
    nes->video.bckgrnd_addr = (control & PPUCTRL_BCKGRND_ADDR)
                            ? 0x1000 : 0x0000;
    nes->video.sprite_addr = (control & PPUCTRL_SPRITE_ADDR)
                            ? 0x1000 : 0x000;
    nes->video.ppu_inc = (control & PPUCTRL_SPRITE_ADDR)? 32 : 1;
    nes->video.name_table = (control & 0x1)?
                                (control & 0x2)? NAME_TABLE_4 : NAME_TABLE_2
                            :   (control & 0x2)? NAME_TABLE_3 : NAME_TABLE_1;
    
} 

// PPUADDR $2006
//used by the cpu to choose a ppu's address,
//because the address is 16 bits, 2 writes are required
//a write will increment the ppu_address by nes->video.ppu_inc, as defined by PPUCTRL.
void write_ppuaddr(struct nestor * nes)
{

}

void write_2007(struct nestor * nes)
{


    nes->video.ppu_inc 
}
void call_NMI(struct nestor * nes) {}

void vblank(struct nestor * nes)
{
    if (nes->video.enable_NMI) {
        call_NMI(nes);  //define me
    }         


}
//For accessing OAM memory in vram, 1 byte at the time
void check_2003(struct nes * nes)
{
    if (nes->video.oam_addr != 0) {
        nes->v_memory[nes->video.oam_addr] = nes->video.oam_data;
        nes->video.oam_addr++;
    }
}

//Direct access memory for transfering the 256 oam bytes from cpu to ppu
void check_dam(struct nestor * nes) 
{
    memcpy(nes->memory[nes->memory[OAMDMA]*0x100], 
            nes->video.oam, 256);
    //blocks cpu for 512 cycles (~4.5 scanlines)
}

#define IDLE 1
#define FETCH_NT_BYTE 2
#define FETCH_AT_BYTE 3


void scanline(struct nestor * nes, int scan_nr, int cycle)
{
    //visible frame
    if (scan_nr >= 0 && scan_nr <= 239) {
        switch (0xF & cycle) 
        {
            case IDLE: break;

            case FETCH_NT_BYTE: 
                fetch_name_table(nes, scan_nr, cycle);
                cycle++;
                break;

            case FETCH_AT_BYTE:
                cycle++;
                break;
                
            case BG_BYTE:
                cycle++;
                break;
                
        }
        cycle++;


    }//Post render Line
    else if (scan_nr < 261) {


    } 
    else //Pre render Line

}

void fetch_name_table(struct nestor * nes, int y, int x)
{
    if (nes->v_memory[PPUCTRL] & 0x1)
}

//
//  Fetch Nametable Byte
//  Fetch Attribute Table Byte
//  Fetch Tile Bitmap low
//  Fetch Tile Bitmap high
//
//  0 <= x < 32
//  0 <= y < 30
//\
void render_tile(struct nestor * nes, int x, int y)
{
    int nametable = nes->video.nametable; 
    int tile_num = y * NAMETABLE_WIDTH + x;

    uint8_t tile_nt = nes->v_memory[nametable + tile_num]
    uint8_t tile_at = nes->v_memory[attr_table + tile_num]
    nes->v_memory[tile_nt]
    nes->v_memory[tile_at]
}