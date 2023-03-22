#include "xioudown.h"
#include "game_io.h"
#include "aspects.h"
#include <functional>

using namespace Xioudown;
using namespace Essentials;


gInstancedApp* initKeyboard(gInstancedApp *(&app)) {
    // initialize keyboard
    function<void(void)> appQuit = bind(&gInstancedApp::appQuit, app);
    function<void(void)> moveObj = bind(&gInstancedApp::moveObj, app);
    
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::ESCAPE, appQuit);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::D, moveObj);

    return app;
}
