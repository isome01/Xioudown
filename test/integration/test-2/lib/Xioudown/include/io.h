#pragma once
#include "xioudown_predefs.h"
#include "utils.h"

#include <SDL.h>
#include <map>
#include <vector>
#include <functional>


namespace Xioudown { namespace Essentials {
    typedef std::function<void(void)> extXioudownMethod;
};};

//This class is used to define the player's device for i/o 
namespace Xioudown { namespace Essentials {

    enum IOType {
        KEYBOARD  // with this value one could 
    };

    enum IOActionType {
        PRESSED = 0,  // KEY IS PRESSED
        TOGGLED = 1, // KEY IS TOGGLED
        HELD = 2  // KEY IS HELD
    };

    const int DEFAULT_IO_CODE_RANGE = 65536;  // max SDL_SCANCODE range 
    const IOType DEFAULT_GAME_IO_CONTROLLER = IOType::KEYBOARD;


    struct IODeviceAction {
        IOActionType action;
        void(*method)();
        std::function<void(void)> &ext_method;
    };

    struct IODeviceObjRefAction {
        IOActionType action;
        void(*method)();
        std::function<void(void)> &_method;
    };  


    class IODevice : public XioudownClass {
        Uint64 m_io_id;
        IOType m_game_io_type;
        std::map<SDL_Scancode, IODeviceAction*> m_device_methods;

        public:
            IODevice(IOType _type = DEFAULT_GAME_IO_CONTROLLER);
            ~IODevice();
            Uint64 id() const { return m_io_id; };
            IOType type() const { return m_game_io_type; }
            virtual void processIO(SDL_Event e) = 0;
            std::map<SDL_Scancode, IODeviceAction*>* getKeyMapping() { return &m_device_methods; }
        
        protected:
            void assignActionMethodPair(SDL_Scancode _code, IOActionType _type, void(*&_method)());
            void assignActionMethodPair(SDL_Scancode _code, IOActionType _type, extXioudownMethod &_method);
            void clearPairAssignments(){ m_device_methods.clear(); }
    };

    class IOKeyBoard : public IODevice {
        private:
            SDL_Keycode previousKeyPressed;
            std::vector<bool> key_codes; //a bit exhaustive an inefficient but this represents all possible values of "key codes" SDL uses to record input.
        public:
            IOKeyBoard();
            ~IOKeyBoard();
            void processIO(SDL_Event e);
            int recodeKeycode(Uint32);
            bool runKeyCode(SDL_Keycode k);
            void addKeyFunctionality();  // add a key pair to the keyboard registry of <SDL_SCANCODE, Function> to have a key bound to function
            bool keyIsPressed(SDL_Scancode _code);
            bool keyIsToggled(SDL_Scancode _code);
            bool keyIsHeld(SDL_Scancode _code);
        
        public:
            void addKeyEvent(KEYBOARD_KEY _key, IOActionType _type, void(*&f)()); // adds a method for any provided keyboard key and the action performed on it.
            void addKeyEvent(KEYBOARD_KEY _key, IOActionType _type, extXioudownMethod &f); // the same as the other if done from a class instance.
            void initiateKeyboardEvents();
    };

    class IOJoystick : public XioudownClass  {
        public:
            IOJoystick();
        /* basic joystick class: services for fightsticks, Xbox, PS controllers */
    };

    class IODeviceManager {
        private:
            IOKeyBoard *m_game_io_devices;
            bool m_io_quit_flag;
            
            //Array of default keycodes
            //mouse IO
            //Xbox controller

        public:
            IODeviceManager();
            ~IODeviceManager();
            void awaitIO(); //basically a function that yields a while loop

        //Getter for the keyboard state
        private:
            SDL_Event m_io_event_listener; //This is our event "listener": whenever something happens.
            const Uint8 *m_kbState; //The keyboard and all of it's scan_codes; the precise keyboard codes imported from keyboard.
            bool game_key_pressed, game_key_held;
        
        public:
            //getters and setters
            const Uint8 *getkbState(){ return m_kbState; } //return const keyboard state/scancodes
            SDL_Event *getListener(){ return &m_io_event_listener; }
            IOKeyBoard *keyboard();
            void keyboard(IOKeyBoard *k);
            bool ioQuit() const { return m_io_quit_flag; }
            void ioQuit(bool f) { m_io_quit_flag = f; }
            bool runKeyCode(SDL_Keycode k) { return m_game_io_devices->runKeyCode(k); }
    };
}; };

#define XIOUDOWN_GAME_IO
