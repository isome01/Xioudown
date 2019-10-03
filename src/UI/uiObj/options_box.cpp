//dependencies
#include "options_box.h"
#include "../../Catalyst/gameCatalyst.h"

#include <SDL.h>

#ifndef Xioudown_CAT
#define Xioudown_CAT Xioudown::Catalyst::gameCatalyst::gWorld()
#endif

namespace Xioudown { namespace UI {

    void Options_Box::renderUI(SDL_Renderer *r){

        SDL_SetRenderDrawColor( r, 0, 225, 255, 1);
        SDL_RenderFillRect(r, this->getObjSurface() );

        //finally finally render the title
        this->ui_obj_title->setTexture( Xioudown_CAT->getManager()->getFontedTexture( r, this->getName(), {255, 255, 255} ) );
        this->ui_obj_title->render(r, getObjSurface());
    }

    void Options_Box::onChoose(){
        printf("Choosen.\n");
        //we will switch over to whichever interface we desire 
        return;
    }

};};