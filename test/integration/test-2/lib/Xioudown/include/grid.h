#pragma once
#include <SDL.h>
#include "xioudown_predefs.h"
#include "utils.h"

// includes for Xioudown Grid classes

namespace Xioudown {

    class XioudownGrid {
        /* Implementation for the Grid*/

        private:
            SDL_Rect **m_grid_wireframe;
            SDL_Rect m_xiou_grid_boundaries;
            int m_wire_frame_grid_size;
            void deconstructWireFrame();
            void initWireFrame(int unit_size); // init wireframe by unit size
            void initWireFrame(short width, short height);

        public:
            XioudownGrid(short width, short height);
            ~XioudownGrid();

        public:
            const short width();
            const short height();
            int wire_frame_grid_size() const {return m_wire_frame_grid_size; }
            void reframeWireFrame(short scale_w, short scale_h);
            void reframeWireFrame(int unit_size); // init wireframe by unit size
            const bool show_grid();
            void show_grid(bool s);
            void render();

            SDL_Rect* wire_frame(int n);

        private:
            bool m_show_grid;
            std::vector<XioudownGridUnit*> m_grid_units;

        public:
            XioudownGridUnit* gridUnit(int n) {
                return m_grid_units[n];
            }
            void addUnit(XioudownGridUnit *_grid_unit) {
                m_grid_units.push_back(_grid_unit);
            }
            int gridUnitsCount() const{ return (int)m_grid_units.size(); }
    };

    class XioudownGridUnit {
        private:
            uint64_t m_grid_unid;  // the unique address of a unit
            SDL_Rect *m_grid_unit_base;
            unitType m_unit_type;
            Essentials::rgba m_unit_rgba;
            bool hidden = false;
            bool m_collision = false;
            Essentials::direction m_direction = Essentials::direction::DOWN;
            Essentials::coordinates m_last_move = {0, 0};
            Media::textureConfig m_unit_texture_config;

        private:
            bool inside(int vert_x , int vert_y);
            bool inside(short vert_x , short vert_y) { return inside(int(vert_x), int(vert_y)); }

        public:
            XioudownGridUnit();
            XioudownGridUnit(XioudownGridUnit *_unit);
            XioudownGridUnit(
                SDL_Rect rect,
                Essentials::rgba _rgba,
                unitType _unit_type
            );
            XioudownGridUnit(
                SDL_Rect rect,
                Essentials::rgb _rgb,
                unitType _unit_type
            );
            ~XioudownGridUnit();
            
            uint64_t id() const { return m_grid_unid; }
            unitType getType() const { return m_unit_type; };
            SDL_Rect* base() { return m_grid_unit_base; }
            Essentials::direction getDirection() const { return m_direction; }
            void setDirection(Essentials::direction d) { m_direction = d; }
            Essentials::direction generateUnitDirection();
            Essentials::coordinates getTransitionDistance();
            void hide() { this->a(0x00); }  // hides this unit
            void show() { this->a(0xff); }
            bool isHidden() const { return hidden; }
            Essentials::coordinates getCenter() const {
                return {
                    m_grid_unit_base->x + (m_grid_unit_base->w / 2),
                    m_grid_unit_base->y + (m_grid_unit_base->h / 2)
                };
            }
            virtual void collision(XioudownGridUnit *_grid_unit); // default is object-blocking effects
            bool collidesWith(XioudownGridUnit *_grid_unit);
            bool collisionSet() const{ return m_collision; }
            void collisionSet(bool f) { m_collision = f; }

            Media::textureConfig textureConfig() const { return m_unit_texture_config; }
            void textureConfig(Media::textureConfig _config) { m_unit_texture_config = _config; }

            int x() const { return m_grid_unit_base->x; }
            void x(int _x);
            int y() const { return m_grid_unit_base->y; }
            void y(int _y);
            int w() const { return m_grid_unit_base->w; }
            void w(int _w) { m_grid_unit_base->w = _w; }
            int h() const { return m_grid_unit_base->h; }
            void h(int _h) { m_grid_unit_base->h = _h; }

            Essentials::coordinates coordinates () const{
                return Essentials::coordinates({m_grid_unit_base->x, m_grid_unit_base->y});
            }
            // rgba specs
            Essentials::rgba rgba(){ return m_unit_rgba; }
            void rgba(Essentials::rgba _rgba){ m_unit_rgba = _rgba; }
            Essentials::rgb rgb(){ return {m_unit_rgba.r, m_unit_rgba.g, m_unit_rgba.b}; }
            void rgb(Essentials::rgb _rgb){ m_unit_rgba.r = _rgb.r; m_unit_rgba.g = _rgb.g; m_unit_rgba.b = _rgb.b; }
            Uint8 r(){ return m_unit_rgba.r; }
            void r(Uint8 _r){ m_unit_rgba.r = Math::deviseMaxMinHex(_r); }
            Uint8 g(){ return m_unit_rgba.g; }
            void g(Uint8 _g){ m_unit_rgba.g = Math::deviseMaxMinHex(_g); }
            Uint8 b(){ return m_unit_rgba.b; }
            void b(Uint8 _b){ m_unit_rgba.b = Math::deviseMaxMinHex(_b); }
            Uint8 a(){ return m_unit_rgba.a; }
            void a(Uint8 _a){ m_unit_rgba.a = Math::deviseMaxMinHex(_a); }
            

            // deep copy methods
            XioudownGridUnit* operator()(SDL_Rect rect);
            // void operator=(SDL_Rect rect);
            // XioudownGridUnit* operator()(XioudownGridUnit _unit);
            // void operator=(XioudownGridUnit _unit);
            XioudownGridUnit* operator()(Essentials::coordinates c);
            // void operator=(Essentials::coordinates c);
            XioudownGridUnit* operator()(Essentials::rgba _rgba);
            // void operator=(Essentials::rgba _rgba);
            XioudownGridUnit* operator()(Essentials::rgb _rgb);
            // void operator=(Essentials::rgb _rgb);

            // inherited pointers operrands
            friend XioudownGridUnit* operator+(XioudownGridUnit *_unit, const Essentials::coordinates c);
            friend XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::coordinates c);
            friend void operator+=(XioudownGridUnit *_unit, const Essentials::coordinates c);
            friend void operator-=(XioudownGridUnit *_unit, const Essentials::coordinates c);
            friend XioudownGridUnit* operator+(XioudownGridUnit *_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::rgba _rgba);
            friend void operator+=(XioudownGridUnit *_unit, const Essentials::rgba _rgba);
            friend void operator-=(XioudownGridUnit *_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit* operator+(XioudownGridUnit *_unit, const Essentials::rgb _rgb);
            friend XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::rgb _rgb);
            friend void operator+=(XioudownGridUnit *_unit, const Essentials::rgb _rgb);
            friend void operator-=(XioudownGridUnit *_unit, const Essentials::rgb _rgb);

            // inherited flat class operrands
            friend XioudownGridUnit operator+(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::coordinates c);
            friend XioudownGridUnit operator+(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::rgba _rgba);
            friend XioudownGridUnit operator+(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend void operator+=(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
            friend void operator-=(XioudownGridUnit &_unit, const Essentials::rgb _rgb);
    };
};

#define XIOUDOWN_GRID
