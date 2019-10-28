#pragma once
#include <SDL.h>
#include "../Essentials/paradigm.h"
#include "../TangObj/Animated_Obj/animatedObj.h"
#include "../Essentials/timer.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Action Classes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Actions resolved from animated objects as well as AoE Environment actions

namespace Xioudown { namespace ObjAction{
	
	using namespace Xioudown::Essentials;

	using Math::Polygon;
	using namespace TangObj;

	#ifndef log
	#define log(x) std::cout << x << std::endl;
	#endif

	class Action{

    private:

		Polygon* AOE_of_action;
		Timer *execution_ttl;
		actionType action_type;
		Uint32 cooldown;
		Essentials::Direction dir;

    public:
        Action(actionType t, Direction d = UP, int ttl = 0) : action_type(t), dir(d), cooldown(0) {
            //if the timer has not been instantiated then we will do that. It will not be overwritten.
            execution_ttl = new Essentials::Timer();
            execution_ttl->startTimer(ttl);

        }
        ~Action() { delete execution_ttl; delete AOE_of_action; }

		virtual void render(SDL_Renderer *r) = 0;
		virtual void execute() = 0;
		virtual void consequences(TangObj::AnimatedObj *a) = 0;

		void setAOE(Polygon *p) { AOE_of_action = p; }
		Timer* getActionTTL();
		Polygon *getAOE() { return AOE_of_action; }
		void setCooldown(Uint32 ttl){ cooldown = ttl; }
		Uint32 getCooldown() const{ return cooldown; }
		void setDirection(Direction d){ dir = d; }
		Direction getDirection(){ return dir; }
	};

	class MeleeAction : public Action{

		private:

		public:
			MeleeAction( TangObj::AnimatedObj *a );
			~MeleeAction() {}
			void render(SDL_Renderer *r);
			void execute();
			void consequences(TangObj::AnimatedObj *a);
	};

	class SwordAction : public Action{

		private:

		public:
			SwordAction( TangObj::AnimatedObj * a );
			~SwordAction();
			virtual void render(SDL_Renderer *r);
			virtual void execute();
			virtual void consequences(TangObj::AnimatedObj *a);
	};

};};