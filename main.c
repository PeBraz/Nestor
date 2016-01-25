#include "nestor.h"

int main(int arg, char * argv[])
{
    
    struct nestor Nes = nestor_init();

    while (true) {
        emulate(Nes);
    //  graphics(Nes);
    //  sounds(Nes);
    //  events(Nes);
        //delay() game to 60 fps..
    }
    
    
    return 0;

}