#include <SDL.h>
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"
#include "SDL2_gfxPrimitives_font.h"

#include "inanimateObj.h"
#include "../../Catalyst/gameCatalyst.h"

#ifndef log
#define log(x) std::cout << "Entry obj " << x << std::endl;
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Entry Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Xioudown { namespace TangObj{


    Entry::Entry(Direction d, int scaling, int x, int y, int rm, int lvl ) :
        InanimateObj(ENTRY), cur_lvl(lvl), cur_room(rm), position(d), matScaling(scaling) {

        matScaling /= 2;
        isSpectral(true);
        setRGB(0, 255, 0);
        setA(255);

        SDL_Rect *r = this->getBoundaries();

        this->getBoundaries()->x = x + matScaling / 2;
        this->getBoundaries()->y = y + matScaling / 2;

        if ( d == UP || d == DOWN ){

            this->getBoundaries()->w = matScaling;
            this->getBoundaries()->h = matScaling / 2;

            if (d == UP){
                this->getBoundaries()->y = this->getBoundaries()->y + (matScaling / 2) + r->h;
            } else if ( d == DOWN){
                this->getBoundaries()->y = y;
            }

        } else if ( d == LEFT || d == RIGHT ) {

            this->getBoundaries()->w = matScaling / 2;
            this->getBoundaries()->h = matScaling;

            if (d == LEFT){
                this->getBoundaries()->x = r->x + (matScaling / 2) + r->w;
            } else if (d == RIGHT){
                this->getBoundaries()->x = x;
            }
        } else if ( d == CENTER) {

            this->getBoundaries()->w = matScaling;
            this->getBoundaries()->h = matScaling;
        }
    }

    Entry::~Entry(){

    }

    void Entry::renderInanimateObj(SDL_Renderer *r){

        RenderColorA rgba = getRGBA();
        if (getPBoundaries()){
            filledPolygonRGBA( r, getPBoundaries()->X(),
                                    getPBoundaries()->Y(),
                                    getPBoundaries()->getVertices(),
                                    rgba.r, rgba.g, rgba.b, rgba.a
                                    );
        } else {
            SDL_SetRenderDrawColor(r, rgba.r, rgba.g, rgba.b, rgba.a);
            SDL_RenderFillRect(r, getBoundaries());
        }
    }

    void Entry::Collision(AnimatedObj *a){
        //Here we will request the traverse room method
        Entry *e = this->getAdjacent();

        if (e){
            log("Entry found")
            //We'll go ahead and make a queue request for the current room and level to the location of the adjacent room's
            Catalyst::gameCatalyst::gWorld()->queueTransition( &(*a), e, e->get_room(), e->get_lvl());

            //Then change the location of the object that touched the entry to be side-by-side the entry

            log("Gets here...")
            a->setX(e->get_xplace());
            a->setY(e->get_yplace());

            log("Other X: " << e->getX())
            log("Other Y: " << e->getY())

            log( "Obj The X: "<< a->getX() << "\nThe Y: " << a->getY() )
            log( "Entry The X: "<< e->getX() << "\nThe Y: " << e->getY() )
            log( "\nThe Level: " << e->get_lvl() << "\nThe Room: " << e->get_room())
        } else log("Entry not found");
    }

    int Entry::get_xplace(){
        if (position == UP || position == DOWN){

            //initialize position of animated object appearance
            return this->getBoundaries()->x + (this->getBoundaries()->w / 2);

        } else if (position == LEFT || position == RIGHT) {

            if (position == LEFT)
                return getBoundaries()->x - (getBoundaries()->w - 1);
            else if (position == RIGHT)
                return getBoundaries()->x + (getBoundaries()->w + 1);

        } else if ( position == CENTER) {

            return this->getBoundaries()->x + (this->getBoundaries()->w / 2);
        }

        return -1;
    }

    int Entry::get_yplace(){
        if (position == UP || position == DOWN){

            if (position == UP){
                return getBoundaries()->y - (getBoundaries()->h - 1);
            } else if ( position == DOWN){
                return getBoundaries()->y + (getBoundaries()->h + 1);
            }

        } else if (position == LEFT || position == RIGHT) {

            //initialize position of animated object appearance
            return getBoundaries()->y + (getBoundaries()->h / 2);

        } else if (position == CENTER) {

            return getBoundaries()->y + getBoundaries()->h + 1;
        }

        return -1;
    }
};};
