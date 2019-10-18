#include <iostream>
#include <SDL.h>
#include "player.h"

#include "../../Catalyst/gameCatalyst.h"
#include "../../ObjAction/game_actions.h"
#include "../../Room/room.h"

#ifndef log
#define log(x) std::cout << "Player obj: " << x << std::endl;
#endif

namespace Xioudown{ namespace Essentials{

    Player::Player(){
        playerObj = new TangObj::Knight(false);
        playerObj->setAgro(true);

        //for now (need to implement a load game function later)
        //initialize player start coordinates
        gwCoordinates start = Catalyst::gameCatalyst::gWorld()->getStart();
        playerObj->getBoundaries()->x = start.x;
        playerObj->getBoundaries()->y = start.y;
        playerObj->setDirection(DOWN);
        playerObj->setObjNum(-1);
        setRoom(1);

        //Initializing player's inventory assets
        player_inv = new Container::Inventory("Player 1");
        player_eqp = new Container::Item_Equip();

        //initialize player IO
        this->p_kb_controller = new gameIOController(true, false);
        this->p_menu_screen = new UI::interfaceManager();

        //lambda function calls to gameIOcontroller

        playerObj->setSpawned(true);
    }

    Player::~Player(){

        player_inv->~Inventory();
        p_kb_controller->~gameIOController();
    }

    //Add to player's keyboard registry
    void Player::addtoKBRegistry(SDL_Keycode k, bool (*f)(SDL_Event*)){

        //properly inserting a keyboard pair.
        kb_action_registry.insert( std::pair<SDL_Keycode, bool (*)(SDL_Event*)>(k, f) );

        [](SDL_Event*e){
            
            return (e->type == SDL_RELEASED );
                
        };

        return;
    }

    //get from keyboard registry
    bool Player::getKBRegistry(SDL_Keycode k){
        
        int code = p_kb_controller->recodeKeycode(k); //recoding...
        SDL_Event *e = p_kb_controller->getListener(); //retrieve the keyboard event

        //Execute the function pointer based on the keycode given.
        return ( kb_action_registry[ code ](e) );
    }


    //Processing whatever the player does
    void Player::processPlayerEvents(){
        

        //process any external methods before anything else.
        if (!player_assets.empty()){
            
            int sz = player_assets.size();
            for (int i = 0; i < sz; i++){
                player_assets[i]();
            }
        }

        //This is where we process the player's events
        p_kb_controller->awaitIO();
        //const Uint8 *state = p_kb_controller->getkbState();

        playerMenuActions();
        playerAvatarActions();
    }

    void Player::playerAvatarActions(){
        //whenever the player's option is to move, and they decide to move, we increase the movement speed based on how fast the player's
        //human object is.
        Room::Room *room = Catalyst::gameCatalyst::gWorld()->getCurrentLevel();

        //Player's movement~~~~

        //~NOTE: Will have to make an array of functions upon initialization instead of directly calling upon gamestate
        if (p_kb_controller->runKeyCode(SDLK_w) ){
            playerObj->moveObj(UP, room);
            Catalyst::gameCatalyst::gWorld()->xyScroll(playerObj->Xsteps(),playerObj->Ysteps(),room);
        }

        if (p_kb_controller->runKeyCode(SDLK_a) ){
            playerObj->moveObj(LEFT, room);
            Catalyst::gameCatalyst::gWorld()->xyScroll(playerObj->Xsteps(),playerObj->Ysteps(),room);
        }

        if (p_kb_controller->runKeyCode(SDLK_s)){
            playerObj->moveObj(DOWN, room);
            //Now that the player has moved, we scroll everything relative to the player
            Catalyst::gameCatalyst::gWorld()->xyScroll(playerObj->Xsteps(),playerObj->Ysteps(),room);
        }
        if (p_kb_controller->runKeyCode(SDLK_d)){
            playerObj->moveObj(RIGHT, room);
            Catalyst::gameCatalyst::gWorld()->xyScroll(playerObj->Xsteps(),playerObj->Ysteps(),room);
        }

        //This is how the player will attack
        if (p_kb_controller->runKeyCode(SDLK_SPACE)){
            if (!this->player_eqp->is_item_equipped()){
                //We will send an attack action to the room action-handler methods

                //first we will start the cooldown timer
                ObjAction::Action* a = new ObjAction::MeleeAction(this->playerObj);
                this->playerObj->willAttack(a->getCooldown());

                if(this->playerObj->willAttack())//if the cooldown time has run down then we are able to create another action
                    Catalyst::gameCatalyst::gWorld()->getCurrentLevel()->addAction( a );
                else delete a;
            }
        }

        //If the player decides to open their inventory
        if ( p_kb_controller->runKeyCode(SDLK_TAB)){

            this->player_inv->setInvDisplay(true);
        }
    }

    void Player::playerMenuActions(){
        
        if (p_menu_screen && p_menu_screen->isUIActive() ){
            
            p_menu_screen->handleUIEvents(p_kb_controller);

        } 

        if (p_kb_controller->runKeyCode(SDLK_KP_ENTER)){
            p_menu_screen->setUIToggle( !(p_menu_screen->isUIActive()) );
        }
    }
    
    void Player::renderPlayerAttributes(SDL_Renderer* r){

        p_menu_screen->renderCurrentInterface(r);

        playerObj->renderAnimObj(r);

        //We will also find some space to render the stats table that can be shown on the screen
        //More coming soon
        player_inv->renderInventory(r);
    
    }
};};