#include <SDL.h>
#include <SDL_image.h>
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"
#include "SDL2_gfxPrimitives_font.h"

#include "../Essentials/timer.h"
#include "../TangObj/Animated_Obj/animatedObj.h"
#include "../TangObj/Inanimate_Obj/inanimateObj.h"
#include "../Math/polygon.h"
#include "game_actions.h"

namespace Astral{ namespace ObjAction{
    using namespace Essentials;
    using namespace Math;
    using TangObj::AnimatedObj;
    using TangObj::InanimateObj;

    //Action methods
    Timer* Action::getActionTTL(){
        if (execution_ttl != NULL){
            return execution_ttl;
        } else log("Timer is Null.")

        return NULL;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Melee Action Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MeleeAction::MeleeAction(AnimatedObj *a) : Action(MELEE, BASE, a->getDirection(), 2000){

        //first determine the direction of the action
        SDL_Rect *r = a->getBoundaries();

        short xs[4] = { r->x, r->x, r->x + (r->w * 2), r->x + (r->w * 2) };
        short ys[4] = { r->y + (r->h * 2), r->y, r->y, r->y + (r->h * 2) };

        this->setAOE( new Polygon(xs, ys, 4) );

        Polygon *p = this->getAOE();

        if (a->getDirection() == UP){
            p->Y(-1 * (r->h * 2));
        } else if (a->getDirection() == DOWN) {
            p->Y(r->h );
        } else if (a->getDirection() == LEFT) {
            p->X(-1 * (r->w * 2) );
        } else if (a->getDirection() == RIGHT) {
            p->X(r->w );
        } else { //if anything else, we will default to an up direction
            p->X(0);
        }

        //set action cooldown
        this->setCooldown(10);
    }

    void MeleeAction::render(SDL_Renderer *r){
        if (this->getAOE()){
            polygonRGBA( r, this->getAOE()->X(),
                                this->getAOE()->Y(),
                                this->getAOE()->getVertices(),
                                0xff, 0xff, 0xff, 0xff);
        } else log("Unrenderable")
    }

    void MeleeAction::execute(){

        if ( getDirection() == UP)
            getAOE()->Y(-12);
        else if( getDirection() == LEFT)
            getAOE()->X(-12);
        else if( getDirection() == RIGHT)
            getAOE()->X(12);
        else if (getDirection() == DOWN)
            getAOE()->Y(12);
        else log("Invalid direction.")
    }

    void MeleeAction::consequences(AnimatedObj *a){
        if (!a->getHp() <= 0){
            a->setHp( a->getHp() - 1 );
            log("Hp left: " << a->getHp());
        }
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Sword Action Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    SwordAction::SwordAction( AnimatedObj* a ) : Action(ONEHAND_ACTION) {

    }

    void SwordAction::render(SDL_Renderer *r){

    }

    void SwordAction::execute(){

    }

    void SwordAction::consequences(TangObj::AnimatedObj *a){

    }

};};

