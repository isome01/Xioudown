#pragma once
#include <stdlib.h>
#include "xioudown.h"
#include "entity.h"
#include "grid.h"
#include <functional>

using namespace Xioudown;
using namespace Essentials;
using std::function;
using std::bind;


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

class gInstancedApp : public XioudownClass {
    /* Game app and all the appropriate utils */

    private:
        XioudownGridUnit *m_paddle;
        XioudownGrid *m_test_grid;
        
    public:
        gInstancedApp();
        ~gInstancedApp();

        void render();
        void appQuit(){ m_device_manager->ioQuit(true); };
        bool processScenarios();
        void moveObj();
        void movePaddle(XioudownGrid *_grid, XioudownGridUnit *paddle);
        void upsizeWireFrame() {
            short width = this->m_test_grid->width();
            short height = this->m_test_grid->height();
            this->m_test_grid->reframeWireFrame(width + 1000, height + 1000);
        }
        void downsizeWireFrame() {
            short width = this->m_test_grid->width();
            short height = this->m_test_grid->height();
            this->m_test_grid->reframeWireFrame(width - 100, height - 100);
        }
        // void processEntityMovement(Entity *e);
        Essentials::IODeviceManager* deviceManager(){ return this->m_device_manager; }

        XioudownGrid* getTestGrid() { return this-> m_test_grid; }

    private:
        gAppWindow *gWindowInstance;
        gInstancedApp& operator =(gInstancedApp*){return *this;};
        IODeviceManager *m_device_manager;
        XioudownGridUnit* paddle() { return this->m_paddle; }
    public:
        void initGameKeyboard();

        // Xioudown::Essentials::gameIOController *app_io_controller;
};

gInstancedApp* addMovementKeyboardActions(gInstancedApp *(&app));

gInstancedApp* addInteractionsKeyboardActions(gInstancedApp *(&app));
