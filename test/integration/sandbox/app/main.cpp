//For testing
#include "../../../../include/Xioudown/Xioudown.h"
#include "game.h"

#include <stdlib.h>
#include <iostream>
#include <ctime>

using namespace Xioudown;
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
                while(Xioudown_Cat->g_gameState()){

                    //let these essentials be toward the end of the loop.
                    Xioudown_Cat->renderEverything();
                    Xioudown_Cat->processEvents();
                }

            } else {
                log("Unable to load game.");
                STANDBY;    
            }
        }
    }
    return 0;
}