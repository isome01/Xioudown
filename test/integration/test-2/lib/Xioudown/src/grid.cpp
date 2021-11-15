#include "xioudown.h"


namespace Xioudown {
    /* Xioudown Grid Implementation */
};


namespace Xioudown {
    XioudownGridUnit::XioudownGridUnit(SDL_Rect rect, unitType _unit_type) : m_unit_type(_unit_type) {
        /* Grid Unit constructor with default safes */
    }

    XioudownGridUnit* XioudownGridUnit::operator()(SDL_Rect rect) {
        m_grid_unit_base->x = rect.x;
        m_grid_unit_base->y = rect.y;
        m_grid_unit_base->w = rect.w;
        m_grid_unit_base->h = rect.h;

        return this;
    }

    void XioudownGridUnit::operator=(SDL_Rect rect) {
        m_grid_unit_base->x = rect.x;
        m_grid_unit_base->y = rect.y;
        m_grid_unit_base->w = rect.w;
        m_grid_unit_base->h = rect.h;
    }
    
    XioudownGridUnit* XioudownGridUnit::operator()(XioudownGridUnit* _unit) {
        SDL_Rect rect = {_unit->x(), _unit->y(), _unit->w(), _unit->h()};
        m_unit_type = _unit->getType();

        return this;
    }

    void XioudownGridUnit::operator=(XioudownGridUnit* _unit) {
        SDL_Rect rect = {_unit->x(), _unit->y(), _unit->w(), _unit->h()};
        m_unit_type = _unit->getType();
    }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::coordinates c) {
        this->x(c.x);
        this->y(c.y);

        return this;
    }

    void XioudownGridUnit::operator=(Essentials::coordinates c) {
        this->x(c.x);
        this->y(c.y);
    }

    XioudownGridUnit* XioudownGridUnit::operator+(Essentials::coordinates c) {
        this->x(this->x() + c.x);
        this->y(this->y() + c.y);

        return this;
    }
    
    void XioudownGridUnit::operator+=(Essentials::coordinates c) {
        this->x(this->x() + c.x);
        this->y(this->y() + c.y);
    }

    XioudownGridUnit* XioudownGridUnit::operator-(Essentials::coordinates c) {
        this->x(this->x() - c.x);
        this->y(this->y() - c.y);

        return this;
    }
    
    void XioudownGridUnit::operator-=(Essentials::coordinates c) {
        this->x(this->x() + c.x);
        this->y(this->y() + c.y);
    }
};
