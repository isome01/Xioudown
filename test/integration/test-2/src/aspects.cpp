#include "aspects.h"

gInstancedApp::gInstancedApp() {

    this->gWindowInstance = new gAppWindow(
        "Maze of Descension",
        APP_SCREEN_WIDTH,
        APP_SCREEN_HEIGHT
    );
}

void gInstancedApp::render() {

    SDL_Rect rect = {100, 100, 600, 200};
    //rinse the screen each time.
    gWindowInstance->rinse_ui();

    gWindowInstance->drawRect(&rect, {0x00, 0xff, 0x00, 0xff});
    gWindowInstance->presentRenderedItems();
}
