#include <iostream>
#include <fstream>
#include <string.h>
#include <SDL.h>
#include <ctime>
#include "level.h"
#include "../../TangObj/tangibleObj.h"
#include "../../Catalyst/gameCatalyst.h"
#include "../../Essentials/paradigm.h"
#include <vector>

using std::vector;

namespace Xioudown { namespace Room{
        
    using namespace TangObj;

    #define log(x) std::cout << "Level object: " << x << std::endl;

    Level::Level(int n) : Room(vector<Pathway*>(), n), difficulty(n), grid_scaling(700) {

        if (!difficulty) difficulty = 1;

        //construct the build guide
        if (!constructBuildGuide())
            log("Unable to construct guide for Level: " << difficulty)
        else{

            //We add this distinct room to the level map
            lvl_map.push_back(this);

            //log("Rooms: " << Room::g_numOfRooms())
            //Then create rooms based off of the build guide
            if (Room::g_numOfRooms()){
                rooms = new Lvl_Room*[g_numOfRooms()];
                //log("There are rooms in this level.")

                //mark the rooms for construction
                blueprintRooms();
                constructRooms();
                //Construct pathways and pathway-connectors
            } else {
                rooms = NULL;
            }

            //Let's see the fruits of our labor.
            //LATER ON:
            // - We'll use this to generate a pixel-filled mini-map. :D

            //log("Making file")
            char *outfile = (char*)("./generated_level_.txt");
            std::ofstream fout(outfile);
            for (int r = 0; r < guide_units.y; r++){
                for (int c = 0; c < guide_units.x; c++){
                    //initialize our visual guide
                    fout << room_buildguide[r][c];
                }
                fout << std::endl;
            } //log("Made file")
            fout.close();

            //We'll create the build grid
            this->constructGrid();
            //this->CreateEntries();//then create entries once rooms are established


            //Then add the start and end to levels accordingly
            Entry *a = NULL;
            Entry *b = NULL;
            if (n == 0){

                a = new Entry(CENTER, grid_scaling, path_coords[path_coords.size()-1].x * grid_scaling, path_coords[path_coords.size()-1].y * grid_scaling, 0, 0);
                entries.push_back(a);
                manage_start_end(a);
                addStructure(a);

            } else if (n == Catalyst::gameCatalyst::gWorld()->getLevelCount() - 1){

                a = new Entry(CENTER, grid_scaling, path_coords[0].x * grid_scaling, path_coords[0].y * grid_scaling, 0, difficulty);
                entries.push_back(a);
                manage_start_end(a);
                addStructure(a);

            } else {

                a = new Entry(CENTER, grid_scaling, path_coords[0].x * grid_scaling, path_coords[0].y * grid_scaling, 0, difficulty);
                b = new Entry(CENTER, grid_scaling, path_coords[path_coords.size()-1].x * grid_scaling, path_coords[path_coords.size()-1].y * grid_scaling, 0, difficulty);
                entries.push_back(a);
                entries.push_back(b);
                manage_start_end(a);
                manage_start_end(b);
                addStructure(a);
                addStructure(b);
            }


            //create start point coordinates from the fully constructed level
            SDL_Rect start = pathway[0].getWalkway();
            this->l_start.x = start.x + start.w / 2;
            this->l_start.y = start.y + start.h / 2;

            //set the start coordinates for our player in gameWorld class
            Catalyst::gameCatalyst::gWorld()->setStart(this->l_start);

            //last but not least: add all rooms in this level to the level map
            //lvl_map.pushback()
        }
    }

    Level::~Level(){

        if (Room::g_numOfRooms() > 1)
            delete [] rooms;
        else if(Room::g_numOfRooms() <= 1)
            delete rooms;

        for (int i = 0; i < guide_units.y; i++)
            delete [] room_buildguide[i];

        //honestly this can be deallocated with the "room_buildguide"
        for (int i = 0; i < guide_units.y; i++)
            delete [] lvl_grid;

        log("Deallocation successful")
    }

    void Level::renderRoom(SDL_Renderer *r){

        //Here we'll render room stuff first before rendering room contents
        //render every pathway and its' contents

        //log("About to render level")

        int pw_sz = pathway.size();

        for (int i = 0; i < pw_sz; i++)
            pathway[i].render(r);

        int sz = entries.size();
        for (int i = 0; i < sz; i++)
            entries[i]->renderInanimateObj(r);

        //log("Rendered level contents.")
        //rendering room contents
        Room::renderRoom(r);
    }

    void Level::xyScroll(int steps, Direction dir){

        //Scroll everything exclusive to the level first
        //...like the pathway

        int pw_sz = pathway.size();

        if (dir == UP || dir == DOWN){
            for (int i = 0; i < pw_sz; i++)
                this->pathway[i].y(steps);

        } else if (dir == LEFT || dir == RIGHT){
            for (int i = 0; i < pw_sz; i++)
                this->pathway[i].x(steps);

        } else return; //for any unknown direction we continue.

        //Then scroll everything in it's room part.
        Room::xyScroll(steps, dir);
    }

    //This isn't supposed to be much of a function. We just need to iteratively check and see if
    //the player is touching any objects within the room.
    void Level::Collision(TangibleObj* obj, Direction dir, int steps){

        //Collide with objects inside the "level" first


        //then check for room collision
        Room::Collision(obj, dir, steps);
    }

    void Level::constructRooms(){

        //Create a list of known coordinates
        //we iterate through the rooms list.
        int rm_sz = room_coords.size();

        int cur_rms = lvl_map.size();
        //create a vector of known rooms
        std::vector<gwCoordinates> uniques;

        for (int i = cur_rms; i <= rm_sz; i++){

            //first we check to see if the room has used these coordinates already
            gwCoordinates rm_c = room_coords[i];
            if (unique_point(rm_c.x, rm_c.y, uniques)){

                //log("\nRoom x: " << rm_c.x << "\nRoom y: " << rm_c.y)
                //create a vector of vector coords and initialize it with empty coords
                std::vector<std::vector <char> > rms;

                //initialize the vector
                for (int r = 0; r < 3; r++){
                    std::vector<char> v;
                    for (int c = 0; c < 3; c++) v.push_back('.');

                    rms.push_back(v);
                }

                //we push back the first
                rms[1][1] = 'R';
                uniques.push_back(rm_c);

                //check to see if left is a room and if it's not already identified
                if ( !(unique_point(rm_c.x-1, rm_c.y, room_coords) ) && (unique_point(rm_c.x-1, rm_c.y, uniques) ) ){
                    rms[1][0] = 'R';
                    rm_c.x -= 1;
                    uniques.push_back(rm_c);

                } if (!(unique_point(rm_c.x+1, rm_c.y, room_coords)) && (unique_point(rm_c.x+1, rm_c.y, uniques) )) {//same with the right
                    rms[1][2] = 'R';
                    rm_c.x += 1;
                    uniques.push_back(rm_c);

                } if (!(unique_point(rm_c.x, rm_c.y-1, room_coords)) && (unique_point(rm_c.x, rm_c.y-1, uniques) ) ) {
                    rms[0][1] = 'R';
                    rm_c.y -= 1;
                    uniques.push_back(rm_c);

                } if (!(unique_point(rm_c.x, rm_c.y+1, room_coords)) && (unique_point(rm_c.x, rm_c.y+1, uniques) ) ) {
                    rms[2][1] = 'R';
                    rm_c.y += 1;
                    uniques.push_back(rm_c);
                }

                //then we create the level room given the vector
                Lvl_Room *lvl_room = new Lvl_Room(i, rms, grid_scaling, grid_scaling);

                SDL_Rect *bounds = lvl_room->getBoundaries();
                //Then we connect the room to the closest pathway on the level
                Entry *a = NULL;
                //create an entry for the level room
                Entry *b = NULL;

                /*int xw = (bounds->x - bounds->w >= 0) ? (bounds->x - bounds->w) : (bounds->x - bounds->w * -1);
                int yh = (bounds->h - bounds->y >= 0) ? (bounds->h - bounds->y) : (bounds->h - bounds->y * -1);*/

                //log("\nLvl_Room X: " << bounds->x << "/nLvl_Room Y: " << bounds->y)
                if (!unique_point(rm_c.x, rm_c.y + 1, path_coords)){

                    //once we have found a coordinate we can latch on to, then create entry from them
                    a = new Entry(DOWN, grid_scaling, rm_c.x * grid_scaling, (rm_c.y + 1) * grid_scaling, 0, difficulty);
                    b = new Entry(UP, grid_scaling, bounds->x, bounds->y, i, difficulty);
                }else if (!unique_point(rm_c.x, rm_c.y - 1, path_coords)){

                    a = new Entry(UP, grid_scaling, rm_c.x* grid_scaling, (rm_c.y - 1) * grid_scaling, 0, difficulty);
                    b = new Entry(DOWN, grid_scaling, bounds->x, bounds->y, i, difficulty);
                }else if (!unique_point(rm_c.x + 1, rm_c.y, path_coords)){

                    a = new Entry(RIGHT, grid_scaling, (rm_c.x + 1) * grid_scaling, rm_c.y * grid_scaling, 0, difficulty);
                    b = new Entry(LEFT, grid_scaling, bounds->x, bounds->y, i, difficulty);
                }else if (!unique_point(rm_c.x - 1, rm_c.y, path_coords)){

                    a = new Entry(LEFT, grid_scaling, (rm_c.x - 1) * grid_scaling, rm_c.y* grid_scaling, 0, difficulty);
                    b = new Entry(RIGHT, grid_scaling, bounds->x, bounds->y, i, difficulty);

                } else continue;

                //connect entries from level to level room and then register
                //Then connect the entries
                b->setAdjacent(a);
                a->setAdjacent(b);

                //Push entry into lvl room
                lvl_room->manage_start_end(b);
                lvl_room->addStructure(b);

                entries.push_back(a);

                //then call the return Rooms function from the created level room - in a for loop.
                for (int i = 0; i < 1; i++){
                    Room *r = lvl_room->getRegistry(i);
                    lvl_map.push_back( r ); //push every room onto the registry
                }
            }
        }

        //log( "\n\n" )

        //log("\nEntries Total: " << entries.size() << "\n")

        int sz = entries.size();
        for (int i = 0; i < sz; i++){
            addStructure(entries[i]);
            //log("Added entry")
        }

    }

    //We can build an initializer grid for the level - so we can place rooms accordingly
    void Level::constructGrid(){

        //We'll run through the vector of coordinates
        int sz = path_coords.size();
        for (int i = 0; i < sz; i++){

            //This is how we are going to build our walls:
            // - we check to see if there is a pathway on all directions:
            //      - if there is an adjacent pathway we won't build a wall that direction
            //      - if not, we build one.
            // - for rooms, we implement a structure of walls representing the room

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

            //Then push our pathway into the vector accordingly
            pathway.push_back( Pathway(
                                    c.x*grid_scaling,
                                    c.y*grid_scaling,
                                    dirs, grid_scaling, grid_scaling));
        }

        //then we register each of the pathway's contents as structures

        int pw_sz = pathway.size();
        for (int i = 0; i < pw_sz; i++){

            int wc = pathway[i].wall_count();
            for (int index = 0; index < wc; index++){
                addStructure(this->pathway[i].getWall(index));
            }
        }

        //then create a zone for enemies to spawn for each pathway
        GenerateZones(pathway);
    }

    bool Level::guideIsStuck(){ //This allows the guide to do backtracking if return true.

        int sz = path_coords.size();
        gwCoordinates c = path_coords[sz - 1];

        //Check on all sides of the current index for an open spot.
        //open spots are defined by:
        // - whether they are of value '.'
        // - if they aren't a bad spot
        bool impasse1 = (!(unique_point(c.x, c.y-1, path_coords))) || (is_bad_point(c.x, c.y-1) );
        bool impasse2 = (!(unique_point(c.x, c.y+1, path_coords))) || (is_bad_point(c.x, c.y+1) );
        bool impasse3 = (!(unique_point(c.x-1, c.y, path_coords))) || (is_bad_point(c.x-1, c.y) );
        bool impasse4 = (!(unique_point(c.x+1, c.y, path_coords))) || (is_bad_point(c.x+1, c.y) );

        return impasse1 && impasse2 && impasse3 && impasse4; //return true if no open index; guide is stuck.

    } //We'll call this function for every point


    //~NOTE:
    // Would have made these two functions just one function, but for the sake
    // of familiarity they're two that are separate.
    bool Level::is_bad_point(int _x, int _y){

        int bad_sz = bad_coords.size();

        if (bad_sz){
            for (int i = 0; i < bad_sz; i++){
                if (_x == bad_coords[i].x && _y == bad_coords[i].y)
                    return true;
            }
        }

        return false;
    }

    bool Level::unique_point(int _x, int _y, std::vector<gwCoordinates> coords){

        int sz = coords.size();

        if(sz){
            for (int i = 0; i < sz; i++){

                if (_x == coords[i].x && _y == coords[i].y)
                    return false;
            }
        }

        return true;
    }

    void Level::resizeGuide(){

        int sz = path_coords.size();

        //first we check min an max values across the x and y
        if (sz){
            for (int i = 0; i < sz; i++)
                set_guide_points(path_coords[i].x, path_coords[i].y);

            //for the sake of conventional scaling, we'll go ahead and expand the x and y
            guide_max.x+=2;
            guide_max.y+=2;
            guide_min.x-=2;
            guide_min.y-=2;

            int dst_x = guide_max.x - guide_min.x;
            int dst_y = guide_max.y - guide_min.y;

            //then we create a temporary vector and char matrix to resize our current guide and map.
            //~~ --------------------------------------------------------------- ~~
            char** tmp_guide = new char*[dst_y];
            for (int i = 0; i < dst_y; i++)
                tmp_guide[i] = new char[dst_x];

            //inputting values according to scale of guide min and max x/y.
            std::vector<gwCoordinates> temp;
            gwCoordinates coords;

            std::vector<char> coordinates;
            for (int r = guide_min.y; r < guide_max.y; r++){
                for (int c = guide_min.x; c < guide_max.x; c++){
                    coordinates.push_back(room_buildguide[r][c]);
                }
            }

            for (int i = 0; i < sz; i++){
                //we are shifting all values to the left and up
                coords.x = path_coords[i].x - guide_min.x;
                coords.y = path_coords[i].y - guide_min.y;
                temp.push_back(coords);
            }

            //Now time to fill in the matrix

            sz = coordinates.size();
            int c_count = 0;
            for (int r = 0; r < dst_y; r++){
                for (int c = 0; c < dst_x; c++){
                    if (c_count < sz){
                        tmp_guide[r][c] = coordinates[c_count++];
                    }
                }
            }

            //We'll deallocate the old memory for our room guide
            for (int r = 0; r < guide_units.x; r++)
                delete [] room_buildguide[r];

            //Then fill in the details with new reallocated memory
            room_buildguide = new char*[dst_y];
            for (int i = 0; i < dst_y; i++)
                room_buildguide[i] = new char[dst_x];

            for (int r = 0; r < dst_y; r++){
                for (int c = 0; c < dst_x; c++){
                    room_buildguide[r][c] = tmp_guide[r][c];
                }
            }
            //deallocate our temporary matrix
            for (int i = 0; i < dst_y; i++)
                delete [] tmp_guide;

            //here we set our guide units again.
            guide_units.x = dst_x;
            guide_units.y = dst_y;
            path_coords = temp; //Get our new path coordinates

        } else return;
    }

    //Here:
    // fairly simple. We'll just add rooms where there is space.
    void Level::blueprintRooms(){

        int room_count = g_numOfRooms(); //have a running count to check if we have made all of the rooms

        //the guide then makes exhaustive searches for room connectors and rooms.
        int path_size = int(path_coords.size());
            gwCoordinates c;

        while (room_count > 0){ //if there are still rooms that need to be built

            for (int i = 0; i < path_size; i++){

                if (room_count % 50 == 0)
                    log("Room count: " << room_count)

                c = path_coords[i];
                Direction dir = getDirection( rand()% 4);
                //if we've reached the max num of rooms
                if (room_count <=0)
                    break;
                //let's also make it a roll of the dice whether a room is allowed to be built or not.
                bool build = ( (rand()%12 + 1 > 8)?(true):(false)  );
                if (! build )
                    continue;

                //We'll use the coordinates stored in our pathway vector to iterate our guide accordingly
                if (dir == UP  ){ //we take a look up
                    if (unique_point(c.x, c.y-1, path_coords)){
                        room_buildguide[c.y-1][c.x] = 'R';
                        c.y--;
                        room_coords.push_back(c);//then push the coordinates back into the rooms list
                        room_count--;
                    }
                } else if(dir == DOWN ){ //... or down
                    if (unique_point(c.x, c.y+1, path_coords)){
                        room_buildguide[c.y+1][c.x] = 'R';
                        c.y++;
                        room_coords.push_back(c);
                        room_count--;
                    }
                } else if(dir == LEFT ){ //... etc...
                    if (unique_point(c.x-1, c.y, path_coords)){
                        room_buildguide[c.y][c.x-1] = 'R';
                        c.x--;
                        room_coords.push_back(c);
                        room_count--;
                    }
                } else if(dir == RIGHT ){
                    if (unique_point(c.x+1, c.y, path_coords)){
                        room_buildguide[c.y][c.x+1] = 'R';
                        c.x++;
                        room_coords.push_back(c);
                        room_count--;
                    }
                } else continue;
            }
        }
        return;
    }

    bool Level::constructBuildGuide(){

        //log("Enters" << __func__)
        //renew the random generator seed

        //creating the guide board with room of size greater than 'difficulty x difficulty'.
        // and middle point of the guide board.
        int dif = (difficulty + 2) * 2 + 1;
        int mid = (dif / 2) + 1;

        //set our guide units (even though we won't use them specifically in this object method)
        guide_units.x = (difficulty + 2) * 2 + 1;
        guide_units.y = (difficulty + 2) * 2 + 1;

        //make guide min/max presets as future reference to resize function
        this->guide_min.x = mid - 1;
        this->guide_min.y = mid - 1;
        this->guide_max.x = mid - 1;
        this->guide_max.y = mid - 1;

        //Initialize our building guide as a matrix.
        room_buildguide = new char*[dif];
        for (int i = 0; i < dif; i++)
            room_buildguide[i] = new char[dif];

        for (int r = 0; r < dif; r++){
            for (int c = 0; c < dif; c++){
                //initialize our visual guide
                room_buildguide[r][c] = '.';
            }
        }

        //This is how we make the starting point in the center of the game board
        room_buildguide[mid-1][mid-1] = 'S';

        //Here is where we randomly generate the growth of the path
        //We go from start: and every path after that is just a roll of the dice.
        gwCoordinates coords;
        coords.x = mid - 1;
        coords.y = mid - 1;

        path_coords.push_back(coords);
        int pathcount = difficulty;

        while (pathcount > 0){

            //We'll check to see if the guide is stuck, based off of last coordinate input.
            if (guideIsStuck()){
                coords = path_coords[path_coords.size()-1]; //get set of last recorded coordinates
                bad_coords.push_back(coords); //add current point to bad points vector.
                path_coords.erase(path_coords.begin() + path_coords.size()-1); //delete the point from path
                room_buildguide[coords.y][coords.x] = '.'; //reset this coordinate signifier.
                //continue to next iteration without decrementing.

            } else {
                //We iterate until the guide can find an untouched point.
                int rand_x, rand_y;

                //We get direction based off guide's last location
                Direction dir = getDirection(rand()%4);

                if (dir == LEFT ){
                    rand_x = path_coords.back().x - 1;
                    rand_y = path_coords.back().y;
                } else if (dir == RIGHT){
                    rand_x = path_coords.back().x + 1;
                    rand_y = path_coords.back().y;
                } else if (dir == UP){ //for the y
                    rand_y = path_coords.back().y - 1;
                    rand_x = path_coords.back().x;
                } else if (dir == DOWN){
                    rand_y = path_coords.back().y + 1;
                    rand_x = path_coords.back().x;
                } else continue;

                //if the direction the guide decides to build is a set of bad coordinates, we'll just continue to next loop iteration
                if (is_bad_point(rand_x, rand_y)){ continue;
                } else{

                    //If we have reached the final index and have passed from the first condition,
                    //we know the coordinate is unique and we add it to our other coordinates.
                    if (unique_point(rand_x, rand_y, path_coords)){
                        coords.x = rand_x;
                        coords.y = rand_y;
                        path_coords.push_back(coords);

                        room_buildguide[rand_y][rand_x] = (pathcount - 1 > 0)?('x'):('E');//Assign value as a path connector

                    } else continue;
                }

                //if it has reached this point, then the guide has placed a point.
                pathcount--;
            }
        }

        //We need for this to return true so that we know the map was constructed

        this->resizeGuide(); //for the sake of compressing size and saving space
                            //and making things more managable, we'll resize the guide

        //log("Exits " << __func__)
        return bool(int(path_coords.size()));
    }

};};