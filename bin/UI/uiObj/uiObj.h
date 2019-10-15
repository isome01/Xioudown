//Buttons and User interface
#pragma once
//dependencies
#include "../../Essentials/paradigm.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> //ui text
#include <string> //for the sake of titles and names
#include <stdio.h> //for logging
#include <vector> //list

namespace Xioudown { namespace UI{

    class UI_Interface;

    //This is a base class used for simplifying the reuse of a Button or Widget/Tab
    class UI_Obj{
        private: //content of ui
            char* ui_obj_name; //name of course 
            std::string interface_link; //the index to where the obj transitions to another interface

        public: //the reusable ui_obj functions
            UI_Obj( char* name ) : ui_obj_name(name) {}
            ~UI_Obj(){ }
            virtual void onChoose() = 0;
            virtual void renderUI(SDL_Renderer *r) = 0;

        private: //the physical interface physics
            Essentials::gwCoordinates ojb_coord; //relative x and y positions
            SDL_Rect obj_surface; //the surface of the object
            bool obj_chosen; //an event for which an object is chosen

        public: //getters and setters
            char* getName()const{ return ui_obj_name; }
            
            void setLinkIndex(std::string s){ interface_link = s; }
            bool getChosen()const{ return obj_chosen; }
            void setChosen(bool flag){ obj_chosen = flag; }
            
            // ... for object surface
            SDL_Rect* getObjSurface(){ return &obj_surface; }
            void setObjSurface(SDL_Rect r){ obj_surface = r; }
    };


    //all this is is just a class that acts like an on and off switch (in this case though, more like a mutex)
    class UI_switch{

        //in the case of rendering and "onChoose" we should make the 'occupied' value a static bool.
        private:
            bool ui_switch_flag;
            static bool occupied;

        public:
            UI_switch(bool flag = false) : ui_switch_flag(flag) {}
            ~UI_switch();

            //Getters and setters
            bool getSwitch()const{ return ui_switch_flag; }
            void setSwitch(bool flag){ ui_switch_flag = flag; }
            
            void setOccupied(bool flag);
    };

}; };