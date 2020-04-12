#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

//~~~~~~~~~~ This header file contains enum and other arbitrary data-types

namespace Xioudown { namespace Room{
    class Room;
};};

namespace Xioudown { namespace Essentials{

    class Action; //This is what each animated object will have, and will be instantiated upon inanimate item reactions
                //(like item consumption)
    class Replenish; //An action based on healing effect
    class MeleeAction; //Action based on melee effect/damage

    class Perishable; //a child class that is an inanimate object
    class Weapon; //a child class that is an inanimate object
    class Apparrel; //a child class that is an inanimate object

    //static vector<Pathway*> DEFAULT_PATHS;

    //For the sake of 
    enum EssentialUnit {
        inGame, gameConfig
    };

    class GridUnit;
    class Grid;

    //The location of axis of which the object is moving
    enum Axis{
        X, Y
    };

    //The direction of which the object is moving
    enum Direction{
        UP, DOWN, LEFT, RIGHT, CENTER
    };

    enum timer{
        ACTIVE, IDLE
    };

    enum tangibleObj{
        HUMAN, BEAST, MONSTER, ITEM, STRUCTURE, PLANT
    };

    //The types of animated objects are yet to be determined
    enum animateObj{
        KNIGHT, NORMAL_MAN, NONE
    };

    //The types of inanimate objects (i.e potions, herbs, weapons, etc.: anything that doesn't have a move function,
    //but is a tangible object)
    enum inanimateObj{
        POTION, WALL, ENTRY, WEAPON, APPARREL
    };

    enum roomType{

    };

    enum actionType{
        MELEE,
        ONEHAND_ACTION,
        TWOHAND_ACTION,
        RANGED_ACTION,
        SPELL_ACTION,
        REPLENISH, TOXIFY, HEMORRHAGE,
    };

    struct gwCoordinates{
        int x, y;
    };

    struct RenderColorA{
        Uint8 r, g, b, a;
    };

    //Hitpoints bar for every animated object
    struct Healthbar{
        SDL_Rect *hp_bar_max;
        SDL_Rect *hp_bar_cur;

        RenderColorA rgb_hp_max, rgb_hp_cur;
    };

    struct gwTexture_Sheet{

        SDL_Rect frame;
        SDL_Texture *texture;

        //for the sake of circulating a sprite sheet.
        int columns, rows, iterations, sprite_x, sprite_y;
    };

    template <typename T>
    struct option_list{
        T opt_value;
        SDL_Texture* (*opt_Interface)(); //a function pointer to how the texture is rendered
        void (*onSelect)(); //an on select function pointer
    };
};};