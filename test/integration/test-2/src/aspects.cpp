#include "aspects.h"
#include "io_methods.h"

using namespace Essentials;

gInstancedApp::gInstancedApp() {

    gWindowInstance =  new gAppWindow(
        "Maze of Descension",
        APP_SCREEN_WIDTH,
        APP_SCREEN_HEIGHT
    );

    for (int i = 0; i < 10; i++) {
        m_grid_objs.push_back(
            new XioudownGridUnit(
                {10 * i, i * 10, 50, 50},
                DEFAULT_XIOUDOWN_GRID_RGBA,
                DEFAULT_XIOUDOWN_GRID_UNIT_TYPE
            )
        );
    }

    m_device_manager = new Essentials::IODeviceManager();
    m_device_manager->keyboard()->addKeyEvent(KEYBOARD_KEY::S, IOActionType::HELD, exit_game);
}

gInstancedApp::~gInstancedApp() {
    
    delete this->m_device_manager;
}

void gInstancedApp::render() {

    SDL_Rect rect = {100, 100, 600, 200};

    int items_to_render = (int)m_grid_objs.size();

    for (int i = 0; i < items_to_render; i++) {
        Essentials::rgb rgb = {200, 100, 100};
        XioudownGridUnit *unit = (m_grid_objs[i] - rgb + rgb);
        gWindowInstance->renderGridUnit(unit);
    }
    
    // short x[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};
    // short y[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};

    // gWindowInstance->drawRect(&rect, {0x00, 0xff, 0x00, 0xff});
    gWindowInstance->presentRenderedItems();
}

bool gInstancedApp::processScenarios() {
    do {
        render();
        m_device_manager->awaitIO();
        if (m_device_manager->ioQuit()) {
            break;
        }
    } while (!m_device_manager->ioQuit());

    return 0;
}