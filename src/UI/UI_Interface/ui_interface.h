#pragma once

//load dependencies
#include "../uiObj/uiObj.h"
#include "../../Essentials/game_io/game_io.h"

#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h> //ui text

#include <string> //for the sake of titles and names
#include <stdio.h> //for logging
#include <vector> //list

namespace Astral{ namespace UI {
    
    class UI_Interface{
        
        private:
            //an interface has a list of UI objects
            std::string interface_name;
            std::vector<UI_Obj*> ui_content_list;
            
            SDL_Rect interface_foundation,
                     interface_title_box; 

        public:
            UI_Interface(std::string _name="");
            ~UI_Interface();

            void renderInterface(SDL_Renderer *r);


        private:
            SDL_Rect ui_obj_selector; //selector for UI objects
            int highlighted_ui, display_by_rows, display_by_cols;
            bool ui_is_matrix; //this is a boolean flag to specify whether a

        public: //Getters and setters
            
            std::string getName()const{ return interface_name; }

            void offsetHighlighter(int n);

            void setCoordinates(SDL_Rect src){ interface_foundation = src; }
            SDL_Rect *getFoundation(){ return &(interface_foundation); }

            int getListSize()const{ return (int)ui_content_list.size(); }

            UI_Obj* getUIObj(){ return ui_content_list[highlighted_ui]; }
            void addUIObj(UI_Obj* obj){ 
                if (ui_content_list.empty())
                    highlighted_ui = 0;
                    
                ui_content_list.push_back(obj); 
            }
            void delUIObj(int index){

                int v_size = (int)ui_content_list.size();

                if (index >= v_size){
                    printf("from s% index out of bounds", __func__ );
                    return;
                } else ui_content_list.erase(ui_content_list.begin() + index);
            }
    };

};};