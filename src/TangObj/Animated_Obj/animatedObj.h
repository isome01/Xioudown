#pragma once
#include <SDL.h>
#include "../tangibleObj.h"
#include "../../Essentials/timer.h"

namespace Astral { namespace TangObj {
    //This is our abstract animated [base] class, which is also a tangible object within the game.
    class AnimatedObj : public TangibleObj
    {
        private:
            animateObj animType;
            Timer *idle_timer;
            Timer *attack_cooldown;
            int attack, defense, speed, hpcount, hplimit, level, agroCount, steps, stepsTotal,steps_taken_x, steps_taken_y;
            bool agrFlag, COMPlayer, fully_spawned;
            bool isAttacking;

        public:
            AnimatedObj(animateObj type, bool COM_flag) : TangibleObj( (type == KNIGHT || type == NORMAL_MAN)?(HUMAN):(MONSTER) ), animType(type),
                        isAttacking(false) { //Then we instantiate the timer

                idle_timer = new Timer();
                setCOMPlayer(COM_flag);
                setX(-1);
                setY(-1);

                attack_cooldown = new Timer();
            }

            ~AnimatedObj();

            //Purely Virtual function declarations:
            virtual void renderAnimObj(SDL_Renderer* r) = 0; //This will render the animate object differently in size, position, and shape.
            virtual bool isAgro() = 0; //This function will let us know if/when the animate object is "agro" (aggressively active)
            virtual void Collision(AnimatedObj *a) = 0; //also processes, by making a search through the monsters in the room, the reaction of collision. O(n)

            void moveObj(Direction dir, Room::Room *room, int stepCount=0); //Whenever the object decides to move

            //Setter and getters for Animate Object stats
            void setAtk(int n){ attack = n; }
            int getAtk()const{ return attack; }
            void setDef(int n){ defense = n; }
            int getDef()const{ return defense; }
            void setSpd(int n){ speed = n; }
            int getSpd()const{ return speed; }
            void setHp(int n){ hpcount = n; }
            int getHp()const{ return hpcount; }
            void setHpLimit(int n){ hplimit = n; }
            int getHpLimit()const{ return hplimit; }
            void setLevel(int n){ level = n; }
            int getLevel()const{ return level; }
            void setStepCount(int n){ steps = n; }
            int getStepCount()const { return steps; }

            animateObj getType()const{ return animType; }

            int getStepTotal()const{ return stepsTotal; }
            void setStepTotal(int n){ stepsTotal = n; }

            int Xsteps()const{ return steps_taken_x; }
            void Xsteps(int n){ steps_taken_x = n; }
            int Ysteps()const{ return steps_taken_y; }
            void Ysteps(int n){ steps_taken_y = n; }

            void setAgro(bool tf) { agrFlag = tf; }
            bool getAgro()const{ return agrFlag; }
            void willAttack(Uint32 ttl){
                //this is where we will control whether the cooldown has finished.
                if (!attack_cooldown->timeStarted()){
                    attack_cooldown->startTimer(ttl);
                    isAttacking = false;
                    return;

                } else if (attack_cooldown->timeIsOut() && !isAttacking){
                    isAttacking = true;
                    attack_cooldown->timeStart(false);

                } /*else log("cooldown time still left")*/
            }
            bool willAttack(){ return isAttacking; }
            void setCOMPlayer(bool flag) { COMPlayer = flag; }
            const bool isSpawned() const{ return fully_spawned; }
            void setSpawned(bool flag) { fully_spawned = flag; }
            void Continue_Spawning(); //This function will control the alpha and spawn status of our animated object.

            //Strictly COM player attributes
            bool isCOMPlayer()const{ return COMPlayer; } //returns the COM player state.
            void processDecision(Room::Room *room); //This is where we process the COM's decisions.

            void operator=(const AnimatedObj &a);
    };


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Animated Object subclasses ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    class Knight : public AnimatedObj
    {
        public:
            Knight(bool COM_flag);
            void renderAnimObj(SDL_Renderer* r);
            void Collision(AnimatedObj *a);
            bool isAgro() { return getAgro(); }
    };

    class NormalMan : public AnimatedObj
    {
        public:
            NormalMan(bool COM_flag);
            void renderAnimObj(SDL_Renderer* r);
            void Collision(AnimatedObj *a);
            bool isAgro() { return getAgro(); }
    };
}; };