#include <iostream>
#include <stdlib.h>
#include "aspects.h"


gInstancedApp *app = new gInstancedApp();


int main (int argc, char **argv) {
    function<void(void)> a = bind(&gInstancedApp::appQuit, app);
    function<void(void)> b = bind(&gInstancedApp::testing, app);
    function<void(void)> c = bind(&gInstancedApp::moveObj, app);
    
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::ESCAPE, IOActionType::PRESSED, a);
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::S, IOActionType::PRESSED, b);
    app->deviceManager()->keyboard()->addKeyEvent(KEYBOARD_KEY::D, IOActionType::PRESSED, c);
    
    app->processScenarios();

    // system("pause");
    return EXIT_SUCCESS;
}