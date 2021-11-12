#include <iostream>
#include <stdlib.h>
#include "game_window.h"

using std::string;
using std::cout;
using std::endl;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace Xioudown;
using namespace Catalyst;

#ifndef SYS_STANDBY
#define SYS_STANDBY system("pause")
#endif


int main(int argc, char **argv) {

    //Create game window
    GameWindow *game_window = new GameWindow((char*)"New Game", 1280, 960);
    bool systems_go = game_window->getStatus();

    if (!systems_go){
        log("Unable to create window.");
    } else {
        log("Game window instantiated.");
        SYS_STANDBY;
        // //Initialize Level variables
        // log("Created window.")

        // this->Levels = NULL;
        // this->test_room = NULL;

        // //Loading the textures into our texture manager.
        // texture_manager = new TextureManager(); //allocate memory for texture manager
    }

    return EXIT_SUCCESS;
}
