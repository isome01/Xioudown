#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.c>
#include <SDL2_rotozoom.c>
#include "../Math/polygon.h"
#include "./tangibleObj.h"

#define log(x) std::cout << "Tangible obj " << x << std::endl;

//Implementation of Tangible Object Base Class

namespace Astral{ namespace TangObj{
        
    TangibleObj::~TangibleObj(){
        delete p_boundaries;
    }

    //if polygon boundaries exist for this object -
    void TangibleObj::setX(int n){

        if (p_boundaries)
            p_boundaries->X(n);
        else boundaries.x = n;
    }
    int TangibleObj::getX()const{

        if (p_boundaries)
            return 0;

        return boundaries.x;
    }
    void TangibleObj::setY(int n){

        if (p_boundaries)
            p_boundaries->Y(n);
        else boundaries.y = n;
    }
    int TangibleObj::getY()const{

        if (p_boundaries)
            return 0;

        return boundaries.y;
    }

    gwCoordinates TangibleObj::getCenter(){

        gwCoordinates c;
        c.x = p_boundaries->getCenterX();
        c.y = p_boundaries->getCenterY();

        return c;
    }

    //This is where we take care of every object's collision - if some happens that
    //Our object touches another.
    bool TangibleObj::isTouching(const SDL_Rect &obj)const{

        //If we have an alternate set of boundaries in use instead of our SDL_Rect
        if (!this->p_boundaries){

                //If our oncoming object is meeting the object...
            if (((this->boundaries.x >= obj.x) && (this->boundaries.x <= obj.x + obj.w)) &&
                ((this->boundaries.y >= obj.y) && (this->boundaries.y <= obj.y + obj.h))) //from top left corner
                return true;

            if (((this->boundaries.x + this->boundaries.w >= obj.x) && (this->boundaries.x + this->boundaries.w <= obj.x + obj.w)) &&
                ((this->boundaries.y + this->boundaries.h >= obj.y) && (this->boundaries.y + this->boundaries.h <= obj.y + obj.h))) //from top right corner
                return true;

            if (((this->boundaries.x >= obj.x) && (this->boundaries.x <= obj.x + obj.w)) &&
                ((this->boundaries.y + this->boundaries.h >= obj.y) && (this->boundaries.y + this->boundaries.h <= obj.y + obj.h))) //Bottom left corner
                return true;

            if (((this->boundaries.x + this->boundaries.w >= obj.x) && (this->boundaries.x + this->boundaries.w <= obj.x + obj.w)) && //Bottom right corner
                ((this->boundaries.y >= obj.y) && (this->boundaries.y <= obj.y + obj.h)))
                return true;
        } else {
            if (p_boundaries->inside(obj.x, obj.y) ) return true;
            if (p_boundaries->inside(obj.x, obj.y + obj.h) ) return true;
            if (p_boundaries->inside(obj.x + obj.w, obj.y) ) return true;
            if (p_boundaries->inside(obj.x + obj.w, obj.y + obj.h) ) return true;
        }

        return false; //anything else, we return false
    }

    bool TangibleObj::isTouching(Math::Polygon *p)const{
        if (!this->p_boundaries){
            if (p->inside(this->boundaries.x, this->boundaries.y)) return true;
            if (p->inside(this->boundaries.x, this->boundaries.y + this->boundaries.h)) return true;
            if (p->inside(this->boundaries.x + this->boundaries.w, this->boundaries.y)) return true;
            if (p->inside(this->boundaries.x + this->boundaries.w, this->boundaries.y + this->boundaries.h)) return true;
        } else {

            for (int i = 0; i < this->p_boundaries->getVertices(); i++){
                //Easy. Just run each point of the polygon through the inside function of the target polygon
                if (p->inside((int)this->p_boundaries->X()[i], (int)this->p_boundaries->Y()[i]) )
                    return true;
            }
        }

        return false;
    }
};};