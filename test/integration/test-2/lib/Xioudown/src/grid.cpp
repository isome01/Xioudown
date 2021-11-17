#include "grid.h"

namespace Xioudown {
    /* Xioudown Grid Implementation */
};


namespace Xioudown {

    XioudownGridUnit::XioudownGridUnit(){
        // Erect default constructor if basic object is defined
        XioudownGridUnit(
            {0, 0, 0, 0},
            DEFAULT_XIOUDOWN_GRID_RGBA,
            DEFAULT_XIOUDOWN_GRID_UNIT_TYPE
        );
    }

    XioudownGridUnit::XioudownGridUnit(SDL_Rect rect, Essentials::rgba _rgba, unitType _unit_type) :
    m_unit_type(_unit_type),
    m_unit_rgba(_rgba) {
        /* Grid Unit constructor with default safes */

        this->m_grid_unit_base = new SDL_Rect;
        this->x(rect.x);
        this->y(rect.y);
        this->w(rect.w);
        this->h(rect.h);
    }

    XioudownGridUnit::XioudownGridUnit(XioudownGridUnit *_unit) {
        // All rect props
        XioudownGridUnit::XioudownGridUnit(
            {_unit->x(), _unit->y(), _unit->w(), _unit->h()},
            {_unit->r(), _unit->g(), _unit->b(), _unit->a()},
            _unit->getType()
        );
    }

    XioudownGridUnit::~XioudownGridUnit() {
        delete m_grid_unit_base;
    }

    XioudownGridUnit* XioudownGridUnit::operator()(SDL_Rect rect) {
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->m_grid_unit_base->x = rect.x;
        unit->m_grid_unit_base->y = rect.y;
        unit->m_grid_unit_base->w = rect.w;
        unit->m_grid_unit_base->h = rect.h;

        return unit;
    }

    void XioudownGridUnit::operator=(SDL_Rect rect) {
        m_grid_unit_base->x = rect.x;
        m_grid_unit_base->y = rect.y;
        m_grid_unit_base->w = rect.w;
        m_grid_unit_base->h = rect.h;
    }
    
    XioudownGridUnit* XioudownGridUnit::operator()(XioudownGridUnit _unit) {
        // Return complete deep copy of passed param XioudownGridUnit.
        return new XioudownGridUnit(_unit);
    }

    void XioudownGridUnit::operator=(XioudownGridUnit _unit) {
        SDL_Rect rect = {_unit.x(), _unit.y(), _unit.w(), _unit.h()};
        m_unit_type = _unit.getType();
    }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::coordinates c) {
        // return deep copy of XioudownGridUnit with new Essentials::coordinates.
        XioudownGridUnit* unit = new XioudownGridUnit(this);
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);

        return unit;
    }

    void XioudownGridUnit::operator=(Essentials::coordinates c) {
        this->x(c.x);
        this->y(c.y);
    }

    XioudownGridUnit* operator+(const XioudownGridUnit &_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with added coordinates.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);

        return unit;
    }
    
    void operator+=(XioudownGridUnit &_unit, const Essentials::coordinates c) {
        XioudownGridUnit *unit = &_unit;
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);
    }

    XioudownGridUnit* operator-(const XioudownGridUnit &_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with subtracted coordinates.
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() - c.x);
        unit->y(unit->y() - c.y);

        return unit;
    }

    /* General operations between said class and others */

    void operator-=(const XioudownGridUnit &_unit, const Essentials::coordinates c) {
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() - c.x);
        unit->y(unit->y() - c.y);
    }
    
    void XioudownGridUnit::operator=(Essentials::rgba _rgba) {
        this->r(_rgba.r);
        this->g(_rgba.g);
        this->b(_rgba.b);
        this->a(_rgba.a);
    }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with parameter rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->r(_rgba.r);
        unit->g(_rgba.g);
        unit->b(_rgba.b);
        unit->a(_rgba.a);
        
        return unit;
    }

    void XioudownGridUnit::operator=(Essentials::rgb _rgb) {
        this->r(_rgb.r);
        this->g(_rgb.g);
        this->b(_rgb.b);
    }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with parameter rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->r(_rgb.r);
        unit->g(_rgb.g);
        unit->b(_rgb.b);
        
        return unit;
    }

    XioudownGridUnit* operator+(XioudownGridUnit& _unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());

        return unit;
    }

    void operator+=(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());
    }

    XioudownGridUnit* operator-(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r - unit->r());
        unit->g(_rgba.g - unit->g());
        unit->b(_rgba.b - unit->b());
        unit->a(_rgba.a - unit->a());

        return unit;
    }

    void operator-=(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r - unit->r());
        unit->g(_rgba.g - unit->g());
        unit->b(_rgba.b - unit->b());
        unit->a(_rgba.a - unit->a());
    }

    XioudownGridUnit* operator+(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with added rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r + unit->r());
        unit->g(_rgb.g + unit->g());
        unit->b(_rgb.b + unit->b());

        return unit;
    }

    void operator+=(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r + unit->r());
        unit->g(_rgb.g + unit->g());
        unit->b(_rgb.b + unit->b());
    }

    XioudownGridUnit* operator-(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with subtracted rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r - unit->r());
        unit->g(_rgb.g - unit->g());
        unit->b(_rgb.b - unit->b());

        return unit;
    }

    void operator-=(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r - unit->r());
        unit->g(_rgb.g - unit->g());
        unit->b(_rgb.b - unit->b());
    }
};
