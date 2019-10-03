#pragma once
#include <SDL.h>
#include "../room.h"
#include "../../Essentials/paradigm.h"
#include "../Level_Room_Sub/level_room_sub.h"
#include "../../TangObj/tangibleObj.h"
#include <vector>
using std::vector;

namespace Xioudown { namespace Room {
    class Lvl_Room : public Room{
    //A Lvl_Room "has a Lvl_Room_Sub

    private:
        Lvl_Room_Sub *rooms; //The dynamically allocated rooms within the level room
        char **rm_guide; //the guide for how pathways should be constructed in a level room
        int guide_sz_x, guide_sz_y, scale_x, scale_y;
        vector<Room*> sub_rooms;
        vector<gwCoordinates> path_coords;
        vector<Pathway*> walkway;
        gwCoordinates center_start;
        int room_number;
        vector<Room*> rm_reg;

    public:
        Lvl_Room(int rm_num, vector<vector<char> > guide, int sc_x, int sc_y);
        ~Lvl_Room();
        bool unique_point(int _x, int _y, vector<gwCoordinates> coords); //checks to see if two coordinates are unique. - O(n)
        void TranslateRooms(std::vector<std::vector<char> > guide); //translates coordinates into pathway coordinates and creates room guide
        void constructPathways(); //instantiates pathways from pathway coordinates
        void createMainRoom(); //Instantiates the room parent class based on parameters
        int getRoomNumber()const{ return room_number; }
        void setRoomNumber(int n){ room_number = n; }
        vector<Room*> *getRegistry(){ return &rm_reg; }
        Room *getRegistry(int i){ return (rm_reg[i]); }

        virtual void renderRoom(SDL_Renderer *r);//renders the current level and its' attributes.
        virtual void xyScroll(int steps, Direction dir);
        virtual void Collision(TangObj::TangibleObj *obj, Direction dir, int steps);
    }; 
}};;