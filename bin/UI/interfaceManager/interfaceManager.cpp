//dependencies
#include "interfaceManager.h"
#include "../../Essentials/game_io/game_io.h"
#include "SDL.h"

#include <stdio.h>
#include <string>

int Xioudown::UI::interfaceManager::ui_interface_count = 0;

using namespace Xioudown::Essentials;

namespace Xioudown { namespace UI{

    interfaceManager::interfaceManager(std::string name) {

        this->ui_interface_name = (bool)name[0] ? name : ("UI_Manager_" + (char)ui_interface_count); 

        ui_interface_count++;
        current_ui_interface =-1; //set current interface to be rendered = -1
    }

    interfaceManager::~interfaceManager(){

    }

    void interfaceManager::addInterface(UI_Interface* i){
        
        if (ui_interface_manager.empty()){
            current_ui_interface = 0;
        }

        this->ui_interface_manager.push_back(i);
    }

    void interfaceManager::delInterface(int n){

        if (n <= ui_interface_manager.size()){
            
            ui_interface_manager.erase( ui_interface_manager.begin() + n);
        } else printf("from %s: Indexing out of bounds", __func__);
    }


    //this is where we handle the process all events from chosen objects.
    void interfaceManager::handleChosenObjects(){

        
    }

    //handle player IO: assuming player "has" a UI, just as a player "has" an animated object:
    // - accept input in the method
    void interfaceManager::handleUIEvents(gameIOController *g){
    
        if ( ui_toggle_active && !ui_interface_manager.empty() && g){
        
            if ( g->runKeyCode(SDLK_UP) ){

                //move UIobj selector up
                ui_interface_manager[current_ui_interface]->offsetHighlighter(-1);

            } else if( g->runKeyCode(SDLK_DOWN) ) { //scrolling down

                //move the UIObj selector downward 
                ui_interface_manager[current_ui_interface]->offsetHighlighter(1);
            
            } else if( g->runKeyCode(SDLK_RIGHT) ) { //scrolling down

                //move the UIObj selector downward 
                //ui_interface_manager[current_ui_interface]->offsetHighlighter(getListSize());
            
            } else if( g->runKeyCode(SDLK_LEFT) ) { //scrolling down

                //move the UIObj selector downward 
                //ui_interface_manager[current_ui_interface]->offsetHighlighter(1);
            
            } else if (g->runKeyCode(SDLK_RETURN) || g->runKeyCode(SDLK_KP_ENTER)){
                
                //Execute the onchoose option if [Enter]
                ui_interface_manager[current_ui_interface]->getUIObj()->onChoose();

            } else if (g->runKeyCode(SDLK_ESCAPE) ){

                ui_toggle_active = false;
            }
        }
    }

    void interfaceManager::renderCurrentInterface(SDL_Renderer* r){
        
        //~~ Maybe for interface manager we'll create a taskbar to reference different interfaces
        //... or a taskbar to represent different ui-interface managers. 

        //Render if and only if there are interfaces and interfaces pushed onto render
        if ( ui_toggle_active && (bool)(int)ui_interface_manager.size() ){
            
            ui_interface_manager[current_ui_interface]->renderInterface(r);
            //Render the UI of the current interface, and it's remnants.
        } else { }
    }

};};