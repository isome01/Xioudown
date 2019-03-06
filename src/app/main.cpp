//For testing
#include "../Astral.h"
#include "game.h"

#include <stdlib.h>
#include <iostream>
#include <ctime>

using namespace Astral;
using namespace Catalyst;
using namespace UI;

#define STANDBY system("pause")

Inst_Catalyst(NULL);//instantiate the catalyst

int main(int args, char *argv[]){

    //Let's load things into the manager.   
    
    //first the fonts
    if (!load_fonts()){
        log("Font loading unsuccessful.\n")
        STANDBY;
        
    } else {
        //next texture loading
        if (!load_texturing()){
            log("Texture loading unsuccessful")
            STANDBY;

        } else {
            //Now to give the game a run
            if (load_game() >= 0){
                while(Astral_Cat->g_gameState()){

                    //let these essentials be toward the end of the loop.
                    Astral_Cat->renderEverything();
                    Astral_Cat->processEvents();
                }

            } else {
                log("Unable to load game.");
                STANDBY;    
            }
        }
    }
    return 0;
}