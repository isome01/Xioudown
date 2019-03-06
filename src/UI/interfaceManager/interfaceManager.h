#pragma once

#include "../UI_Interface/ui_interface.h" //for dependencies
#include "../../Essentials/game_io/game_io.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h> //for logging
#include <vector> //for list of interfaces
#include <string>
#include <map>

namespace Astral{ namespace UI{

    //non-static class: there will most likely be more than one set of interfaces
    class interfaceManager{

        private:
            std::map<char*, UI_Interface*> ui_interface_managerp;
            std::vector<UI_Interface*> ui_interface_manager;
            std::string ui_interface_name;
            static int ui_interface_count;
            bool ui_toggle_active;

        public:
            interfaceManager(std::string name = "");
            ~interfaceManager();
            
            virtual void renderCurrentInterface(SDL_Renderer* r);
            virtual void handleChosenObjects();
            void handleUIEvents(Essentials::gameIOController *g); //when UI is being rendered, handle I/O from user 
            
        public: //getters and setters
            UI_Interface* getInterface(int i){ return ui_interface_manager[i]; }
            void addInterface(UI_Interface* i);
            void addInterface(char* _name, UI_Interface* ui);
            void delInterface(int n);

            //toggling our UI
            void toggleUI(){ ui_toggle_active = !ui_toggle_active; }
            void setUIToggle(bool flag){ ui_toggle_active = flag; }
            bool isUIActive()const{ return ui_toggle_active; }

        private:
            int current_ui_interface; //the current index of the interface to be rendered
            
            //For a later time:

            //std::vector<int> current_ui_interface; //pointer; rendering x amount of interfaces.
            //bool is_multiview;
            
    };
};};