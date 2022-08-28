#include "aspects.h"
#include <functional>

using namespace Essentials;
using std::bind;
using std::function;
using namespace std::placeholders;

gInstancedApp::gInstancedApp() {

    gWindowInstance =  new gAppWindow(
        "Maze of Descension",
        APP_SCREEN_WIDTH,
        APP_SCREEN_HEIGHT
    );

    for (int i = 0; i < 10; i++) {
        m_grid_objs.push_back(
            new XioudownGridUnit(
                {30 * i, i * 30, 50, 50},
                DEFAULT_XIOUDOWN_GRID_RGBA,
                DEFAULT_XIOUDOWN_GRID_UNIT_TYPE
            )
        );
    }
    m_device_manager = new Essentials::IODeviceManager();
}

gInstancedApp::~gInstancedApp() {
    
    delete this->m_device_manager;
}

void gInstancedApp::render() {
    // Set screen background to default black
    gWindowInstance->setScreenBackgroundColor({0x00, 0x00, 0x00, 0x00});
    gWindowInstance->clearScreenToRenderedColor();

    int items_to_render = (int)m_grid_objs.size();

    for (int i = 0; i < items_to_render; i++) {
        Essentials::rgb rgb = {200, 100, 100};
        gWindowInstance->renderGridUnit(m_grid_objs[i]);
    }

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

gInstancedApp* addMovementKeyboardActions(gInstancedApp *(&app)) {

    function<void(void)> a = bind(&gInstancedApp::appQuit, app);
    function<void(void)> b = bind(&gInstancedApp::testing, app);
    function<void(void)> c = bind(&gInstancedApp::moveObj, app);
    
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::ESCAPE, IOActionType::PRESSED, a);
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::S, IOActionType::PRESSED, b);
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::D, IOActionType::PRESSED, c);

    return app;
}

gInstancedApp* addInteractionsKeyboardActions(gInstancedApp *(&app)) {

    function<void(void)> a = bind(&gInstancedApp::appQuit, app);
    function<void(void)> b = bind(&gInstancedApp::testing, app);

    return app;
}

void gInstancedApp::moveObj() {
    int items_to_render = (int)m_grid_objs.size();   
    for (int i = 0; i < items_to_render; i++) {
        XioudownGridUnit *u = m_grid_objs[i];
        m_grid_objs[i]->x(m_grid_objs[i]->x() + 10);
    }
}