#pragma once //to make sure that our header file is only included once in our compilation; header is the interface/library

#include <SDL.h>
#include <vector>

#ifndef environment
#define environment
#include "gameWindow.h"
#include "../Essentials/paradigm.h"
#include "../Essentials/player/player.h"
#include "../Essentials/textureManager.h"
#include "../TangObj/tangibleObj.h"
#include "../Room/room.h"
#endif


using std::vector;

namespace Xioudown{ namespace Catalyst {

    using namespace TangObj;

    // organized from big scope to small
    class gameCatalyst{
        private:
            static gameCatalyst *world; //This is the instance of our game world. We will call it accordingly
            std::vector<gwCoordinates> gwStart; //start coordinates for the player
            std::vector<std::vector<Room::Room*> > gRegistry; //Testing // The actual level room pointers for the game.

            gameWindow *game_window;
            
            Essentials::TextureManager *texture_manager; //Texture manager for every texture and sprite within the game
            Essentials::Player *player; // this is our player; this was debatable on whether or not this class should be the singleton.
            
            Room::Room* test_room; //this is just a test room where we can spawn anything or test structures.
            Room::Room **Levels; //a reference holder for where we allocate our levels
            
        public:

            //Here we will return the instance of our game world; This is where we will call our game world variable
            static gameCatalyst* gWorld(){

                if (!world)
                    world = new gameCatalyst;

                return world;
            }


            gameCatalyst();
            ~gameCatalyst();
            bool initDependence();
            void Process_Level(); //Here is where we process other rooms inside of the level.
            static int gwTask_Handle_Rooms( void *obj ); //changing the name later - 12/25/2018
            TangObj::AnimatedObj *MonsterDice(animateObj type, bool COM); //A method that returns a new Monster at random/request.

        //small scope variables
        private:
            //the systems 'OK' and game state
            bool systems_go, gameState, has_finalized; 
            
            bool roomChange; //This determines the process of changing rooms.

            //current level/rooms to be rendered and processed
            int level_count, current_lvl, current_room;
            
            int renderScaling;
            static int gwTraversal;

            gwCoordinates rm_change; //the desired room and level (x and y) for room change
            
            TangObj::Entry *queued_entry;
            TangObj::AnimatedObj *queued_aobj;
            SDL_Surface *gwScreenSurface;


        //small scale/ background methods
        public:

            void renderEverything(); //This is where we render everything to the screen: of course there are cases where we specify.
            void processEvents();//This is where we will process the bulk of game events
            void changeRooms(); //This function happens when the there is a request to change rooms
            void traverseRooms(int room, int level); //This function happens when an object is being added to a room
            void connectLevelEntries(); //This function connects the entries of adjacent levels

            //Testing purposes only
            Room::Room* getTestRoom() const { return test_room; }

            //This is where we will scroll everything, including the rooms' contents according to the player;
            void scrollToPlayer(); //scrolls everything to center the player to the screen.
            void xyScroll(int x_steps, int y_steps, Room::Room *room); //a scroll that all methods can use to scroll their assets according to the player
            void resetRandom(); //arbitrary reset for every SDL_Tick
            bool finalize(); // an internal backend game refurbisher: connects room entries; gets rid of bs

            TangObj::AnimatedObj *createDeepCopy(TangObj::AnimatedObj *a); //Returns an exact copy of a given animated object
            TangObj::InanimateObj *createDeepCopy(TangObj::InanimateObj *ia); //Returns an exact copy of a given animated object


        //getters and setters
        public:
            //The getter for gameCatalyst usability
            bool gameStatusOK()const{ return systems_go; }

            //getter and setters for game registry
            void addLeveltoRegistry(std::vector<Room::Room*> levels){ gRegistry.push_back(levels); }
            Room::Room* getRoomFromReg(int level, int room){ return gRegistry[level][room]; }
            std::vector<Room::Room*> getLevelFromRegistry(int level){ return gRegistry[level]; }
            
            //Getter for the current room
            Room::Room *getCurrentLevel() { return gRegistry[current_lvl][current_room];} 
            
            //Getter and Setter for the player
            Essentials::Player* getPlayer() { return player; }
            void setPlayer(Essentials::Player *p){ player = p; }

            //Getter and setter for the game's state
            bool g_gameState()const{ return gameState; }
            void s_gameState(bool state){ gameState = state; }

            //getter and setter for texture manager
            Essentials::TextureManager *getManager(){ return texture_manager; }
            gameWindow *getWindow(){ return game_window; }

            SDL_Texture *gwLoadMedia(char* path);    

            //Getters and setters for level affairs
            void setLevelCount(int n){ level_count = n; }
            void setLevel(int n) { current_lvl = n; }
            void setRoom(int n) { current_room = n; }
            gwCoordinates getStart()const{ return gwStart[current_lvl]; }
            void setStart(gwCoordinates c){ gwStart.push_back(c); }
            bool traversalOccupied() { return gwTraversal; }
            int getLevelCount()const{ return level_count; }

            //getters for Entries, traversals, 'n such.
            void setQueuedEntry(TangObj::Entry *e){ queued_entry = e; }
            void queueTransition(TangObj::AnimatedObj *a, TangObj::Entry* e, int room, int level);
            TangObj::Entry *getQueuedEntry(){ return queued_entry; }
            void setQueuedAobj(TangObj::AnimatedObj *a){ queued_aobj = a; }
            TangObj::AnimatedObj *getQueuedAobj(){ return queued_aobj; }
    };
}; };
