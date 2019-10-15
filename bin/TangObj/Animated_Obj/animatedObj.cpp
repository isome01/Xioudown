#include "../tangibleObj.h"
#include "../../Catalyst/gameCatalyst.h"
#include "../../Essentials/paradigm.h"

#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"
#include "SDL2_gfxPrimitives_font.h"

//For rand()
#include <ctime>
#include <stdlib.h>

#ifndef Xioudown_Cat
#define Xioudown_Cat Xioudown::Catalyst::gameCatalyst::gWorld()
#endif

namespace Xioudown{ namespace TangObj{

    using namespace Essentials;

    AnimatedObj::~AnimatedObj(){
        delete idle_timer;
    }

    //We then check to see if the object is going to collide with any of the animated objects within the room (We will do this with
    //every object instance.)
    void AnimatedObj::moveObj(Direction dir, Room::Room *room, int stepCount){

        //We set the direction where the object is going to move first: this happens regardless of a collision.
        setDirection(dir);

        if (stepCount == 0){

            //std::cout << "Step Count: " << stepCount << std::endl;
            stepCount = getStepCount() + getSpd(); //The amount of steps taken by the object.
        }

        else if (stepCount){

            //std::cout << "Step Count greater than 0: " << stepCount << std::endl;
        }

        //For the sake of the player we take a count of whether the player moved 0 - n amount of frames: for each x and y
        int pre_x = getX();
        int pre_y = getY();

        //If it's a negative direction (up or left)
        if (dir == UP)
            setY(getY() - stepCount); //the object moves up x amount of steps

        if (dir == LEFT)
            setX(getX() - stepCount); //the object moves left x amount of steps

        //Or if it's a positive direction
        if (dir == DOWN)
            setY(getY() + stepCount); //the object moves down x amount of steps

        if (dir == RIGHT)
            setX(getX() + stepCount); //the object moves right x amount of steps

        //Now we see if the object is colliding with any of the animated objects within the game, if any animated objects.
        if (room->processCollisions(this)){
            steps_taken_x = 0;
            steps_taken_y = 0;
            return;
        }

        //the amount of steps taken will be recorded
        steps_taken_x = (pre_x - getX());
        steps_taken_y = (pre_y - getY());
    }

    //A decision attribute for all COM player animated objects.
    void AnimatedObj::processDecision(Room::Room *room){

        //first we check if the object is alive
        if (this->getHp() <= 0){
            isObjAlive(false);
            log("Animated object has perished.")
            return;
        }

        if (!this->isCOMPlayer()) return; //Problem solved within distinguishing between the real player.

        //If the COM was just spawned and doesn't have time started
        if (!this->idle_timer->timeStarted())
            this->idle_timer->startTimer((rand()%1000) + 2000); //Automatically set it to 2 seconds after the COM has spawned

        //If the COM had used up the amount of steps it had.
        else if (!this->getStepTotal()){
            //We reset the time
            this->idle_timer->startTimer((rand()%1000) + 2000);
            //and step count goes back up
            this->setStepTotal(rand()%50 + 10);
            //We set the direction again
            this->setDirection(room->getDirection(rand()%4));

            this->idle_timer->startTimer((rand()%1000) + 2000); //Then we restart the timer with a random timeout
        }

        //If the COM has started its timer and has spawned before, then we will check to see if their time is up to make a move
        if (this->idle_timer->timeIsOut() && this->getStepTotal() >= 0){
            //For now, we will just have them move in different directions
            this->moveObj(this->getDirection(), room);
            this->setStepTotal(this->getStepTotal() - 1);
        }

    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~KNIGHT FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Knight::Knight(bool COM_flag) : AnimatedObj(KNIGHT, COM_flag){

        //making every knight spectral
        isSpectral(true);

        //Setting stats for our knight obj
        setAtk(1);
        setDef(1);
        setSpd(1);
        setHpLimit(8);
        setHp(getHpLimit());
        setStepCount(0);
        setLevel(1);
        setA(0);
        AnimatedObj::setSpawned(false);

        //for rasterizing/rendering to SDL_Rect
        setW(50);
        setH(50);

        //Creating healthbar for knight
        this->HpBar()->hp_bar_cur = new SDL_Rect;
        this->HpBar()->hp_bar_max = new SDL_Rect;

        //start drawing all attributes of the hp bar
        this->HpBar()->hp_bar_max->w = this->getBoundaries()->w * 2;
        this->HpBar()->hp_bar_max->h = 20;
        this->HpBar()->hp_bar_cur->w = this->getBoundaries()->w * 2;
        this->HpBar()->hp_bar_cur->h = 20;

        //TangibleObj::SetPBoundaries( &(*this->getBoundaries() ) );

        //setting the amount of steps a knight can take
        setStepCount(5);
    }

    void Knight::renderAnimObj(SDL_Renderer *r){

        if (!this->isSpawned()){
            //Rendering the spawning animation for Knight
            Uint8 alpha = (getA() + 2 > 255)?(1):(2);
            setA( getA() + alpha);

            if (this->getA() >= 255) this->setSpawned(true);
        }

        if (this->getPBoundaries()){
            //This works perfectly!
            log("Renders polygon")
            filledPolygonRGBA(r,
                            this->getPBoundaries()->X(),
                            this->getPBoundaries()->Y(),
                            4, 0xff, 0xff, 0xff, this->getA());
        } else if( this->getBoundaries() ) {

            SDL_SetRenderDrawColor(r, 0x00, 0xff, 123, 100);
            SDL_RenderFillRect(r, getBoundaries());

        } else log("Inapt to render.")

        //if the hp bar exists for target
        if (this->HpBar()->hp_bar_max && this->HpBar()->hp_bar_cur){

            //start drawing the position of the bar
            this->HpBar()->hp_bar_max->x = this->getX() + ((this->getBoundaries()->w - this->HpBar()->hp_bar_max->w) / 2);
            this->HpBar()->hp_bar_max->y = this->getY() - 40;
            this->HpBar()->hp_bar_cur->x = this->HpBar()->hp_bar_max->x;
            this->HpBar()->hp_bar_cur->y = this->getY() - 40;

            //Then determine the length of the bar for UI
            this->HpBar()->hp_bar_cur->w = ( double(this->getHp() / this->getHpLimit()) * double(this->HpBar()->hp_bar_max->w) );

            SDL_SetRenderDrawColor(r, 0, 0xff, 0, 0xff);
            SDL_RenderFillRect(r, HpBar()->hp_bar_cur);
            SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
            SDL_RenderDrawRect(r, HpBar()->hp_bar_max);

        }
    }

    void Knight::Collision(AnimatedObj *a){

        /*
        //We set the object at the same amount of steps the object has taken
        Direction dir = a->getDirection();
        int steps = a->getSpd() + a->getStepCount();

        if (dir == DOWN){
            a->setY( a->getY() - steps);
        } else if (dir == RIGHT){
            a->setX( a->getX() - steps);
        } else if (dir == LEFT){
            a->setX( steps + a->getX());
        } else if ( dir == UP ){
            a->setY( steps + a->getY());
        } else log("Invalid direction")
        */
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~NORMAL MAN FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    NormalMan::NormalMan(bool COM_flag) : AnimatedObj(NORMAL_MAN, COM_flag){

        //to make sure that each man object is spectral
        isSpectral(true);

        setAtk(1);
        setDef(1);
        setSpd(2);
        setHpLimit(8);
        setHp(getHpLimit());
        setStepCount(0);
        setLevel(1);
        setA(0);
        AnimatedObj::setSpawned(false);
        //Creating the size of our normal man avatar
        setW(150);
        setH(120);

        //Hp bar for normal man
        this->HpBar()->hp_bar_cur = new SDL_Rect;
        this->HpBar()->hp_bar_max = new SDL_Rect;

        //start drawing all attributes of the hp bar
        this->HpBar()->hp_bar_max->w = this->getBoundaries()->w * 2;
        this->HpBar()->hp_bar_max->h = 20;
        this->HpBar()->hp_bar_cur->w = this->getBoundaries()->w * 2;
        this->HpBar()->hp_bar_cur->h = 20;

        //Loading the sprite for every normal man.
        gwTexture_Sheet src = { {0, 0 ,31, 27}, Catalyst::gameCatalyst::gWorld()->getManager()->getTexture(0) };
        this->setSprite( new Essentials::gwTexturePack(this->getBoundaries(), &src) );

        //Setting the step count for a normal man
        setStepCount(2);
    }

    void NormalMan::renderAnimObj(SDL_Renderer *r){

        if (!this->isSpawned()){
            //Rendering the spawning animation for Normal Man
            int alpha = (getA() + 2 > 255)?(1):(2);
            setA( getA() + alpha);

            if (this->getA() >= 255) this->setSpawned(true);
            SDL_SetTextureAlphaMod(getSprite()->getTexture(), this->getA());
        }

        if (this->getPBoundaries()){
            //This works perfectly!
            log("Renders polygon")
            filledPolygonRGBA(r,
                            this->getPBoundaries()->X(),
                            this->getPBoundaries()->Y(),
                            4, 0xff, 0xff, 0xff, this->getA());
        } else if(this->getBoundaries() ) {

            if ( !this->getSprite()->getTexture() ){

                //Try and set the texture again.
                this->getSprite()->setTexture( Xioudown_Cat->getManager()->getTexture(0));
                //log("Texture not loaded.")
                return;
            }

            this->getSprite()->render(r, getBoundaries());

        } //else log("Inapt to render.")

        //if the hp bar exists for target
        if (this->HpBar()->hp_bar_max && this->HpBar()->hp_bar_cur){

            //start drawing the position of the bar
            this->HpBar()->hp_bar_max->x = this->getX() + ((this->getBoundaries()->w - this->HpBar()->hp_bar_max->w) / 2);
            this->HpBar()->hp_bar_max->y = this->getY() - 40;
            this->HpBar()->hp_bar_cur->x = this->HpBar()->hp_bar_max->x;
            this->HpBar()->hp_bar_cur->y = this->getY() - 40;

            //Then determine the length of the bar for UI
            this->HpBar()->hp_bar_cur->w = ( double(double(this->getHp()) / double(this->getHpLimit()) ) * double(this->HpBar()->hp_bar_max->w) );

            SDL_SetRenderDrawColor(r, 0, 0xff, 0, 0xff);
            SDL_RenderFillRect(r, HpBar()->hp_bar_cur);
            SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
            SDL_RenderDrawRect(r, HpBar()->hp_bar_max);

        }
    }

    void NormalMan::Collision(AnimatedObj *a){

        /*
        //We set the object at the same amount of steps the object has taken
        Direction dir = a->getDirection();
        int steps = a->getSpd() + a->getStepCount();

        if (dir == DOWN){
            a->setY( a->getY() - steps);
        } else if (dir == RIGHT){
            a->setX( a->getX() - steps);
        } else if (dir == LEFT){
            a->setX( steps + a->getX());
        } else if ( dir == UP ){
            a->setY( steps + a->getY());
        } else log("Invalid direction")*/
    }
};};