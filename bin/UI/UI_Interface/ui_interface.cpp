
//dependencies
#include "../../Catalyst/gameCatalyst.h"
#include "./ui_interface.h"
#include "../../Math/methods.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef Xioudown_CAT
#define Xioudown_CAT Xioudown::Catalyst::gameCatalyst::gWorld()
#endif

namespace Xioudown { namespace UI {

    UI_Interface::UI_Interface(std::string _name) : interface_name(_name) {
        
        //default interface foundation coordinates
        interface_foundation.x = 0;
        interface_foundation.y = 0;
        interface_foundation.w = 400;
        interface_foundation.h = 400;

        //set the SDL_Title box with SDL_ttf.h
    }

    UI_Interface::~UI_Interface(){

    }

    void UI_Interface::offsetHighlighter(int n){

        int sz = (int)ui_content_list.size();
        int ui_dst = highlighted_ui + n;
        if ( ui_dst >= 0 && ui_dst < sz ){

            highlighted_ui = ui_dst;
        }
    }

    void UI_Interface::renderInterface(SDL_Renderer *r){

        if (!ui_content_list.empty()){
            //printf("Yes");

            //Draw the interface foundation
            SDL_SetRenderDrawColor(r, 125, 125, 125, 1);
            SDL_RenderFillRect(r, &(this->interface_foundation));

            //dynamic SDL_Rect rendering:
            // - render content areas for however many UIObjects contained
            int content_sz = (int)ui_content_list.size();
            SDL_Rect *content_areas = new SDL_Rect[ content_sz ];

            //apply matricing view: if applicable.
            int view_ = (ui_is_matrix) ? ( Math::cl_SQRT( content_sz) ) : ( content_sz );
            int _by_view = (ui_is_matrix) ? ( Math::cl_SQRT( content_sz) ) : (1);

            int matrix_index = 0;
            for (int n = 0; n < view_; n++){
                
                for (int i = 0; i < _by_view; i++){
                    content_areas[i].w = interface_foundation.w;
                    content_areas[i].h = interface_foundation.h / content_sz;
                    content_areas[i].x = interface_foundation.x;

                    content_areas[i].y = i * content_areas[i].h; //make sure to line content areas vertically

                    //then finally render the ui_obj on the content area
                    if (matrix_index <= content_sz){
                        SDL_Rect *ui_obj_surface = ui_content_list[i]->getObjSurface();
                        ui_content_list[i]->setObjSurface( Math::centerSDLRect( content_areas[i], *ui_obj_surface) );
                        ui_content_list[i]->renderUI(r);
                        matrix_index++;
                    }
                }
            }

            //finally render the selector
            this->ui_obj_selector = *(ui_content_list[highlighted_ui]->getObjSurface() );
            SDL_SetRenderDrawColor(r, 255, 255, 0, 1);
            SDL_RenderDrawRect(r, &ui_obj_selector);

            //finally finally render the title
            //SDL_Texture *t = Xioudown_CAT->getManager()->getFontedTexture(r, (char*)"DENMARK", (char*) );

        } else printf("Content list is empty.");
    }

};};