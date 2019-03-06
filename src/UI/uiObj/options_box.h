#pragma once
//dependencies
#include "uiObj.h"
#include "../../Essentials/objTextureSheet.h"
#include "../../Essentials/paradigm.h"
#include "../../Catalyst/gameCatalyst.h"

#include <SDL.h>
#include <stdio.h>
#include <string>

namespace Astral { namespace UI{ 

    class Options_Box : public UI_Obj {
        
        private:
            Essentials::gwTexturePack *ui_obj_title;

        public:
            Options_Box(char* _name) : UI_Obj(_name) {
                
                SDL_Rect src = { 0, 0, 250, 50 };
                this->setObjSurface( src );

                SDL_Color title_color = {255, 255, 255};
    
                Essentials::gwTexture_Sheet sheet = {*(this->getObjSurface()), NULL};

                ui_obj_title = new Essentials::gwTexturePack(this->getObjSurface(), &sheet );
            }
            ~Options_Box(){
                delete ui_obj_title;
            }

            void renderUI(SDL_Renderer *r);
            void onChoose();
    };
};};