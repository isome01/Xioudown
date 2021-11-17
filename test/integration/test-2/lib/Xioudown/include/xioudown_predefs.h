#pragma once

#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

namespace Xioudown {

    /* Xioudown Namespace */

    class XioudownGrid;  // This class relays boundaries of the grid from points (x^0, y^0) -> (x^n, y^n)
    class XioudownGridUnit; //Anything that can be tracked on the grid

    enum unitType {
        XIOU_UI_CONFIG,
        XIOU_OBJ
    };

    namespace UI {
        class UIElementComponent; // menus, dialogue boxes - subclass of UIGridUnit
    }
    
    namespace Essentials {
        
        /* Xioudown Essentials namespace */

        class Action; //This is what each animated object will have, and will be instantiated upon inanimate item reactions
        enum EssentialUnit {
            grid_obj, grid_config
        };

        //The location of axis of which the object is moving
        enum axis{
            X, Y
        };

        //The direction of which the object is moving
        enum direction{
            UP, DOWN, LEFT, RIGHT, CENTER
        };

        enum timer{
            ACTIVE, IDLE
        };

        struct coordinates {
            int x, y;
        };

        struct rgba {
            Uint8 r, g, b, a;
        };

        struct rgb {
            Uint8 r, g, b;
        };

        //Hitpoints bar for every animated object
        struct Healthbar{
            SDL_Rect *hp_bar_max;
            SDL_Rect *hp_bar_cur;

            rgba rgb_hp_max, rgb_hp_cur;
        };

        struct TextureSheet {

            SDL_Rect frame;
            SDL_Texture *texture;

            //for the sake of circulating a sprite sheet.
            int columns, rows, iterations, sprite_x, sprite_y;
        };

        template <typename T>
        struct option_list {
            T opt_value;
            SDL_Texture* (*opt_Interface)(); //a function pointer to how the texture is rendered
            void (*onSelect)(); //an on select function pointer
        };
    };

    namespace Math {
        
    };
};

// defaults
namespace Xioudown {
    const unitType DEFAULT_XIOUDOWN_GRID_UNIT_TYPE = unitType::XIOU_OBJ;
    const Essentials::rgba DEFAULT_XIOUDOWN_GRID_RGBA = {0xff, 0xff, 0xff, 0xff};
}

#define XIOUDOWN_PREDEFS
