#include "game.h"

bool load_fonts(){

    bool flag = true;
    
    SDL_Renderer *r = Astral_Cat->getWindow()->getRenderer();

    Astral_Cat->getManager()->LoadFont("./src/assets/ttf/Denmark.ttf", 14);

    if (!Astral_Cat->getManager()->fontLoaded()){
        printf("From %s: font not loaded.\n", __func__);
        flag = false;
    }

    return flag;
}