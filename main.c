
#include <unistd.h>
#include "nestor.h"
#include <SDL2/SDL.h>
//#include "graphi.h"


int main(int arg, char * argv[])
{
    //"../nestest.nes";
    //  "../official_only.nes";
    char * game_path = "../Donkey Kong (World) (Rev A).nes";
       // "../Arkanoid (USA).nes";
    //"../Donkey Kong (World) (Rev A).nes";

    struct nestor Nes = nestor_init();

    Nes.video = init_graphics();
    if (INIT_GRAPHICS_FAILED(Nes.video)) {
        fprintf(stderr, "Unable to initialize video output.");
        return 1;
    }

    int err = nestor_cartridge(&Nes, game_path);
    
    if (err) {
        fprintf(stderr, "Unable to load game <%s>\n", game_path);
        return 1;
    }

    //SDL_Window *dbg_win = ppu_mem_view(&Nes.video);
    
    int vblank_clock = 10000;
    while (true) {
        emulate(&Nes);
        if (!(--vblank_clock)){
            puts("UPDATE SCREEN");
            nes_vblank(&Nes); 
            vblank_clock = 10000;
        }

        if (nestor_events(&Nes))
            break;  
        //getchar(); 
    }
    free_graphics(&Nes.video);
    return 0;

}