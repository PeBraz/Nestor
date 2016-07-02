
#include <unistd.h>
#include "nestor.h"
//#include "graphi.h"


int main(int arg, char * argv[])
{
    //"../nestest.nes";
    //  "../official_only.nes";
    char * game_path =  "../Donkey Kong (World) (Rev A).nes";
       // "../Arkanoid (USA).nes";
    //"../Donkey Kong (World) (Rev A).nes";

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
        //usleep(1000);
        emulate(&Nes);

    //  update_screen(&Nes.video);    
    //  graphics(Nes);
    //  sounds(Nes);
    //  events(Nes);
    //  delay() zillion fps
    }
    
    //free_graphics(&Nes.video);
    return 0;

}