#include "game.h"

bool load_fonts(){

    bool flag = true;
    
    SDL_Renderer *r = Xioudown_Cat->getWindow()->getRenderer();

    Xioudown_Cat->getManager()->LoadFont("./assets/ttf/Denmark.ttf", 14);

    if (!Xioudown_Cat->getManager()->fontLoaded()){
        printf("From %s: font not loaded.\n", __func__);
        flag = false;
    }

    return flag;
}