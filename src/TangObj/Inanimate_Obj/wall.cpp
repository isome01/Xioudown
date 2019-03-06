#include <SDL.h>
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"
#include "SDL2_gfxPrimitives_font.h"

#include "inanimateObj.h"
#include "../Animated_Obj/animatedObj.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Wall Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Astral{ namespace TangObj{
    void Wall::renderInanimateObj(SDL_Renderer *r){

        if (this->getPBoundaries()){
            filledPolygonRGBA(r,
                        this->getPBoundaries()->X(),
                        this->getPBoundaries()->Y(),
                        this->getPBoundaries()->getVertices(),
                        0xff, 0xff, 0xff, 0xff);
        }
    }

    //collision effect for any object to cross a wall
    void Wall::Collision(AnimatedObj *a){

        Direction dir = a->getDirection();
        int steps = a->getSpd() + a->getStepCount();

        //We set the object at the same amount of steps the object has taken
        if (dir == DOWN){
            a->setY( a->getY() - steps);
        } else if (dir == RIGHT){
            a->setX( a->getX() - steps);
        } else if (dir == LEFT){
            a->setX( steps + a->getX());
        } else if ( dir == UP ){
            a->setY( steps + a->getY());
        } else log("Invalid direction")
    }

};};