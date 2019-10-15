#include <iostream>
#include <fstream>

#include "../../Catalyst/gameCatalyst.h"
#include "level_room.h"

#define log(x) std::cout << "Lvl Room: from "<< __func__ << " " << x << std::endl;

namespace Xioudown { namespace Room {
    
    using namespace TangObj;
    
    //This class represents and instance of a room inside of a level (notably the "Level room")
    Lvl_Room::Lvl_Room(int rm_num, std::vector<std::vector<char> > guide, int sc_x, int sc_y) : Room(),
        scale_x(sc_x), scale_y(sc_y), room_number(rm_num){

        //log("Entering Lvl Room creation")

        //We pass the room guide (contains the size and direction of build for the room)
        //in and translate it into "xy" coords.
        TranslateRooms(guide);


        //log("Making file")
        //std::string outfile = ;
        char* of = "C:/Users/Micha/Documents/CodeBlocks Projects/Game World/Levels/LEVEL_ROOM.txt";

        std::ofstream fout(of);
        for (int r = 0; r < guide_sz_y; r++){
            for (int c = 0; c < guide_sz_x; c++){
                //initialize our visual guide
                fout << rm_guide[r][c];
            }
            fout << std::endl;
        } //log("Made file")
        fout.close();

        constructPathways();

        //Add itself to it's registry and then other rooms
        rm_reg.push_back(this);
    }

    Lvl_Room::~Lvl_Room(){

        //log("Deleting level room content")

        if (guide_sz_y)
            for (int i = 0; i < guide_sz_y; i++){
                delete [] rm_guide[i];
            }
    }

    bool Lvl_Room::unique_point(int x, int y, std::vector<gwCoordinates> coords){


        int sz = coords.size();

        if(sz){
            for (int i = 0; i < sz; i++){

                if (x == coords[i].x && y == coords[i].y)
                    return false;
            }
        } return true;
    }


    //This is where we determine the direction of the rooms' entry given the room being built.
    void Lvl_Room::createMainRoom(){
        //create room boundaries based on the n pathways in the main room.
        //this->Room(this->walkway);

        //log( "\nY: " << this->getBoundaries()->x << "\nX: " << this->getBoundaries()->y )
    }

    //Once we've constructed our pathway guide we can build the Level Room pathways that make the room
    void Lvl_Room::constructPathways(){

        //get the size of our room guide
        int sz = path_coords.size();
        for (int i = 0; i < sz; i++){

            //Same for generating a Level:
            std::vector<Direction> dirs;//for all possible directions to build walls accordingly
            gwCoordinates c = path_coords[i];

            //if we have no known direction ahead, we'll seal off the pathway with a wall.
            if ( unique_point(c.x, c.y-1, path_coords) )
                dirs.push_back(UP);
            if ( unique_point(c.x, c.y+1, path_coords) )
                dirs.push_back(DOWN);
            if ( unique_point(c.x-1, c.y, path_coords) )
                dirs.push_back(LEFT);
            if ( unique_point(c.x+1, c.y, path_coords) )
                dirs.push_back(RIGHT);

            walkway.push_back( new Pathway(c.x * scale_x, c.y * scale_y, dirs, scale_x, scale_y) );

            //if the parameters are met, then we will instantiate the "main" room
            if (i + 1 == 9){
                createMainRoom();
            }
        }

        sz = walkway.size();
        for (int i = 0; i < sz; i++){
            //add the walls of each pathway to the room
            int walls = walkway[i]->wall_count();
            for (int w = 0; w < walls; w++)
                addStructure(walkway[i]->getWall(w));
        }


        //Creating the boundary lines of our room - based on the paths max y + h and x + w
        this->getBoundaries()->w = 0;
        this->getBoundaries()->h = 0;
        int i_sz = walkway.size();
        for (int i = 0; i < i_sz; i++){
            SDL_Rect p = walkway[i]->getWalkway();

            if (i == 0){
                this->getBoundaries()->x = p.x;
                this->getBoundaries()->y = p.y;
            }

            if (this->getBoundaries()->w < p.x + p.w)
                this->getBoundaries()->w = p.x + p.w;

            if (this->getBoundaries()->h < p.y + p.h)
                this->getBoundaries()->h = p.y + p.h;
        }

        SDL_Rect* r = this->getBoundaries();
        //log("Width: " << r->w << "  Height: " << r->h)
    }

    //renders the current level_room and its' attributes.
    void Lvl_Room::renderRoom(SDL_Renderer *r){

        int sz = this->walkway.size();

        for (int i = 0 ; i <sz; i++)
            this->walkway[i]->render(r);

        vector<InanimateObj*> *structures = this->getStructures();

        sz = structures->size();

        //for (int i = 0; i < sz; i++)
    //        structures[i]->renderInanimateObj();

        Room::renderRoom(r);
    }

    void Lvl_Room::xyScroll(int steps, Direction dir){

        int pw_sz = walkway.size();

        if (dir == UP || dir == DOWN){
            for (int i = 0; i < pw_sz; i++)
                this->walkway[i]->y(steps);

        } else if (dir == LEFT || dir == RIGHT){
            for (int i = 0; i < pw_sz; i++)
                this->walkway[i]->x(steps);

        } Room::xyScroll(steps, dir);
    }

    void Lvl_Room::Collision(TangibleObj *obj, Direction dir, int steps){

        Room::Collision(obj, dir, steps);
    }

    //this function translates rooms into pathway coordinates and creates
    void Lvl_Room::TranslateRooms(std::vector<std::vector<char> > guide){

        //Generate the size for the matrix
        int sz_y = guide.size();
        guide_sz_x = guide[0].size();
        for (int i = 1; i < sz_y; i++){
            if (guide_sz_x < (int)guide[i].size())
                guide_sz_x = guide[i].size();
        }

        //scale the constraints relative to the size of each room
        guide_sz_y = sz_y * 3;
        guide_sz_x *= 3;

        rm_guide = NULL;

        //create the pathway build guide matrix from given constraints
        if (scale_x && scale_y){
            rm_guide = new char*[guide_sz_y];
            for (int i = 0; i < guide_sz_y; i++){
                rm_guide[i] = new char[guide_sz_x];
            }
        }

        //initialize the pathway build guide matrix
        for (int r = 0; r < guide_sz_y; r++)
            for (int c = 0; c < guide_sz_x; c++){
                rm_guide[r][c] = '.';
            }

        //Then add points to the vector for pathway building
        for (int r = 1, rm_y = 0; r < guide_sz_y; r+=3, rm_y++){

            for (int c = 1, rm_x = 0; c < guide_sz_x; c+=3, rm_x++ ){

                //if the given coordinates are a room, then we add them to the vector.
                if (guide[rm_y][rm_x] == 'R'){

                    gwCoordinates g;
                    g.x = c;
                    g.y = r;
                    //if the coordinate is a room, we push back all coordinates around (1,1) .. (1,4)
                    path_coords.push_back(g);
                    g.x = c;
                    g.y = r-1;
                    path_coords.push_back(g);
                    g.x = c+1;
                    g.y = r-1;
                    path_coords.push_back(g);
                    g.x = c+1;
                    g.y = r;
                    path_coords.push_back(g);
                    g.x = c+1;
                    g.y = r+1;
                    path_coords.push_back(g);
                    g.x = c;
                    g.y = r+1;
                    path_coords.push_back(g);
                    g.x = c-1;
                    g.y = r+1;
                    path_coords.push_back(g);
                    g.x = c-1;
                    g.y = r;
                    path_coords.push_back(g);
                    g.x = c-1;
                    g.y = r-1;
                    path_coords.push_back(g);

                    //then mark the map
                    rm_guide[r][c] = 'x';
                    rm_guide[r-1][c] = 'x';
                    rm_guide[r-1][c+1] = 'x';
                    rm_guide[r][c+1] = 'x';
                    rm_guide[r+1][c+1] = 'x';
                    rm_guide[r+1][c] = 'x';
                    rm_guide[r+1][c-1] = 'x';
                    rm_guide[r][c-1] = 'x';
                    rm_guide[r-1][c-1] = 'x';

                } else continue;
            }
        }
    }

};};