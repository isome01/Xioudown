#pragma once

#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

namespace Xioudown {

    /* Xioudown Namespace */

    class XioudownClass{
        
    }; // Base class for any class that wants 
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
            AXIS_X, AXIS_Y
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

        /* Keyboard Key definitions */
        enum KEYBOARD_KEY {
            ETC = SDLK_UNKNOWN,
            ENTER = SDLK_RETURN,
            ESCAPE = SDLK_ESCAPE,
            BACKSPACE = SDLK_BACKSPACE,
            TAB = SDLK_TAB,
            SPACE = SDLK_SPACE,
            EXCLAMATION = SDLK_EXCLAIM,
            DOUBLE_QUOTE = SDLK_QUOTEDBL,
            HASH = SDLK_HASH,
            PERCENT = SDLK_PERCENT,
            DOLLAR = SDLK_DOLLAR,
            AMPERSAND = SDLK_AMPERSAND,
            QUOTE = SDLK_QUOTE,
            LEFT_PAREN = SDLK_LEFTPAREN,
            RIGHT_PAREN = SDLK_RIGHTPAREN,
            ASTERISK = SDLK_ASTERISK,
            PLUS = SDLK_PLUS,
            COMMA = SDLK_COMMA,
            MINUS = SDLK_MINUS,
            PERIOD = SDLK_PERIOD,
            SLASH = SDLK_SLASH,
            A = SDLK_a,
            B = SDLK_b,
            C = SDLK_c,
            D = SDLK_d,
            E = SDLK_e,
            F = SDLK_f,
            G = SDLK_g,
            H = SDLK_h,
            I = SDLK_i,
            J = SDLK_j,
            K = SDLK_k,
            L = SDLK_l,
            M = SDLK_m,
            N = SDLK_n,
            O = SDLK_o,
            P = SDLK_p,
            Q = SDLK_q,
            R = SDLK_r,
            S = SDLK_s,
            T = SDLK_t,
            U = SDLK_u,
            V = SDLK_v,
            W = SDLK_w,
            X = SDLK_x,
            Y = SDLK_y,
            Z = SDLK_z,
            ONE = SDLK_1,
            TWO = SDLK_2,
            THREE = SDLK_3,
            FOUR = SDLK_4,
            FIVE = SDLK_5,
            SIX = SDLK_6,
            SEVEN = SDLK_7,
            EIGHT = SDLK_8,
            NINE = SDLK_9
        };
    };

    namespace Math {
        
    };
};

// defaults
namespace Xioudown {
    const unitType DEFAULT_XIOUDOWN_GRID_UNIT_TYPE = unitType::XIOU_OBJ;
    const Essentials::rgba DEFAULT_XIOUDOWN_GRID_RGBA = {0xff, 0xff, 0xff, 0xff};
    const Essentials::rgb DEFAULT_XIOUDOWN_GRID_RGB = {0xff, 0xff, 0xff};
}

#define XIOUDOWN_PREDEFS
