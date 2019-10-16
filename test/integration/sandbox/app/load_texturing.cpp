#include "game.h"

bool load_texturing(){

    bool flag = true;
    SDL_Renderer *r = Xioudown_Cat->getWindow()->getRenderer();
    
    //Start loading textures
    Xioudown_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/lesserImpnoalpha.png");
    Xioudown_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/basic_stone_brick.png");
    Xioudown_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/paulina-janzen-dungeon-floor-texture-1.jpg");
    Xioudown_Cat->getManager()->LoadTexture(r, "./src/assets/media/img/inventory_slot.png");
    /*Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");
    Xioudown_Cat->getManager()->LoadTexture(r, "../assets/media/img/");*/

    if (Xioudown_Cat->getManager()->getCount() > 0){
        int size = Xioudown_Cat->getManager()->getCount();

        //check through each texture: return accordingly
        for (int i = 0; i < size; i++){

            if (Xioudown_Cat->getManager()->getTexture(i) == NULL){
                printf("Error: Texture NULL at index %d\n", i);
                flag = false;
            }
        }

    } else printf("~NOTE~: Texture manager turned out empty.\n");
    
    return flag;
}