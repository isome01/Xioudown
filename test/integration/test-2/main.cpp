#include "aspects.h"
#include "io_methods.h"
#include "common.h"

gInstancedApp *app = new gInstancedApp();

int main (int argc, char **argv) {

    Logger g_logger("logger.main");

    int count = app->getTestGrid()->gridUnitsCount();
    for (int i = 0; i < count; i++) {
        g_logger << app->getTestGrid()->gridUnit(i)->id();
    }

    /* Basic exit game switch. */
    function<void(void)> appQuit = bind(&gInstancedApp::appQuit, app);
    function<void(void)> moveObj = bind(&gInstancedApp::moveObj, app);
    function<void(void)> upsizeWireFrame = bind(&gInstancedApp::upsizeWireFrame, app);
    function<void(void)> downsizeWireFrame = bind(&gInstancedApp::downsizeWireFrame, app);
    
    g_logger << __cplusplus;

    g_logger << "Add application IO specifics";
    // app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::W, moveObj);
    // app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::A, moveObj);
    // app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::S, moveObj);
    // app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::D, moveObj);
    // app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::W, moveObj);
    // app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::A, moveObj);
    // app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::S, moveObj);
    // app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::D, moveObj);
    app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::SPACE, downsizeWireFrame);
    app->deviceManager()->keyboard()->heldKey(KEYBOARD_KEY::TAB, upsizeWireFrame);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::ESCAPE, appQuit);
    // app = initKeyboard(app);
    g_logger << "Start processing scenarios";
    app->processScenarios();

    // system("pause");
    return EXIT_SUCCESS;
}
