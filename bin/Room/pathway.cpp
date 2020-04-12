#include "room.h"
#include "../Math/polygon.h"
#include "../Essentials/paradigm.h"
#include "../Essentials/objTextureSheet.h"
#include "../Catalyst/gameCatalyst.h"
#include "../Catalyst/gameWindow.h"
#include "../TangObj/tangibleObj.h"

#include <stdlib.h>
#include <iostream>

#ifndef log
#define log(x) std::cout <<"Pathway Obj: from "<< __func__ << " " << x << std::endl;
#endif

namespace Xioudown { namespace Room {
    using namespace Xioudown::TangObj;
    using namespace Essentials;
    using namespace Catalyst;

    //Our construction of a pathway object - this is what makes a level a level: very exciting
    Pathway::Pathway( double _x, double _y, std::vector<Direction> dirs, int _w, int _h) : width(_w), height(_h) {

        //Initialize the walkway "Rect"
        walkway.x = _x;
        walkway.y = _y;
        walkway.w = width;
        walkway.h = height;

        //Creating walls according to directions input:
        int sz = dirs.size();

        //We'll create a polygon from a line - as given.
        for (int i = 0; i < sz; i++){

            if ( dirs[i] == UP ){

                short xs[2] = {(short)walkway.x, ((short)(walkway.x + width))};
                short ys[2] = {(short)walkway.y, (short)walkway.y};
                walls.push_back( new Wall(xs, ys) );

            } else if( dirs[i] == DOWN){

                short xs[2] = {(short)walkway.x, (short)(walkway.x + width)};
                short ys[2] = {(short)(walkway.y + height), (short)(walkway.y + height)};
                walls.push_back( new Wall(xs, ys));

            } else if ( dirs[i] == LEFT ){

                short xs[2] = {(short)walkway.x, (short)walkway.x };
                short ys[2] = {(short)walkway.y, (short)(walkway.y + walkway.h)};
                walls.push_back( new Wall(xs, ys));

            } else if ( dirs[i] == RIGHT ){

                short xs[2] = { (short)(walkway.x + width), (short)(walkway.x + width)};
                short ys[2] = { (short)walkway.y, (short)(walkway.y + walkway.h)};
                walls.push_back( new Wall(xs, ys));

            } else{
                log("Not a valid direction. (Index " << i << ").")
                continue;
            }
        }

        gwTexture_Sheet src = { {0, 0, 160, 160}, gameCatalyst::gWorld()->getManager()->getTexture(1) };
        this->tiles = new Essentials::gwTexturePack(&walkway, &src, 4, 4);
    }


    Pathway::~Pathway(){

    }

    void Pathway::render(SDL_Renderer* r){

        if (gameCatalyst::gWorld()->getWindow()->aptToRender(walkway) ){
            /*SDL_SetRenderDrawColor(r, 0xff, 0x00, 0x00, 0xff);
            SDL_RenderFillRect(r, &walkway);*/
            tiles->render(r, &walkway);
        }// else log("inapt to render tiling.")
    }


    EntryPackage::EntryPackage(Direction d, int rm, int lvl, int _x, int _y){
        //entry = new Entry(d, rm, lvl, _x, _y);
    }

};};