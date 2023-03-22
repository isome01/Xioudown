#include <SDL.h>
#include <stdio.h>
#include "io.h"
#include "utils.h"
#include <string>


namespace Xioudown{ namespace Essentials{
    
    IODevice::IODevice(IOType _type) : m_game_io_type(_type) {

        m_io_id = Math::generateReferenceId<IODevice*>(this);
    }

    IODevice::~IODevice() {
        
    }

    void IODevice::assignActionMethodPair(SDL_Scancode _code, IOActionType _type, void(*&_method)()) {
        /* Assign key pair of code to device action */
        extXioudownMethod DEFAULT_EXT_METHOD = std::bind([](){});
        m_device_methods[_code] = new IODeviceAction({_type, _method, DEFAULT_EXT_METHOD});
    }

    void IODevice::assignActionMethodPair(SDL_Scancode _code, IOActionType _type, extXioudownMethod &_method) {
        /* Assign key pair of code to device action */
        void(*DEFAULT_METHOD_PTR)() = [](){};
        m_device_methods[_code] = new IODeviceAction({_type, DEFAULT_METHOD_PTR, _method});
    }
};};

namespace Xioudown { namespace Essentials {
    /* Keyboard implementation*/

    IOKeyBoard::IOKeyBoard() : IODevice(IOType::KEYBOARD) {
        //set all keycodes to false
        
        for (int i = 0; i < DEFAULT_IO_CODE_RANGE; i++){
            key_codes.push_back(false);
        }
        currentKey = previousKey = SDLK_UNKNOWN; 
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
                currentKey = e.key.keysym.sym;
                if (currentKey != SDLK_UNKNOWN){
                    previousKey = currentKey;
                } 
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

    void IOKeyBoard::addKeyEvent(KEYBOARD_KEY _key, IOActionType _type, void(*&f)()){  // adds a method for any provided keyboard key and the action performed on it.
        // NOTE: make key lowercase if needed

        this->assignActionMethodPair(
            SDL_GetScancodeFromKey(_key), _type, f
        );
    }

    void IOKeyBoard::addKeyEvent(KEYBOARD_KEY _key, IOActionType _type, extXioudownMethod &f){  // adds a method for any provided keyboard key and the action performed on it.
        // NOTE: make key lowercase if needed

        this->assignActionMethodPair(
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
                        (action->ext_method)();
                    }
                    break;
                case (IOActionType::TOGGLED):  // if scancode is 'keyIsToggled'... then execute method...
                    if (keyIsToggled(code)){
                        (*action->method)();
                        (action->ext_method)();
                    }
                    break;
                case (IOActionType::HELD):  // if scancode is 'keyIsHeld'... then execute method...
                    if (keyIsHeld(code)){
                        (*action->method)();
                        (action->ext_method)();
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

    bool IOKeyBoard::keyIsPressed(SDL_Scancode _code)
    {
        return key_codes[_code];
    }

    bool IOKeyBoard::keyIsHeld(SDL_Scancode _code)
    {
        return (
            key_codes[_code] && key_codes[SDL_GetScancodeFromKey(currentKey)] && key_codes[SDL_GetScancodeFromKey(previousKey)] 
        );
    }

    bool IOKeyBoard::keyIsToggled(SDL_Scancode _code)
    {
        return (
            key_codes[SDL_GetScancodeFromKey(previousKey)] && !key_codes[_code]
        );
    }

    bool IOKeyBoard::keyIsPressed(KEYBOARD_KEY _key)
    {
        return runKeyCode(_key);
    }

    bool IOKeyBoard::keyIsToggled(KEYBOARD_KEY _key)
    {
        return (
            runKeyCode(_key) && runKeyCode(previousKey)
        );
    }

    bool IOKeyBoard::keyIsHeld(KEYBOARD_KEY _key)
    {
        return (
            runKeyCode(previousKey) && !runKeyCode(_key)
        );
    }

    void IOKeyBoard::pressedKey(KEYBOARD_KEY _key, std::function<void(void)> &callback)
    {
        addKeyEvent(_key, IOActionType::PRESSED, callback);
    }

    void IOKeyBoard::toggledKey(KEYBOARD_KEY _key, std::function<void(void)> &callback)
    {
        addKeyEvent(_key, IOActionType::TOGGLED, callback);
    }

    void IOKeyBoard::heldKey(KEYBOARD_KEY _key, std::function<void(void)> &callback)
    {    
        addKeyEvent(_key, IOActionType::HELD, callback);
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
            ioQuit(m_io_event_listener.type == SDL_QUIT);

            if (ioQuit() == true) {
                break;
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
        return m_game_io_devices;
    }

    void IODeviceManager::keyboard(IOKeyBoard *k) {
        m_game_io_devices = k;
    }
};};
