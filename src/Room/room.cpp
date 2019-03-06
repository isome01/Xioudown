#include <iostream>
#include <SDL.h>
#include <ctime>

#include "../Catalyst/gameCatalyst.h"
#include "../Catalyst/gameWindow.h"
#include "../Essentials/timer.h"
#include "../Math/methods.h"
#include "../ObjAction/game_actions.h"
#include "../TangObj/tangibleObj.h"
#include "room.h"
#include <vector>

using std::vector;

#define log(x) std::cout <<"Room Obj: from "<< __func__ << " " <<  x << std::endl;

namespace Astral{ namespace Room{

    using namespace Catalyst;
    using namespace Essentials;
    using namespace ObjAction;


    double Room::Register_Number = 0;
    double Room::RegisterObj(){

        Register_Number++;
        return Register_Number;
    }

    //Upon creation we will generate the enemies
    Room::Room(vector<Pathway*> paths, int n) : zones(NULL), a_obj_limit(0)
    {
        //log("Enters room instantiation " << paths.size())

        //Initializing the number of rooms and offset
        if (n){
            int r_rooms = rand()% n/2;
            numOfRooms = (r_rooms > n)?(n):(r_rooms); //random room generator
        } else numOfRooms = n;

        //resolving the main room point - if specified
        if (!paths.empty()){
            //Creating the boundary lines of our room - based on the paths max y + h and x + w
            roomBorder.w = 0;
            roomBorder.h = 0;
            int sz = paths.size();
            for (int i = 0; i < sz; i++){
                SDL_Rect p = paths[i]->getWalkway();

                if (i == 0){
                    roomBorder.x = p.x;
                    roomBorder.y = p.y;
                }

                if (roomBorder.w < p.x + p.w)
                    roomBorder.w = p.x + p.w;

                if (roomBorder.h < p.y + p.h)
                    roomBorder.h = p.y + p.h;
            }

            //log("Width: " << roomBorder.w << "  Height: " << roomBorder.h)

            //Create the zone for spawning
            zone_count = 1;
            spaces = new InhabitableSpace*;
            spaces[0] = new InhabitableSpace( this->roomBorder );
            zones = NULL;

        } else { // otherwise we just construct a default
            //log("Got default")
            zone_count =0;
            roomBorder.x = 0;
            roomBorder.y = 0;
            roomBorder.w = gameCatalyst::gWorld()->getWindow()->getWidth();
            roomBorder.h = gameCatalyst::gWorld()->getWindow()->getHeight();
            spaces = NULL;
            zones = NULL;
        }

        spawn_timer = new Timer(); //Creating a new timer for our monster generator

        a_obj_limit = 1;
        //Our avatar factorization: This is where we will instantiate our enemies into existence!
        // ~ Coming soon
    }

    Room::~Room(){
        delete [] spaces;
        delete [] zones;
    }

    void Room::GenerateZones(std::vector<Pathway> paths){

        spaces = new InhabitableSpace*[paths.size()];

        zone_count = paths.size();
        for (int i = 0; i < zone_count; i++){
            spaces[i] = new InhabitableSpace( paths[i].getWalkway() );
        }
    }

    //This is what happens when an object encounters the room's boundaries...
    void Room::Collision(TangibleObj* obj, Direction dir, int steps){
        //We set the object at the same amount of steps the object has taken
        if (dir == DOWN){
            obj->setY((-1 * steps) + obj->getY());
        } else if (dir == RIGHT){
            obj->setX((-1 * steps) + obj->getX());
        } else if (dir == LEFT){
            obj->setX(steps + obj->getX());
        } else if (dir == UP){
            obj->setY(steps + obj->getY());
        } else {
            log("Invalid direction")
        }
    }

    void Room::xyScroll(int steps, Direction dir){


        int a_sz = avatars.size(), ia1_sz = structures.size(), ia2_sz = items.size();

        //log("Structure count: " << ia1_sz << "\nItem count: " << ia2_sz)
        //Transition everything in the room by x steps

        if ( dir == UP || dir == DOWN){

            //moving all animated objects
            for (int i = 0; i < a_sz; i++){
                AnimatedObj *a = avatars[i];
                if (a)
                    a->setY( a->getY() + steps);
                else log("Cannot scroll undefined animated object")
            }
            //same for structures
            for (int i = 0; i < ia1_sz; i++){
                InanimateObj *ia = structures[i];
                ia->setY( ia->getY() + steps );
            }
            //same for items on the ground
            for (int i = 0; i < ia2_sz; i++){
                InanimateObj *ia = items[i];
                ia->setY( ia->getY() + steps );
            }
            if (spaces != NULL){
                for (int i = 0; i < this->getZoneCount(); i++ ){
                    SDL_Rect *stuffs = this->getSpace(i);
                    if (stuffs) stuffs->y += steps;
                }
            }
        }
        //X - Direction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (dir == LEFT || dir == RIGHT){
            for (int i = 0; i < a_sz; i++){
                AnimatedObj *a = avatars[i];
                if (a)
                    a->setX( a->getX() + steps);
                else log("Cannot scroll undefined animated object")
            }

            for (int i = 0; i < ia1_sz; i++){
                InanimateObj *ia = structures[i];
                ia->setX( ia->getX() + steps );
            }
            for (int i = 0; i < ia2_sz; i++){
                InanimateObj *ia = items[i];
                ia->setX( ia->getX() + steps );
            }

            //scrolling inhabitable zone/spaces if they exist
            if (spaces){
                for (int i = 0; i < this->getZoneCount(); i++ ) {
                    SDL_Rect *stuffs = this->getSpace(i);
                    if (stuffs) stuffs->x += steps;
                }
            }
        }
    }

    void Room::renderRoom(SDL_Renderer *r)
    {
        //Drawing the boundary line of the room
        /*if ( GameWorld::gWorld()->aptToRender(roomBorder) ){
            SDL_SetRenderDrawColor(r, 0x00, 255, 128, 0xff);
            SDL_RenderDrawRect(r, &roomBorder);
        }*/

        if (zone_count){
            for (int index = 0; index < this->zone_count; index++){
                    SDL_Rect *stuffs = this->getSpace(index);
                    if (stuffs && gameCatalyst::gWorld()->getWindow()->aptToRender(*stuffs)){
                        SDL_RenderDrawRect(r, stuffs);
                    }
            }
        }

        //Draw all items on the floor
        if (!this->items.empty()){
            int sz = structures.size();
            for (int i = 0; i < sz; i++)
                if (gameCatalyst::gWorld()->getWindow()->aptToRender( items[i]->getPBoundaries() ))
                    items[i]->renderInanimateObj(r);
        }

        //Draw all structures
        if (!this->structures.empty() ){
            int sz = structures.size();
            for (int i = 0; i < sz; i++){

                if (structures[i]->getType() == ENTRY){
                    //log("\nEntry X: " << structures[i]->getBoundaries()->x << "\nEntry Y:" << structures[i]->getBoundaries()->y)
                    SDL_SetRenderDrawColor(r, 0, 0xff, 0, 0xff);
                    SDL_RenderFillRect(r, structures[i]->getBoundaries());
                }

                if (structures[i]->getPBoundaries()){
                    if (gameCatalyst::gWorld()->getWindow()->aptToRender( structures[i]->getPBoundaries() ))
                        structures[i]->renderInanimateObj(r);
                } else if(structures[i]->getBoundaries()){
                    structures[i]->renderInanimateObj(r);
                    //log(structures[i]->getBoundaries()->w)
                }
            }
        }

        //Then we draw all of our animated objects
        if(!avatars.empty())
        {
            for (int index = 0; index < int(avatars.size()); index++)
            {
                if (avatars[index] && gameCatalyst::gWorld()->getWindow()->aptToRender( *avatars[index]->getBoundaries() ))
                    avatars[index]->renderAnimObj(r);
                else continue;
            }
        }

        //Then render all AOE actions within the room!
        RenderRoomActions(r);
    }

    //This where we will process events in the current room (aging resources, COM player moves, etc.)
    void Room::processRoomEvents()
    {
        //this is where we check to see if we should add a new monster
        this->Handle_Monsters();

        //Processing animated object decisions: could be agro or idle characters
        if (!avatars.empty()){
            for (int n = 0; n < int(avatars.size()); n++)
            {
                if (avatars[n]){
                    if (avatars[n]->isObjAlive()){ //check to see if avatar is alive
                        avatars[n]->processDecision(this);
                    } else destroyAvatar(n); //if not then we remove it from rendering.
                } else continue;
            }
        }


        //Processing idle object events: if items left on the floor need to be deleted.

        //~~~~~~~~~~~~~~~~ Coming soon! ~~~~~~~~~~~~~~~~~~~~~~~~

        //Process AOE room actions
        int sz = this->room_actions.size();
        for (int i = 0; i < sz; i++){
            this->QueryRoomActions();
            this->ExecuteRoomActions();
        }
    }

    //This method takes an "isTouching" function that processes through each obj in the room
    bool Room::processCollisions( AnimatedObj *a ){

        //the player's animated obj will be in for reference
        AnimatedObj *p = gameCatalyst::gWorld()->getPlayer()->getAnimatedObj();

        //Now we see if the object is colliding with any of the animated objects within the game, if any animated objects.
        int a_sz = this->avatars.size();
        for (int i = 0; i < a_sz; i++){
            //If the indexed object is pointing to a NULL reference for some reason
            if (!this->getAnimatedObject(i) ) continue;

            else if (a->getObjNum() == i){ //We don't care if an object is touching itself, so we'll just make space
                                            //to see if an object is touching the player instead.
                if (a->isCOMPlayer())
                    if (a->isTouching(*p->getBoundaries())){

                        //if a collision was made, we activate collision against the suspected
                        p->Collision(a);
                        if (p->isSpectral())
                            continue;
                        else return p->isSpectral();
                    }
            }

            else{ //If not, then we check to see if it is touching another object along with the player
                if (a->isTouching(*avatars[i]->getBoundaries())){
                    //we move the object back to where it came from.
                    avatars[i]->Collision(a);

                    if (avatars[i]->isSpectral())
                        continue;
                    else return a->isSpectral();
                }

                if (a->isCOMPlayer()){
                    if (a->isTouching(*p->getBoundaries())){
                        p->Collision(a);
                        if (p->isSpectral())
                            continue;
                        else return p->isSpectral();
                    }
                }
            }
        }

        //See if the object collides with any of the structures in the room
        int ia1_sz = this->structures.size();
        for (int i = 0; i < ia1_sz; i++){

            if (structures[i]->getPBoundaries()){
                if (a->isTouching( structures[i]->getPBoundaries()) ){
                    structures[i]->Collision(a);

                    if (structures[i]->isSpectral())
                        continue;
                    else return a->isSpectral();
                }

            } else {
                if (a->isTouching( *structures[i]->getBoundaries() )){
                    structures[i]->Collision(a);
                    if (structures[i]->isSpectral())
                        continue;
                    else return a->isSpectral();
                }
            }

        }

        //Same, we check to see if our object is going to collide with any of the inanimate objects within the room.

        int ia2_sz = this->items.size();
        for (int i = 0; i < ia2_sz; i++){
            if (a->isTouching( items[i]->getPBoundaries()))
                return a->isSpectral();
        }

        return false; //if all cases pass then the obj is no touching anything
    }

    //This function is solely for factory-style generated monsters based on the room.

    // - In the future, we might overload the function by passing in the "room-type" to specify which generator

    AnimatedObj *Room::GenerateMonster(SDL_Rect *space, animateObj type = NONE){

        //Our avatar factorization: This is where we will instantiate our enemies into existence!
        //the enemies that are created are simply the roll of the dice.
        AnimatedObj *a = gameCatalyst::gWorld()->MonsterDice(NONE, true);
        gwCoordinates coords = g_rand_coords_from_Rect(*space);

        //The location where the object spawns
        a->setX(coords.x);
        a->setY(coords.y);
        a->setStepCount(rand()%10);
        a->setObjNum( this->RegisterObj() ) ; //Then we set the object number
        a->setDirection(this->getDirection(rand()%4));

        bool flag = true;//Current a_obj position flag

        /*while(flag){

            //If there are no monsters in the vector, we just simply return.
            if (avatars.empty()) break;

            //This is the case that the object is touching another object upon spawn, we loop
            //Since every object that spawns into the vector spawns as the greatest index in iteration, it works
            //perfectly.
            int sz = int(avatars.size());
            for (int obj_c = 0; obj_c < sz; obj_c++)
            {
                //If it does happen to touch another object...
                if (a->isTouching(*avatars[obj_c]->getBoundaries()))
                {
                    coords = g_rand_coords_from_Rect(*space);
                    //Then we give the animated object new coordinates
                    avatars[obj_c]->setX(coords.x);
                    avatars[obj_c]->setY(coords.y);
                    break; //then go back to the while loop and start the cases again.
                }

                //If we made it passed the last one, then we have to okay to set flag to true
                if (obj_c >= obj_c + 1) flag = false;
            }
        }*/

        return a; //return a fresh monster, properly positioned and ready to go!
    }

    void Room::Handle_Monsters(){

        if (!zone_count || !spaces) //if there are no inhabitable spaces for our monsters to spawn, we return
            return;

        //If our limit for generating monsters has been reached, we simply return
        if ( int(avatars.size()) >= a_obj_limit)
            return;

        //This is where we will handle the events of generating monsters
        if (!spawn_timer->timeStarted()){
            spawn_timer->startTimer(rand()%5 * 1000);
        }

        else if (spawn_timer->timeStarted() && spawn_timer->timeIsOut()){

            //If our timer is started and our time is out we then check to see if we are able to
            //add another monster to the vector
            if ( int(avatars.size()) < this->a_obj_limit){
                log("Adding avatar...")
                avatars.push_back( GenerateMonster( spaces[rand()%getZoneCount()]->getSpace() ) );
                log("Handled monster")
                //if we have reached our limit then we will just turn off the timer
                if (int(avatars.size()) >= a_obj_limit) spawn_timer->timeStart(false);
            }
        }
    }

    //~~~~~~~~~~~~~ Handling AOE room actions ~~~~~~~~~~~~~~
    void Room::RenderRoomActions(SDL_Renderer* r){

        if (!room_actions.empty()){

            int sz = room_actions.size();
            for (int i = 0; i < sz; i++){
                room_actions[i]->render(r);
            }
        }
    }

    void Room::QueryRoomActions(){

        //run through each action to see if their time to live has expired
        int sz = room_actions.size();
        for (int i = 0; i < sz; i++){
            if (room_actions[i]->getActionTTL()->timeIsOut()){
                log("Time has run out.")
                room_actions.erase(room_actions.begin() + i);
            }
        }

        //Then check to see if the action is touching anything in the room
        sz = room_actions.size();
        for (int i = 0; i < sz; i++){

            int a_sz = this->g_aObjCount();
            for (int a = 0; a < a_sz; a++){

                AnimatedObj *a_obj = this->avatars[a];
                if (a_obj && room_actions[i]){
                    if (a_obj->isTouching(room_actions[i]->getAOE())){
                        //if so then execute the consequences for that animated object
                        room_actions[i]->consequences(a_obj);
                        log("Action lands and hits animated obj.")
                    }
                }
            }
        }
    }

    void Room::ExecuteRoomActions(){

        //Here we will check to see if the the actions transition their state
        int sz = room_actions.size();
        for (int i = 0; i < sz; i++){
            if (room_actions[i])
                room_actions[i]->execute();
        }
    }

};};