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

    const unitType DEFAULT_XIOUDOWN_GRID_UNIT_TYPE = unitType::XIOU_OBJ;

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
};

namespace Xioudown {

    class XioudownGrid {
        /* Implementation for the Grid*/
        public:
            XioudownGrid(short width, short height);
            ~XioudownGrid();

        private:
            SDL_Rect *m_grid_wireframe, m_xiou_grid_boundaries;

        public:
            const short width();
            const short height();
            const bool show_grid();
            void show_grid(bool s);

        private:
            bool m_show_grid;
            std::vector<XioudownGridUnit*> m_grid_units;
    };

    class XioudownGridUnit {
        private:
            SDL_Rect *m_grid_unit_base;
            unitType m_unit_type;

        public:
            XioudownGridUnit(
                SDL_Rect rect = {0, 0, 0, 0},
                unitType _unit_type = unitType::XIOU_OBJ
            );
            ~XioudownGridUnit();
            unitType getType() const { return m_unit_type; };
            SDL_Rect* base() const { return m_grid_unit_base; }
            int x() const { return m_grid_unit_base->x; }
            void x(int _x) { m_grid_unit_base->x = _x; }
            int y() const { return m_grid_unit_base->y; }
            void y(int _y) { m_grid_unit_base->y = _y; }
            int w() const { return m_grid_unit_base->w; }
            void w(int _w) { m_grid_unit_base->w = _w; }
            int h() const { return m_grid_unit_base->h; }
            void h(int _h) { m_grid_unit_base->h = _h; }

            // deep copy methods
            XioudownGridUnit* operator()(SDL_Rect rect);
            void operator=(SDL_Rect rect);
            XioudownGridUnit* operator()(XioudownGridUnit* _unit);
            void operator=(XioudownGridUnit* _unit);
            XioudownGridUnit* operator()(Essentials::coordinates c);
            void operator=(Essentials::coordinates c);

            XioudownGridUnit* operator+(Essentials::coordinates c);
            XioudownGridUnit* operator-(Essentials::coordinates c);
            void operator+=(Essentials::coordinates c);
            void operator-=(Essentials::coordinates c);
    };
};
