#pragma once
#include "../room.h"

#include "../../Essentials/paradigm.h" //For coordinates struct
#include "../Level_Room/level_room.h" //Get level room class

#include <SDL.h>
#include <vector>
using std::vector;


namespace Astral { namespace Room { 



    //The level in which hosts all activities of the current difficulty
    class Level : public Room{

        // - generates lvl rooms
        // - generates pathways between level rooms
        // generates location of start and finish.
        //A level "has a" Lvl_Room

        private:
            gwCoordinates l_start; //the start point for the player

            //strictly for resizing the guide
            gwCoordinates guide_min, guide_max;
            gwCoordinates guide_units; //n x n size of guide board for x and y
            vector<Pathway> pathway; //The pathway that snakes around level for the player to walk on.
            vector<TangObj::Entry*> entries; //Entries to the level.
            vector <gwCoordinates> path_coords;
            vector <gwCoordinates> bad_coords; //A list of bad coordinates for our guide to look out for.
            vector <gwCoordinates> room_coords; //coordinates of rooms inside the level.
            vector <Room*> lvl_map;

            bool guideIsStuck(); //returns status of current guide index whether "stuck" or not. - O(1)
            bool unique_point(int _x, int _y, vector<gwCoordinates> coords); //checks to see if two coordinates are unique. - O(n)
            bool is_bad_point(int _x, int _y); //a function that checks if a set of coordinates is a bad point. - O(n)
            void resizeGuide(); //this just gets rid of extra space
            void set_guide_points(int _x, int _y){

                if (_x < guide_min.x){
                    guide_min.x = _x;
                } if (_y < guide_min.y) {
                    guide_min.y = _y;
                } if (_x > guide_max.x){
                    guide_max.x = _x;
                } if (_y > guide_max.y){
                    guide_max.y = _y;
                }
            }

            void blueprintRooms(); //using our guide board we [randomly] generate rooms.
            void constructRooms(); //this is where we instantiate lvl rooms to add to the grid.
            void constructGrid(); //initializes a construction grid for the pathways and rooms. (namely SDL Rects and Polygons)
            void connectEntries(); //Connects entries and then adds them to the grid.
        public:
            Level(int n = 0);
            ~Level();

            char **getBuildGuide() const{ return room_buildguide; }// - O(1)
            bool constructBuildGuide();// - O( )
            gwCoordinates start_pts()const{ return l_start; }
            vector<Room*> *getRegistry(){ return &lvl_map; } //This is where all of the rooms in the level will be registered
            int g_room_limit()const { return lvl_map.size(); }
            virtual void renderRoom(SDL_Renderer *r);//renders the current level and its' attributes.
            virtual void xyScroll(int steps, Direction dir);
            virtual void Collision(TangObj::TangibleObj *obj, Direction dir, int steps);
        
        private:
            int difficulty;
            int **lvl_grid; // This integer matrix holds
            int grid_scaling; //this is a variable that, when multiplied, scales the x and y coords of pathways.
            Lvl_Room **rooms; //The amount of rooms located within the Level
            char **room_buildguide; //The guide to how our level assembles its' attributes
    };
};};