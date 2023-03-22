#pragma once
#include "xioudown.h"
#include "aspects.h"

using namespace Xioudown;
using namespace Essentials;

void exit_game()
{
    system("echo yuuuuuuuuuuus");
}

void initKeyboard(gInstancedApp *app)
{
    /* Basic exit game switch. */
    function<void(void)> appQuit = bind(&gInstancedApp::appQuit, app);
    function<void(void)> moveObj = bind(&gInstancedApp::moveObj, app);

    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::ESCAPE, appQuit);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::W, moveObj);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::A, moveObj);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::S, moveObj);
    app->deviceManager()->keyboard()->pressedKey(KEYBOARD_KEY::D, moveObj);
}