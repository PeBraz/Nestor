#define PPUCTRL 0x2000

#define PPUCTRL_SPRITE_SIZE 0x20
#define PPUCTRL_BCKGRND_ADDR 0x10
#define PPUCTRL_SPRITE_ADDR 0x08


struct nes_ppu {
    int sprite_size;
    uint16_t bckgrnd_addr;
    uint16_t sprite_addr;
    int ppu_inc;
    uint8_t oam[256];
};