#include "aspects.h"
#include "xioudown.h"

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

    short x[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};
    short y[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};

    gWindowInstance->drawFilledPolygon(new Xioudown::Math::Polygon(x, y), {0x00, 0xff, 0x00, 0xff});

    gWindowInstance->drawRect(&rect, {0x00, 0xff, 0x00, 0xff});
    gWindowInstance->presentRenderedItems();
}
