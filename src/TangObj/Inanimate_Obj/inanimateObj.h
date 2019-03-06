#pragma once
#include <SDL.h>
#include "../../Essentials/paradigm.h"
#include "../Animated_Obj/animatedObj.h"
#include "../tangibleObj.h"

namespace Astral { namespace TangObj{
    //This is our abstract inanimate [base] class, which is also a tangible object within the game.
    class InanimateObj : public TangibleObj
    {
            //An inanimate object is practically just a bump on a log.
        private:
            inanimateObj type;
        public:
            //Needs to be fized/changed, along with animatedObj implementation.
            InanimateObj(inanimateObj t) : TangibleObj( (t == WALL)?(STRUCTURE):(ITEM) ), type(t){}

            virtual void use() = 0;
            virtual void renderInanimateObj(SDL_Renderer* r) = 0;
            virtual void Collision(AnimatedObj*) = 0;

            inanimateObj getType()const{ return type; }
    };


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ InanimateObject subclasses ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    class Wall : public InanimateObj{
        private:


        public:
            Wall(const SDL_Rect &rect) : InanimateObj(WALL) {

                this->SetPBoundaries( new Polygon(rect) );

                if (this->getPBoundaries())
                    std::cout << "Wall Boundaries created.\n";
            }

            //Set wall thickness to ___ by default.
            Wall(short *x, short *y, int p, int t = 0) : InanimateObj(WALL) {

                if (!t)
                    this->SetPBoundaries( new Polygon(x, y, p) );
                else if (t)
                    this->SetPBoundaries( new Polygon(x, y, p, t) );
            }

            Wall(short *x, short *y) : InanimateObj(WALL) {

                this->SetPBoundaries( new Polygon(x, y) );
            }

            //To "use" a wall... Might just put up a description that can be turned off later..?
            void use() { return; }
            void renderInanimateObj(SDL_Renderer* r);
            void Collision(AnimatedObj *a);
    };


    //These are the entry mats the objects will use to traverse rooms.
    class Entry : public InanimateObj{

        private:
            Entry *other;
            int cur_room, cur_lvl;
            Direction position;
            double matScaling;

        public:
            Entry(Direction d, int scaling, int _x, int _y, int rm, int lvl );
            ~Entry();
            void setAdjacent(Entry *e){ other = e; }
            Entry* getAdjacent() const{ return other; };

            void set_lvl(int n) { cur_lvl = n; }
            int get_lvl() const{ return cur_lvl; }
            void set_room(int n) { cur_room = n; }
            int get_room() const{ return cur_room; }
            Entry* getEntry(){ return this; }
            int get_xplace();
            int get_yplace();

            void use(){}
            void renderInanimateObj(SDL_Renderer *r);
            void Collision(AnimatedObj *a); //This collision function will activate the TraverseRoom method
    };
};};