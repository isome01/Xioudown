#pragma once
#include <SDL.h>
#include "../Essentials/paradigm.h"
#include "../Essentials/objTextureSheet.h"
#include "../TangObj/Animated_Obj/animatedObj.h"
#include "../TangObj/Inanimate_Obj/inanimateObj.h"
#include "../ObjAction/game_actions.h"

#include <vector>

using std::vector;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Room Generic Class and subclasses. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//This room generates based off of the current difficulty of the game.


namespace Astral { namespace Room{ 
    
    using namespace Essentials;

    //This is a class exclusively for Level design:
    // - This class constructs paths in between rooms: basically a glorified SDL_Rect or Polygon
    // - This also constructs walls necessary for
    class Pathway{

        private:
            //Default sidewalk length and width is 500
            int width, height;

            int w_count;
            SDL_Rect walkway;
            std::vector< TangObj::Wall* > walls;
            std::vector< TangObj::Entry* > entries;
            Essentials::gwTexturePack *tiles;

        public:

            Pathway(double _x, double _y, std::vector<Direction> dirs, int _w = 500, int _h = 500);
            ~Pathway();

            const int x() const{ return walkway.x; }
            void x(int n) { walkway.x += n; }
            const int y() const{ return walkway.y; }
            void y(int n) { walkway.y += n; }
            const int w() const{ return walkway.w; }
            const int h() const{ return walkway.h; }
            const int wall_count() const{ return walls.size(); }
            TangObj::Wall *getWall(int i){ return walls[i]; }
            SDL_Rect getWalkway() { return walkway; }
            void render(SDL_Renderer *r); //renders every aspect of the pathway
    };

    class EntryPackage{

        private:
            TangObj::Entry *entry;
        public:
            EntryPackage(Direction d, int rm, int lvl, int _x, int _y);
            ~EntryPackage(){ }

            TangObj::Entry *getEntry(){ return entry; }
    };

    //This room generates based off of the current difficulty of the game.
    class InhabitableSpace{

        private:
            SDL_Rect* space;
        public:
            //DEFAULT: Creates an SDL Rect from given zone coordinates.
            InhabitableSpace(const SDL_Rect& zone){

                space = new SDL_Rect;
                space->w = zone.w / 2;
                space->h = zone.h / 2;

                //center the zone space
                space->x = (zone.x + (zone.w / 2) / 2);
                space->y = (zone.y + (zone.h / 2) / 2);
            } //Most inhabitable spaces will be half the space of the zone.

            ~InhabitableSpace() { delete space;}

            //Getters and (no setters)
            const int getX() const{ return space->x; }
            const int getY() const{ return space->y; }
            const int getW() const{ return space->w; }
            const int getH() const{ return space->h; }
            SDL_Rect *getSpace() const{ return space; }
            void render();
    };

//The room class: A room can be anything (Forest, desert, mountain basin, etc.).
    class Room{
        private:
            InhabitableSpace **spaces;
            Timer *spawn_timer; //A timer that ticks for each enemy to spawn.
            SDL_Rect background; //The sdl rect color background
            SDL_Rect *door; //Here we will create a series/amount of doors according to the generic room
            SDL_Rect roomBorder; // this tells us where the boundaries are for the window frame.
            SDL_Rect *zones;
            int zone_count; //a placeholder for the number of zones inside our room.
            int a_obj_limit; //The limit for however many animated objects are aloud to be rendered at once.
            // Since we are unable to use integers within our gameState, we take the number of rooms in our
            // class, that way they all contain the details.
            int numOfRooms, room_limit;
            static double Register_Number;
            vector< TangObj::AnimatedObj* > avatars; //Animated objects that are spawned inside the room.
            vector< TangObj::InanimateObj* > structures; //structures and inanimate objects within the room.
            vector< TangObj::InanimateObj* > items; //any items laying on the ground.
            TangObj::Entry *entry; //this services as a placeholder for entries that are dynamically allocated
            vector<TangObj::Entry*> start_end;
            vector<ObjAction::Action*> room_actions;

            double aobj_counter;

        public:
            Room(vector<Pathway*> paths = vector<Pathway*>(), int n = 0); //Initializing a room with an optional 'n' number of rooms within the room.
            ~Room();

            //Basic getters and setters for Room class
            SDL_Rect *getBoundaries() { return &roomBorder; }

            TangObj::AnimatedObj *getAnimatedObject(int n){ return avatars[n]; }

            int g_numOfRooms()const { return numOfRooms; }
            int g_aObjCount()const{ return avatars.size(); }
            const int getZoneCount() const{ return zone_count; }
            SDL_Rect *getZone(int n) const{ return &zones[n]; }
            SDL_Rect *getSpace(int n) const{ return spaces[n]->getSpace(); }
            bool processCollisions(TangObj::AnimatedObj *a);
            virtual void renderRoom(SDL_Renderer *r); //Renders the current room on offset
            virtual void xyScroll(int steps, Direction dir);

            //Dealing with entries and room traversals
            void manageEntry(TangObj::Entry *e) { entry = e; }
            TangObj::Entry* getEntry()const{ return entry; }
            void manage_start_end(TangObj::Entry *e) { start_end.push_back(e); }
            TangObj::Entry *getStartEnd(int i){ return start_end[i]; }

            virtual void Collision(TangObj::TangibleObj *obj, Direction dir, int steps); //the verdict of what happens to an object, so long as it touches the boundary
            void addAvatar(TangObj::AnimatedObj *a){ avatars.push_back(a); }
            void addStructure(TangObj::InanimateObj* ia){ structures.push_back(ia); }
            void destroyAvatar(int n){ avatars.erase(avatars.begin() + n);}
            vector<TangObj::InanimateObj*> *getStructures(){ return &structures; }
            void addItem(TangObj::InanimateObj* ia){ items.push_back(ia); }
            void processRoomEvents(); //Literally does as it says. Go to implementation "RoomFunctions.cpp".
            virtual std::vector<Room*> *getRegistry(){ return new std::vector<Room*>; }
            Direction getDirection(int n){//This is a function that allows return of a Direction

                switch(n){
                    case (0): return UP;
                    case (1): return DOWN;
                    case (2): return LEFT;
                    case (3): return RIGHT;
                }

                return DOWN; //Facing downward is the default value
            }

            void GenerateZones(std::vector<Pathway> paths); //A function to create zones for inhabitable spaces within the room
            TangObj::AnimatedObj *GenerateMonster(SDL_Rect *zone, animateObj type); //This function generates a random monster based on zone.
            void Handle_Monsters(); //This function handles processes from "AnimatedObjs"/Monsters for rooms.

            //How the room handles AOE actions
            void RenderRoomActions(SDL_Renderer* r);
            void ExecuteRoomActions();
            void QueryRoomActions();

            int ThreadTest(void *data){ return 0; }
            TangObj::Entry* CreateEntryFromRect(Direction d, SDL_Rect r);
            int GenerateWall(); //Generates a wall from an SDL_Rect
            static double RegisterObj();
            void addAction(ObjAction::Action *a = NULL){
                if (a == NULL){
                    printf("action is NULL");
                    return;
                }room_actions.push_back(a);
            }
        };
};};