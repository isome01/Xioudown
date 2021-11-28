#include <SDL.h>
#include <stdio.h>
#include "game_io.h"
#include "utils.h"
#include <string>


namespace Xioudown{ namespace Essentials{
    
    IODevice::IODevice(IOType _type) : m_game_io_type(_type) {

        m_io_id = Math::generateReferenceId<IODevice*>(this);
    }

    IODevice::~IODevice() {
        
    }

    void IODevice::assignActionMethodPair(SDL_Scancode _code, IOActionType _type, void(&_method)()) {
        /* Assign key pair of code to device action */
        m_device_methods[_code] = new IODeviceAction({_type, _method});
    }
};};

namespace Xioudown { namespace Essentials {
    /* Keyboard implementation*/

    IOKeyBoard::IOKeyBoard() : IODevice(IOType::KEYBOARD) {
        //set all keycodes to false
        
        for (int i = 0; i < DEFAULT_IO_CODE_RANGE; i++){
            key_codes.push_back(false);
        }
    }

    IOKeyBoard::~IOKeyBoard() {
        
    }

    int IOKeyBoard::recodeKeycode(Uint32 n)
    {
        return (int)(( n >= 1073741824) ? (n - 1073741824) + 5000 : n) ;
    }


    void IOKeyBoard::processIO(SDL_Event e) {
        //because of value struggles on our 'SDLK_UP' SDLK_Scancode_Masks
        int code = SDL_GetScancodeFromKey(e.key.keysym.sym);
        
        switch (e.type) {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
                previousKeyPressed = previousKeyPressed ? previousKeyPressed : e.key.keysym.sym;
                key_codes[code] = true;
                break;
            case SDL_KEYUP:
                key_codes[code] = false;
                break;
            default:
                break;
        }

        initiateKeyboardEvents();
    }

    void IOKeyBoard::addKeyEvent(KEYBOARD_KEY _key, IOActionType _type, void(&f)()) {
         // NOTE: make key lowercase if needed

        assignActionMethodPair(
            SDL_GetScancodeFromKey(_key), _type, f
        );
    }

    void IOKeyBoard::initiateKeyboardEvents() {
        // probably should move this implementation to manager.
        /* This will run through each key pair and check if their corresponding actions trigger a method execution  */

        std::map<SDL_Scancode, IODeviceAction*>::iterator it;
        std::map<SDL_Scancode, IODeviceAction*> *mapping = getKeyMapping();
        for (it = (*mapping).begin(); it != (*mapping).end(); it++) {

            const SDL_Scancode code = it->first;
            const IODeviceAction *action = it->second;
            const IOActionType type = action->action;

            switch(type) {
                case (IOActionType::PRESSED):  // if scancode is 'keyIsPressed'... then execute method...
                    if (keyIsPressed(code)){
                        (*action->method)();
                    }
                    break;
                case (IOActionType::TOGGLED):  // if scancode is 'keyIsToggled'... then execute method...
                    if (keyIsToggled(code)){
                        (*action->method)();
                    }
                    break;
                case (IOActionType::HELD):  // if scancode is 'keyIsHeld'... then execute method...
                    if (keyIsHeld(code)){
                        (*action->method)();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    bool IOKeyBoard::runKeyCode(SDL_Keycode _key){
        
        return key_codes[SDL_GetScancodeFromKey(_key)]; 
    }

    bool IOKeyBoard::keyIsPressed(SDL_Scancode _code) {
        return key_codes[_code];
    }

    bool IOKeyBoard::keyIsHeld(SDL_Scancode _code)
    {
        return (
            key_codes[_code] && key_codes[SDL_GetScancodeFromKey(previousKeyPressed)] 
        );
    }

    bool IOKeyBoard::keyIsToggled(SDL_Scancode _code)
    {
        return (
            key_codes[SDL_GetScancodeFromKey(previousKeyPressed)] && !key_codes[_code]
        );
    }
    
};};


namespace Xioudown{ namespace Essentials{

    IODeviceManager::IODeviceManager() {
        m_game_io_devices  = new IOKeyBoard();
        m_io_quit_flag = false;
    }

    IODeviceManager::~IODeviceManager () {
        delete m_game_io_devices;
    }

    void IODeviceManager::awaitIO(){
        /* Checks and processes anything that happens from I/O. */
        // "while" instead of if because of pending events
        while (SDL_PollEvent(&m_io_event_listener)) {
            if (m_io_event_listener.type == SDL_QUIT) {
                m_io_quit_flag = true;
            }
            if (m_game_io_devices->type() == IOType::KEYBOARD) {
                    m_game_io_devices->processIO(m_io_event_listener);
                    break;
            }
        }
    }

    IOKeyBoard *IODeviceManager::keyboard() {
        if (!m_game_io_devices) {
            m_game_io_devices = new IOKeyBoard();
        }
        return this->m_game_io_devices;
    }
};};
