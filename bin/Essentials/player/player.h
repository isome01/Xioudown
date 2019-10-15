#pragma once
#include <SDL.h>
#include "../game_io/game_io.h"
#include "../../Container/storage.h"
#include "../../TangObj/Animated_Obj/animatedObj.h"
#include "../../UI/interfaceManager/interfaceManager.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Player Object ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

namespace Xioudown{ namespace Essentials{
    class Player{
    private:
        //Player avatar
        TangObj::AnimatedObj *playerObj;

        //Inventory pointers
        Container::Inventory *player_inv;
        Container::Item_Equip *player_eqp;

        //Keyboard registry for keyboard events:
        //a key registry specific to how a key is being pressed: might have to make this a struct
        std::map<SDL_Keycode, bool(*)(SDL_Event*) > kb_action_registry;

        std::vector< void(*)()> player_assets;

    public:
        Player();
        ~Player();
        TangObj::AnimatedObj *getAnimatedObj() { return playerObj; }
        void processPlayerEvents(); //This is where the player gets a say so on what they do.
        void renderPlayerAttributes(SDL_Renderer *r); //This is where we draw the player's sprite, health bar, etc.}

        void playerAvatarActions(); //method for ingame avatar I/O
        void playerMenuActions(); //method for ingame menu I/O
    
        private:
            int m_currentRoom; //current offset of the room the player is in.
            int m_stepsTaken; //The count of the steps taken in regards to scrolling the screen.

            //player I/O
            gameIOController *p_kb_controller;
            //player UI
            UI::interfaceManager *p_menu_screen;
        
        public:
            //A few getter and setters for the player class
            void setRoom(int n){ m_currentRoom = n; }
            int getRoom()const{ return m_currentRoom; }
            void setSteps(int n){ m_stepsTaken = n; };
            int getSteps()const{ return m_stepsTaken; } 
            gameIOController *getIOState(){ return p_kb_controller; }
            UI::interfaceManager *getMenuScreen(){ return p_menu_screen; }

            bool getKBRegistry(SDL_Keycode k);
            void addtoKBRegistry(SDL_Keycode k, bool(*f)(SDL_Event*));

            //custom methods used for the player to execute before rendering.
            void addPlayerAssets( void(*f)() ){ player_assets.push_back(f); }
    };
};};