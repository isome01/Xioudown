#pragma once
#include <SDL.h>
#include <map>
#include <vector>

//This class is used to define the player's device for i/o 
namespace Astral{ namespace Essentials{

    class gameIOController{
        private:
        
            SDL_Event* m_event; //To get whenever the player does something
            std::vector<bool> key_codes;
            bool game_keyboard; // For the amount of different keycodes used on a keyboard (whether popular or not)
            bool game_joystick;
            //Array of default keycodes
            //mouse IO
            //Xbox controller

        public:
            gameIOController(bool keyboard = true, bool joystick = false);
            ~gameIOController();
            void awaitIO(); //basically a function that yields a while loop
            void keyboard(SDL_Event e);
            void joystick(SDL_Event e);
            int recodeKeycode(Uint32 n){ return (int)(( n >= 1073741824) ? (n - 1073741824) + 5000 : n) ; }

        //Getter for the keyboard state
        private:
            SDL_Event gEvent; //This is our event "listener": whenever something happens, this will turn to NULL
            const Uint8 *m_kbState; //The keyboard and all of it's scan_codes; the precise keyboard codes imported from keyboard.
            bool game_key_pressed, game_key_held;
        
        public:
            //getters and setters
            const Uint8 *getkbState(){ return m_kbState; } //return const keyboard state/scancodes
            SDL_Event *getListener(){ return &gEvent; }

            bool runKeyCode(SDL_Keycode k);
    };
}; };