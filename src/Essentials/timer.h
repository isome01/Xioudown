#pragma once
#include <SDL.h>

namespace Astral{ namespace Essentials{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Timer Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //The reason I made my own timer class is because SDL_Timer API and reference sucks shit to follow.
    class Timer{
        public:
            Timer() : time_started(false) {} //Automatically initialize the timer to false

            void startTimer(int time){
                start = SDL_GetTicks();
                current = start;

                time_started = true; //time starts

                //Every time we start/reset the timer, we set must include a time out.
                timeOut = time;
            }

            //Just in case we have to modify the timers by outside functions (Usually they are all modified in the class' scope)
            Uint32 getCurrent(){
                current = SDL_GetTicks();
                return current;
            }

            void setCurrent(Uint32 time) { current = time; }
            Uint32 getStart()const{ return start; }
            void setStart(Uint32 time){ start = time; }
            int getTimeOut()const{ return timeOut; }
            void setTimeOut(int time){ timeOut = time; }

            //When time has elapsed passed the allotted time given
            bool timeIsOut(){
                //Get the current timer
                current = SDL_GetTicks();

                return timeOut <= int(current - start);
            }

            void timeStart(bool flag){ time_started = flag; }
            //Returns whether the timer is active or not
            bool timeStarted()const{ return time_started; }
        private:
            Uint32 start, current;
            int timeOut;
            bool time_started;
    };

};};