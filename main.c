#define NESTOR_DEBUG

#include "nestor.h"
#include "graphi.h"


int main(int arg, char * argv[])
{
    
    char * game_path = "../Arkanoid (USA).nes";

    struct nestor Nes = nestor_init();
    int err = nestor_cartridge(&Nes, game_path);
    
    if (err) {
        fprintf(stderr, "Unable to load game <%s>\n", game_path);
        return 1;
    }
    /*Nes.video = init_graphics();
    if (INIT_GRAPHICS_FAILED(Nes.video)) {
        fprintf(stderr, "Unable to initialize video output.");
        return 1;
    }*/
    while (true) {
        emulate(&Nes);
    //  update_screen(&Nes.video);    
    //  graphics(Nes);
    //  sounds(Nes);
    //  events(Nes);
    //  delay() zillion fps
    }
    
    free_graphics(&Nes.video);
    return 0;

}