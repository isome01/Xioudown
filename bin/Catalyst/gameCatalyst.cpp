#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>

//game engine files
#include "../Essentials/texturemanager.h"
#include "gameCatalyst.h"
#include "../Room/Level/level.h"
#include "../Essentials/player/player.h"
#include "../UI/uiObj/options_box.h"

int Xioudown::Catalyst::gameCatalyst::gwTraversal = 0;
const int SCREEN_WIDTH = Xioudown::Catalyst::gameCatalyst::gWorld()->getWindow()->getWidth();
const int SCREEN_HEIGHT = Xioudown::Catalyst::gameCatalyst::gWorld()->getWindow()->getHeight();

namespace Xioudown { namespace Catalyst{

    #ifndef log
    #define log(x) std::cout << "gameCatalyst: from " << __func__ << " " << x << std::endl;
    #endif
    
    using namespace Essentials;

    gameCatalyst::gameCatalyst()
    {
        //The game is on unless errors or manual loop break dictate otherwise.
        systems_go = initDependence();
        
        if (!systems_go){
            log("Unable to initialize dependencies.");
        } else {

            //Create game window 
            game_window = new gameWindow((char*)"Testing", 1280, 960);
            systems_go = game_window->getStatus();

            if (!systems_go){
                log("Unable to create window.")
            } else {

                //Initialize Level variables
                log("Created window.")

                this->Levels = NULL;
                this->test_room = NULL;

                //Loading the textures into our texture manager.
                texture_manager = new TextureManager(); //allocate memory for texture manager
            }
        }
    }

    bool gameCatalyst::initDependence(){
        
        log("Initializing SDL.")
        int pngflag = IMG_INIT_PNG, jpgflag = IMG_INIT_JPG;

        if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
            log(SDL_GetError())
            return false;
        } if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
            log(SDL_GetError() )
            return false;
        } if (  !(IMG_Init(pngflag) & pngflag) || !(IMG_Init(jpgflag) & jpgflag ) ){
            log(IMG_GetError() )
            return false;
        } if ( !TTF_Init() < 0) {
            printf("Error %s", TTF_GetError());
            return false;
        }
        return true;
    }

    bool gameCatalyst::finalize(){

        bool tis_final = false;

        tis_final = systems_go;
        connectLevelEntries();
        scrollToPlayer(); //scroll everything to center player

        log("Scrolls to player.")
        gameState = true; //set gamestate = true
        
        return tis_final;
    }

    gameCatalyst::~gameCatalyst(){

        //delete texture_manager;
        delete game_window;

        /* this is an object of non-polymorphic class: deleting this might cause undefined behavior
        delete test_room;*/

        if (Levels){
            if (level_count > 1)
                delete [] Levels;
            delete Levels;
        }

        if (test_room)
            delete test_room;

        SDL_Quit();
        IMG_Quit();
        TTF_Quit();
    }


    void gameCatalyst::resetRandom(){

        //We'll add more to this

        clock_t start_time = clock();
        while(1){

            clock_t elapsed_time = clock();

            if (elapsed_time - start_time >= 1000)
                break;
        }

        //renew the seed.
        SDL_GetTicks();

        srand(time(NULL));

        int x = rand()%10 + 1;
    }

    int gameCatalyst::gwTask_Handle_Rooms(void *obj){

        //busy-waiting : We had to create a mutex for the processes to not run into each other.
        while(1){
            //log("waiting to process");
            if (gwTraversal == 0){
                gwTraversal = 1;
                break;
            }

        }
        ((Room::Room*)obj)->processRoomEvents();
        gwTraversal = 0;
        return 1;
    }

    void gameCatalyst::Process_Level(){

        //Here we will attempt to process every room event, given child processes.
        //They will return 0 upon completion.
        // ~ Child processes will only happen for rooms not rendered - in which we will first instantiate before processing current room ~

        int sz = gRegistry[current_lvl].size();
        SDL_Thread **gwProcRoomTask = new SDL_Thread*[sz];

        int gwRoomstatus;
        for (int rms = 0; rms < sz; rms++){

            if (rms == current_room)
                continue;
            gwProcRoomTask[rms] = SDL_CreateThread( gwTask_Handle_Rooms, "Process for other rooms", gRegistry[current_lvl][rms]);
            //log("Process for level " << lvls << " room: " << rms)
            //SDL_DetachThread(gwProcRoomTask[rms]);
            SDL_WaitThread(gwProcRoomTask[rms], &gwRoomstatus);

            if (gwRoomstatus < 0){
                //log("Room process " << rms);
                // /log(": status is " << gwRoomstatus << ".");
            }
        }
    }

    //This is where we process whatever [events] happens in the game
    void gameCatalyst::processEvents(){


        //Processing everything that is going on inside the room
        this->Process_Level();
        //printf("processes events...\n");
        getCurrentLevel()->processRoomEvents();
        //printf("processes events...\n");
        //Despite menu and utility options, the player always has the last move
        player->processPlayerEvents();
        //printf("processes events...\n");
        //Change rooms whenever prompted
        changeRooms();
        //printf("processes events...\n");
    }

    void gameCatalyst::renderEverything(){

        //Clear the screen each time.
        SDL_RenderClear(game_window->getRenderer());

        //Rendering the game background
        SDL_SetRenderDrawColor(game_window->getRenderer(), 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(game_window->getRenderer()); //We clear the screen of anything previously drawn

        //Rendering everything in the entire room - as the room will call the render functions for all
        //tangible objects - excluding the player.

        getCurrentLevel()->renderRoom(game_window->getRenderer());

        //log("Rendered level")

        //This is where we render all of our player's significances [last]
        if (player)
            player->renderPlayerAttributes(game_window->getRenderer());

        //Then we show everything we attempted to render
        SDL_RenderPresent(game_window->getRenderer());

        //log("Rendered everything")

        return;
    }

    //This is where we scroll everything opposite to the player -> if they have taken steps
    void gameCatalyst::xyScroll(int x_steps, int y_steps, Room::Room *room){

        Direction dir = player->getAnimatedObj()->getDirection();

        //Here we will scroll the current room and its' attributes
        room->xyScroll(x_steps | y_steps, dir);

        //Depending on the direction, we scroll everything in the room opposite to the player, one by one.
        AnimatedObj *player_obj = gWorld()->player->getAnimatedObj();//Player
        if ( dir == UP || dir == DOWN){
            player_obj->setY( player_obj->getY() + y_steps );
        } else if (dir == LEFT || dir == RIGHT){
            player_obj->setX( player_obj->getX() + x_steps );
        }
    }

    //This function operates to center the player
    void gameCatalyst::scrollToPlayer(){

        gwCoordinates p;
        p.x = player->getAnimatedObj()->getX();
        p.y = player->getAnimatedObj()->getY();
        AnimatedObj *plyr = player->getAnimatedObj();

        int d_x = abs( (SCREEN_WIDTH / 2) - p.x );
        int d_y = abs( (SCREEN_HEIGHT / 2) - p.y );

        int centerX = (SCREEN_WIDTH / 2), centerY = (SCREEN_HEIGHT / 2);

        //for both x...
        if (p.x < centerX ) {
            this->getCurrentLevel()->xyScroll( d_x, RIGHT);
            plyr->setX( d_x + p.x );

        } else if(p.x > centerX) {
            this->getCurrentLevel()->xyScroll( -1 * (d_x), LEFT);
            plyr->setX( -1 * (d_x) + p.x);
        }


        //and y: of everything in the room.
        if (p.y < centerY) {
            this->getCurrentLevel()->xyScroll( (d_y), DOWN);
            plyr->setY( (d_y) + p.y );

        } else if(p.y > centerY) {
            this->getCurrentLevel()->xyScroll(-1 * (d_y), UP);
            plyr->setY( -1 * (d_y) + p.y  );
        }
    }

    void gameCatalyst::traverseRooms(int room, int level){

        log("COMplayer traversing room")
        //add object to other room and delete object from current room
        AnimatedObj *a = NULL;
        if (queued_aobj->isCOMPlayer()){

            Room::Room* r = gRegistry[current_lvl][current_room];
            int obj_num = queued_aobj->getObjNum();
            animateObj obj_type = queued_aobj->getType();

            //delete the previous object
            for (int i = 0, a_count = r->g_aObjCount(); i < a_count; i++ ){

                if (r->getAnimatedObject(i)->getObjNum() == obj_num){
                    a = createDeepCopy(r->getAnimatedObject(i)); //createDeepCopy(a);

                    if (a){
                        r->destroyAvatar(i);
                        log("destroys avatar")
                    }
                }
            }

            if (!a){
                log("Object not copied. There was an error.")
            } else {
                log("about to set xyplace")
                a->setX(queued_entry->get_xplace());
                a->setY(queued_entry->get_yplace());
                log("Sets xyplace")

                log("about to access next room")
                r = gRegistry[level][room];

                log("about to add avatar")
                queued_aobj = a;
                r->addAvatar(queued_aobj);
                log("traversed room")
            }
        }
    }

    //Just a basic getter and setter for the rooms
    void gameCatalyst::queueTransition(AnimatedObj *a, Entry* e, int room = -1, int level = -1){

        //if the parameters were correctly entered.
        if (room >= 0 && level >= 0){

            //if the room exists and object is the player - for now
            if (gRegistry[level][room] != NULL){
                //set up for queue transition
                roomChange = true;

                rm_change.x = room;
                rm_change.y = level;
                log("Traversing to room: " << room << " and level: " << level)

                //Queue the animated object for room traversal
                queued_entry = e;
                queued_aobj = &(*a);

            } else {
                log("Invalid room or level")
            }

        } else {

            log("Invalid paramters: room: " << room << " level: " << level)
        }
    }

    void gameCatalyst::changeRooms(){

        if (roomChange){

            roomChange = false; //reset the roomChange flag

            //log("Current room: " << rm_change.x << "\nCurrent level: " << rm_change.y)

            //if the player enters the room, then there is transition between rooms.
            if (!queued_aobj->isCOMPlayer()){
                queued_aobj->setX(queued_entry->get_xplace());
                queued_aobj->setY(queued_entry->get_yplace());
                current_room = rm_change.x;
                current_lvl = rm_change.y;

                //then scroll to player
                log("Gets to player scroll method")
                scrollToPlayer();
                log("passes!")
            } else {
                traverseRooms(rm_change.x, rm_change.y); //if it's just a COM then we add it to the other room
                log(gRegistry[current_lvl][current_room]->g_aObjCount());
            }
            //reset the room change
            rm_change.x = 0;
            rm_change.y = 0;
        }

        else return;
    }


    void gameCatalyst::connectLevelEntries(){

        //This is where we loop through each of the levels to concatenate their entries

        int sz = gRegistry.size();
        for (int i = 0; i < sz - 1; i++){

            Entry *a = NULL;
            Entry *b = NULL;

            if (i == 0){

                a = gRegistry[0][0]->getStartEnd(0);
                b = gRegistry[1][0]->getStartEnd(0);

            } else {
                a = gRegistry[i][0]->getStartEnd(1);
                b = gRegistry[i + 1][0]->getStartEnd(0);
            }

            if (a && b){
                a->setAdjacent(b);
                b->setAdjacent(a);
            }
        }
    }

    //returns a fresh monster
    AnimatedObj *gameCatalyst::MonsterDice(animateObj type = NONE, bool COM = true){

        AnimatedObj *a = NULL;
        int _type;

        if (type == NONE){
            _type = rand()%1 + 1;
           // log("The number is " << _type)
        } else if (type == KNIGHT){
            _type = 0;
        } else if (type == NORMAL_MAN){
            _type = 1;
        }


        switch(_type){
            case (0):
                a = new Knight(COM);
            case (1):
                a = new NormalMan(COM);
        }

        return a;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Deep copy methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    AnimatedObj *gameCatalyst::createDeepCopy(AnimatedObj *a = NULL){

        if (a){

            AnimatedObj *b = MonsterDice(a->getType(), a->isCOMPlayer());

            a->setAxis(b->getAxis());
            a->setDirection(b->getDirection());

            //Setter and getters for Animate Object stats
            b->setObjNum(a->getObjNum());
            b->setAtk(a->getAtk());
            b->setDef(a->getDef());
            b->setSpd(a->getSpd());
            b->setHp(a->getHp());
            b->setHpLimit(a->getHpLimit());
            b->setLevel(a->getLevel());
            b->setStepCount(a->getStepCount());
            b->setStepTotal(a->getStepTotal());
            b->Xsteps(a->Xsteps());
            b->Ysteps(a->Ysteps());

            b->setAgro(a->getAgro());
            b->willAttack(false); //we dont want an attack to happen as soon as a deep copy happens (for traversals at least)
            b->setSpawned(a->isSpawned());

            return b;
        }

        return a;
    }

    InanimateObj *gameCatalyst::createDeepCopy(InanimateObj *ia = NULL){

        InanimateObj *b = ia;

        return b;
    }

};};