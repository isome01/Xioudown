#include "aspects.h"
#include "entity.h"
#include <functional>
#include  <stdlib.h>

using namespace Essentials;
using std::bind;
using std::function;
using namespace std::placeholders;
// #include "game_io.h"

std::string APP_NAME = "Fireball Ping-Pong";

gInstancedApp::gInstancedApp() {
    gWindowInstance =  new gAppWindow(
        // "Maze of Descension", for later ^.-
        "Fireball Ping-Pong",
        APP_SCREEN_WIDTH,
        APP_SCREEN_HEIGHT
    );

    // Set level
    m_test_grid = new XioudownGrid(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
    m_test_grid->show_grid(true);

    // add list of entities as units on grid
    for (int i = 0; i < 10; i++) {
        Entity *e = new Entity({30 * i, i * 30});
        e->collisionSet(true);
        e->rgb({0xff, 0, 0});
        e->textureConfig({"src/data/media/img/mainVillagerMan.png", "COM_TEXTURE"});

        m_test_grid->addUnit(e);
    }

    // Set new paddle and add paddle to the list of objects on the grid 
    m_paddle = new Entity({400, 300});
    m_paddle->collisionSet(true);
    m_paddle->rgb({0, 0xff, 0});
    m_paddle->textureConfig({"src/data/media/img/dungeon_floor.png", "Paddle"});
    m_test_grid->addUnit(m_paddle);

    m_device_manager = new IODeviceManager();
}

gInstancedApp::~gInstancedApp()
{
    
    delete this->m_device_manager;
    delete m_paddle;
}

void gInstancedApp::render() {
    // Set screen background to default black
    gWindowInstance->setScreenBackgroundColor({0xF3, 0xF2, 0xEF, 0xFF});
    gWindowInstance->clearScreenToRenderedColor();

    int items_to_render = m_test_grid->gridUnitsCount();

    // Rendering test grid
    gWindowInstance->renderGrid(m_test_grid);

    for (int i = 0; i < items_to_render; i++) {
        Essentials::rgb rgb = {200, 100, 100};
        gWindowInstance->renderGridUnit(m_test_grid->gridUnit(i));
    }

    gWindowInstance->presentRenderedItems();
}

bool gInstancedApp::processScenarios() {
    do {
        if (m_device_manager->ioQuit()) {
            break;
        }
        // First draw the level
        render();
        
        m_device_manager->awaitIO();
        movePaddle(m_test_grid, m_paddle);
        gWindowInstance->setCameraTarget(m_test_grid, m_paddle);
        
    } while (!m_device_manager->ioQuit());

    return 0;
}

gInstancedApp* addMovementKeyboardActions(gInstancedApp *(&app)) {

    function<void(void)> a = bind(&gInstancedApp::appQuit, app);
    function<void(void)> c = bind(&gInstancedApp::moveObj, app);
    
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::ESCAPE, IOActionType::PRESSED, a);
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::D, IOActionType::PRESSED, c);

    return app;
}

gInstancedApp* addInteractionsKeyboardActions(gInstancedApp *(&app)) {

    function<void(void)> a = bind(&gInstancedApp::appQuit, app);

    return app;
}

void gInstancedApp::movePaddle(XioudownGrid *_grid, XioudownGridUnit *paddle)
{
    IOKeyBoard* keyboard = m_device_manager->keyboard();
    XioudownGridUnit *p = paddle;
    if (keyboard->keyIsPressed(KEYBOARD_KEY::W))
    {
        p->y(p->y() - 10);
        int unit_count = _grid->gridUnitsCount();
        for (int i = 0; i < unit_count; i++) {
            p->collision(_grid->gridUnit(i));
        }
    }
    if (keyboard->keyIsPressed(KEYBOARD_KEY::S))
    {
        p->y(p->y() + 10);
        int unit_count = _grid->gridUnitsCount();
        for (int i = 0; i < unit_count; i++) {
            p->collision(_grid->gridUnit(i));
        }
    }
    if (keyboard->keyIsPressed(KEYBOARD_KEY::A))
    {
        p->x(p->x() - 10);
        int unit_count = _grid->gridUnitsCount();
        for (int i = 0; i < unit_count; i++) {
            p->collision(_grid->gridUnit(i));
        }
    }
    if (keyboard->keyIsPressed(KEYBOARD_KEY::D))
    {
        p->x(p->x() + 10);
        int unit_count = _grid->gridUnitsCount();
        for (int i = 0; i < unit_count; i++) {
            p->collision(_grid->gridUnit(i));
        }
    }
    // Check if object collides with any of the other grid objects
    
}

void gInstancedApp::moveObj() {
    IOKeyBoard* keyboard = m_device_manager->keyboard();
        
    int units_count = m_test_grid->gridUnitsCount();
    for (int i = 0; i < units_count; i++) {
        XioudownGridUnit *u = m_test_grid->gridUnit(i);

        if (keyboard->keyIsPressed(KEYBOARD_KEY::W))
        {
            u->y(u->y() - 10);
            u -= rgb({0, 0x01, 0x01});  
        }
        if (keyboard->keyIsPressed(KEYBOARD_KEY::S))
        {
            u->y(u->y() + 10);
            u += rgb({0, 0x01, 0x01});
        }
        if (keyboard->keyIsPressed(KEYBOARD_KEY::A))
        {
            u->x(u->x() - 10);
            u -= rgb({0x01, 0x01, 0});
        }
        if (keyboard->keyIsPressed(KEYBOARD_KEY::D))
        {
            u->x(u->x() + 10);
            u += rgb({0x01, 0x01, 0});
        }
    }
}