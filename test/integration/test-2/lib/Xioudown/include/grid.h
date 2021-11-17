#pragma once
#include "xioudown_predefs.h"

// includes for Xioudown Grid classes

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
            Essentials::rgba m_unit_rgba;
            Essentials::direction m_direction;

        public:
            XioudownGridUnit();
            XioudownGridUnit(XioudownGridUnit *_unit);
            XioudownGridUnit(
                SDL_Rect rect = {0, 0, 0, 0},
                Essentials::rgba _rgba = DEFAULT_XIOUDOWN_GRID_RGBA,
                unitType _unit_type = DEFAULT_XIOUDOWN_GRID_UNIT_TYPE
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

            // rgba specs
            Essentials::rgba rgba(){ return m_unit_rgba; }
            void rgba(Essentials::rgba _rgba){ m_unit_rgba = _rgba; }
            Essentials::rgb rgb(){ return {m_unit_rgba.r, m_unit_rgba.g, m_unit_rgba.b}; }
            void rgb(Essentials::rgb _rgb){ m_unit_rgba.r = _rgb.r; m_unit_rgba.g = _rgb.g; m_unit_rgba.b = _rgb.b; }
            Uint8 r(){ return m_unit_rgba.r; }
            void r(Uint8 _r){ m_unit_rgba.r = _r; }
            Uint8 g(){ return m_unit_rgba.g; }
            void g(Uint8 _g){ m_unit_rgba.g = _g; }
            Uint8 b(){ return m_unit_rgba.b; }
            void b(Uint8 _b){ m_unit_rgba.b = _b; }
            Uint8 a(){ return m_unit_rgba.a; }
            void a(Uint8 _a){ m_unit_rgba.a = _a; }
            

            // deep copy methods
            XioudownGridUnit* operator()(SDL_Rect rect);
            void operator=(SDL_Rect rect);
            XioudownGridUnit* operator()(XioudownGridUnit _unit);
            void operator=(XioudownGridUnit _unit);
            XioudownGridUnit* operator()(Essentials::coordinates c);
            void operator=(Essentials::coordinates c);
            XioudownGridUnit* operator()(Essentials::rgba _rgba);
            void operator=(Essentials::rgba _rgba);
            XioudownGridUnit* operator()(Essentials::rgb _rgb);
            void operator=(Essentials::rgb _rgb);

            friend XioudownGridUnit* operator+(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend XioudownGridUnit* operator-(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend XioudownGridUnit* operator+(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit* operator-(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit* operator+(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend XioudownGridUnit* operator-(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
    };
};

#define XIOUDOWN_GRID
