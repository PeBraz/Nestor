
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "nestor.h"
#include "graphi.h"


static int quit_flag = 0;
static int vblank_clock = 0;

static int update = 0;

void *video_fn(void *__nes)
{
    sleep(1);
    struct nestor *nes = (struct nestor*) __nes;
    while (true) {
        printf("SET NMI\n");
        set_nmi(nes);
        printf("UPDATE\n");
        nes_update(nes); 
    
        if (nestor_events(nes) || quit_flag) {
            quit_flag = 1;
            break;
        }
    }
}

int main(int arg, char * argv[])
{
    //"../nestest.nes";
    //  "../official_only.nes";
    //"../Bomberman (USA).nes";
    char * game_path ="../Donkey Kong (World) (Rev A).nes";//
       // "../Arkanoid (USA).n../NEStress.NESes";
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

    pthread_t video_thr;
    if (pthread_create(&video_thr, NULL, video_fn, (void*)&Nes)) {
        return 1;
    }

    while (true) {
        // emulate a cpu cycle
        if (emulate(&Nes) || quit_flag) {
            quit_flag = 1;
            break;
        }
        vblank_clock += 1;
        if (vblank_clock > 10000) {
            printf("start vblank\n");
            Nes.nmi = 1;
            vblank_clock = 0;
        }

    }
    pthread_join(video_thr, NULL);
    free_graphics(&Nes.video);
    return 0;

}