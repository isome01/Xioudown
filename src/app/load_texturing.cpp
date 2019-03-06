#include "game.h"

bool load_texturing(){

    bool flag = true;
    SDL_Renderer *r = Astral_Cat->getWindow()->getRenderer();
    
    //Start loading textures
    Astral_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/lesserImpnoalpha.png");
    Astral_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/basic_stone_brick.png");
    Astral_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/paulina-janzen-dungeon-floor-texture-1.jpg");
    Astral_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/inventory_slot.png");
    /*Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Astral_Cat->getManager()->LoadTexture(r, "../assets/media/img/");*/

    if (Astral_Cat->getManager()->getCount() > 0){
        int size = Astral_Cat->getManager()->getCount(); 

        //check through each texture: return accordingly
        for (int i = 0; i < size; i++){

            if (Astral_Cat->getManager()->getTexture(i) == NULL){
                printf("Error: Texture NULL at index %d\n", i);
                flag = false;
            }
        }

    } else printf("~NOTE~: Texture manager turned out empty.\n");
    
    return flag;
}