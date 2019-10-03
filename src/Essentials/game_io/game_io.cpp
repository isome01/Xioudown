#include <SDL.h>
#include <stdio.h>
#include "game_io.h"
#include "../../Catalyst/gameCatalyst.h"

namespace Xioudown{ namespace Essentials{

    gameIOController::gameIOController(bool keyboard, bool joystick) : 
        game_keyboard(keyboard), game_joystick(joystick) {
        //set all keycodes to false
        
        for (int i = 0; i < 10000; i++){
            key_codes.push_back(false); //a bit exhaustive an inefficient
        }
        //only one or the other: not neither nor either
        game_keyboard = !(game_joystick);
    }

    gameIOController::~gameIOController(){

    }

    bool gameIOController::runKeyCode(SDL_Keycode k){
        
        return key_codes[ recodeKeycode(k) ]; 
    }

    void gameIOController::awaitIO(){

        //Make sure to set the IO for keyboard

        while (SDL_PollEvent(&gEvent)) {
            //Whenever the player does anything from the menu
            game_keyboard ? keyboard(gEvent) : joystick(gEvent);
        }
    }

    void gameIOController::keyboard(SDL_Event e){

        //because of value struggles on our 'SDLK_UP' SDLK_Scancode_Masks
        int code = recodeKeycode(e.key.keysym.sym);
        switch (e.type) {

            case SDL_QUIT:
                //If the player happens to close out of the game
                //we close out of the game.
                Catalyst::gameCatalyst::gWorld()->s_gameState(false);
                return;
            case SDL_KEYDOWN:
                key_codes[code] = true;
                return;
            case SDL_KEYUP:
                key_codes[code] = false;
                return;
            default:
                return;
        }
    }

    void gameIOController::joystick(SDL_Event e){

    }
};};