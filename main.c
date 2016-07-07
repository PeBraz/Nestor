
#include <unistd.h>
#include "nestor.h"
#include <SDL2/SDL.h>
//#include "graphi.h"


int main(int arg, char * argv[])
{
    //"../nestest.nes";
    //  "../official_only.nes";
    char * game_path =  "../Donkey Kong (World) (Rev A).nes";
       // "../Arkanoid (USA).nes";
    //"../Donkey Kong (World) (Rev A).nes";

    struct nestor Nes = nestor_init();
    Nes.video = (struct graphics){};


    Nes.video = init_graphics();
    if (INIT_GRAPHICS_FAILED(Nes.video)) {
        fprintf(stderr, "Unable to initialize video output.");
        return 1;
    };

    int err = nestor_cartridge(&Nes, game_path);
    
    if (err) {
        fprintf(stderr, "Unable to load game <%s>\n", game_path);
        return 1;
    }

    //SDL_Window *dbg_win = ppu_mem_view(&Nes.video);
    
    int vblank_clock = 100000;
    int ppu_clock = 3;
    while (true) {
        emulate(&Nes);

        if (!(--vblank_clock)){
           // if (Nes.video.vbank_nmi && !(--vblank_clock))
             //   Nes.memory[]*/
            printf("%s\n", "UPDATE SCREEN");
            update_screen(&Nes.video); 
            vblank_clock = 10000;

        printf("vram_off:%x\n", Nes.video.vram_addr);
        if (Nes.video.vram_addr == 0x27FF) 
                getchar();
        }

        if (nestor_events(&Nes))
            break;  
        //getchar(); 
    }
    free_graphics(&Nes.video);
    return 0;

}