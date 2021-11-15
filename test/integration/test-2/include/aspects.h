#pragma once
#include <stdlib.h>
#include "game_window.h"
#include "xioudown.h"
// #include "game_io.h"


using namespace Xioudown;


const int APP_SCREEN_WIDTH = 1280;
const int APP_SCREEN_HEIGHT = 960;
const Xioudown::Essentials::rgba DEFAULT_APP_BKGD_COLOR = {0x00, 0x00, 0x00, 0xff};

class gObject {
};


class gAppWindow : public GameWindow {
    public:
        gAppWindow (char *title, int screen_width, int screen_height) : GameWindow(title, screen_width, screen_height) {
        }

    public:
        void rinse_ui() {
            /* resets UI with render clear and background color */
            setScreenBackgroundColor({0x00, 0x00, 0x00, 0xff});
            clearScreenToRenderedColor();
        }

        void renderItems() {
            this->clearScreenToRenderedColor();
        }
};

class gInstancedApp {
    /* Game app and all the appropriate utils */

    private:
        // vars
    public:
        gInstancedApp();
        ~gInstancedApp();

        void render();
        bool appQuit();
    private:
        gAppWindow *gWindowInstance;
        gInstancedApp (gInstancedApp&){};
        gInstancedApp& operator =(gInstancedApp*){};

        // Xioudown::Essentials::gameIOController *app_io_controller;
};
