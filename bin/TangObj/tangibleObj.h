#pragma once
#include <SDL.h>
#include <iostream>
#include "../Essentials/paradigm.h"
#include "../Essentials/objTextureSheet.h"
#include "../Essentials/timer.h"
#include "../Math/polygon.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Tangible Objects ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//This defines the holistic view of whether an object is a physical object within the game: base class.

namespace Xioudown{ namespace TangObj{

    #ifndef log
    #define log(x) std::cout << "Tangible obj " << x << std::endl;
    #endif

    using namespace Math;
    using namespace Essentials;
    /*using Essentials::tangibleObj;
    using Essentials::inanimateObj;
    using Essentials::animateObj;
    using Essentials::Healthbar;
    using Essentials::Axis;
    using Essentials::Direction;
    using Essentials::RenderColorA;
    using Essentials::gwCoordinates;
    using Essentials::Timer;*/

    class TangibleObj{
    private:
        tangibleObj objType; //The yield for what type of object our tangible object is.
        Math::Polygon *p_boundaries; //The physical square/rectangle boundary lines
        SDL_Rect boundaries; //alternate set of boundaries
        Healthbar obj_hp_bar; //every object will have a pointer to a hp bar; may be available, may be null.
        Essentials::gwTexturePack *obj_sprite;//The texture for the object sprite.

        RenderColorA obj_rgba;
        int obj_num; // This is to keep track of what object is which whenever there is iteration

        //The status of where the object is facing
        Axis axis;
        Direction direction;
        bool spectral; //A value that states whether an object can be passed through
        bool isAlive; //a flag to simply let the object know if it is alive.

    public:
        TangibleObj(tangibleObj type) : objType(type), p_boundaries(NULL), spectral(false) {
            obj_rgba.a = 255; obj_rgba.r = 0; obj_rgba.b = 0; obj_rgba.g = 0;
            obj_hp_bar.hp_bar_max = NULL; obj_hp_bar.hp_bar_cur = NULL;

            obj_sprite = NULL;
            isAlive = true;
        }
        ~TangibleObj();

        tangibleObj getObjType()const{ return objType; }
        void setObjType(tangibleObj type) { objType = type; }

        //Getters for the objects center x and y coordinates
        int getcenterX() const{
            return boundaries.x + (boundaries.w / 2);
        }
        int getcenterY() const{
            return boundaries.y + (boundaries.h / 2);
        }

        //Getters and setters for the object's boundaries and positioning
        SDL_Rect *getBoundaries(){ return &boundaries; }
        Math::Polygon* getPBoundaries() {return p_boundaries; }
        void SetPBoundaries(Math::Polygon *p) { p_boundaries = p; }
        Axis getAxis() const{ return axis; }
        void setAxis(Axis xy) { axis = xy; }
        Direction getDirection() const{ return direction; }
        void setDirection(Direction dir){ direction = dir; }

        //Robust getters and setters for our SDL_Rect/boundaries and alpha
        void setX(int n);
        int getX()const;
        void setY(int n);
        int getY()const;
        gwCoordinates getCenter();

        void setH(int n) { boundaries.h = n; }
        int getH()const{ return boundaries.h; }
        void setW(int n) { boundaries.w = n; }
        int getW()const{ return boundaries.w; }
        void setA(int a) { obj_rgba.a = a; }
        int getA() const{ return obj_rgba.a; }
        void setRGB(int r, int g, int b){ obj_rgba.r = r; obj_rgba.g = g; obj_rgba.b = b; }
        RenderColorA getRGBA() const{ return obj_rgba; }
        void setObjNum(int n) { obj_num = n; }
        int getObjNum()const{ return obj_num; }
        bool isSpectral() const{ return spectral; } //returns whether the object is passable.
        void isSpectral(bool f) { spectral = f; }
        //try to make isTouching virtual: multiple interactions can happen

        //Here is the collision function for all objects: something will happen upon collision.
        bool isTouching(const SDL_Rect& obj)const; //If an object is engaging another object, then we will return true;
        bool isTouching(Math::Polygon *p)const;

        bool isObjAlive()const{ return isAlive; } // a getter to inform if the object is alive
        void isObjAlive(bool f){ isAlive = f; } // a getter to inform if the object is alive

        Healthbar *HpBar(){ return &obj_hp_bar; }

        //Getter and setter for object sprite
        Essentials::gwTexturePack *getSprite(){ return obj_sprite; }
        void setSprite(Essentials::gwTexturePack *g) { obj_sprite = g; }

        //A little bit of value semantics for our tangible obj class: wouldn't want value semantics between a warthog and a warrior, now would we?
        TangibleObj(const TangibleObj &other);
        void operator=(const TangibleObj &other);
    };

};};