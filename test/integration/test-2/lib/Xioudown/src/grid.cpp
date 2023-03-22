#include "grid.h"
#include "utils.h"
#include "math.h"
#include <stdio.h>

namespace Xioudown {
    const int DEFAULT_GRID_UNIT_SIZE = 100;
    /* Xioudown Grid Implementation */
    XioudownGrid::XioudownGrid(short width, short height) {
        this->m_xiou_grid_boundaries.x = 0;
        this->m_xiou_grid_boundaries.y = 0;
        this->m_xiou_grid_boundaries.w = width;
        this->m_xiou_grid_boundaries.h = height;

        m_show_grid = true;

        this->initWireFrame(DEFAULT_GRID_UNIT_SIZE);
    }
    
    XioudownGrid::~XioudownGrid() {
        deconstructWireFrame();
    }

    void XioudownGrid::reframeWireFrame(short scale_w, short scale_h) {
        deconstructWireFrame();
        initWireFrame(scale_w, scale_h);
    }

    void XioudownGrid::reframeWireFrame(int unit_size) {
        deconstructWireFrame();
        initWireFrame(unit_size);
    }

    void XioudownGrid::initWireFrame(int unit_size) {
        int count = 0;
        Essentials::coordinates room_size = {m_xiou_grid_boundaries.w, m_xiou_grid_boundaries.h};
        int max_size = room_size.x * room_size.y;
        this->m_grid_wireframe = new SDL_Rect*[room_size.x * room_size.y];
        for (short y = 0; y < room_size.y && count < max_size; y+=unit_size) {
            for (short x = 0; x < room_size.x && count < max_size; x+=unit_size) {
                if ((x + unit_size) >= room_size.x) {
                    this->m_grid_wireframe[count++] = new SDL_Rect({x, y, abs(room_size.x - (x + unit_size)), unit_size});
                }
                else if ((y + unit_size) >= room_size.y) {
                    this->m_grid_wireframe[count++] = new SDL_Rect({x, y, unit_size, abs(room_size.y - (y + unit_size))});
                }
                else {
                    this->m_grid_wireframe[count++] = new SDL_Rect({x, y, unit_size, unit_size});
                }
            }
        }
        m_wire_frame_grid_size = count;
    }
    
    void XioudownGrid::initWireFrame(short width, short height) {
        short scale_w = Math::cl_SQRT(width);
        short scale_h = Math::cl_SQRT(height);
        int count = 0;
        int max_size = width * height;

        this->m_grid_wireframe = new SDL_Rect*[max_size];
        for (short y = 0; y < height && count < max_size; y+=scale_h) {
            for (short x = 0; x < width && count < max_size; x+=scale_w) {
                this->m_grid_wireframe[count++] = new SDL_Rect({x, y, scale_w, scale_h});
            }
        }
        m_wire_frame_grid_size = count;
    }

    void XioudownGrid::deconstructWireFrame() {
        delete m_grid_wireframe;
    }

    SDL_Rect* XioudownGrid::wire_frame(int n) {
        // short max_size = m_xiou_grid_boundaries.w * m_xiou_grid_boundaries.h;
        if (n < 0) n = 0;
        if (n >= m_wire_frame_grid_size) n = m_wire_frame_grid_size - 1;
        return m_grid_wireframe[n];
    }

    const short XioudownGrid::width() { return m_xiou_grid_boundaries.w; }

    const short XioudownGrid::height() { return m_xiou_grid_boundaries.h; }
    
    void XioudownGrid::show_grid(bool s) { m_show_grid = s; }

    const bool XioudownGrid::show_grid() { return m_show_grid; }

    void XioudownGrid::render() {
        if (m_show_grid) {
            short box_width = Math::cl_SQRT(m_xiou_grid_boundaries.w);
            short box_height = Math::cl_SQRT(m_xiou_grid_boundaries.h);
        }
    }
};


namespace Xioudown {
    XioudownGridUnit::XioudownGridUnit() : XioudownGridUnit(
            {0, 0, 0, 0},
            DEFAULT_XIOUDOWN_GRID_RGBA,
            DEFAULT_XIOUDOWN_GRID_UNIT_TYPE
        ) {
        // Erect default constructor if basic object is defined
        m_last_move = {0, 0};
        m_grid_unid = Math::generateReferenceId<XioudownGridUnit*>(this); // generate id for unit
    }

    XioudownGridUnit::XioudownGridUnit(XioudownGridUnit *_unit) : XioudownGridUnit::XioudownGridUnit(
        {_unit->x(), _unit->y(), _unit->w(), _unit->h()},
        {_unit->r(), _unit->g(), _unit->b(), _unit->a()},
        _unit->getType()
    ) {
        // construct a grid unit from a parameter grid unit.
        hidden = _unit->isHidden();
        m_grid_unid = Math::generateReferenceId<XioudownGridUnit*>(this); // generate id for unit
    }

    XioudownGridUnit::XioudownGridUnit(SDL_Rect rect, Essentials::rgba _rgba, unitType _unit_type) :
    m_unit_type(_unit_type),
    m_unit_rgba(_rgba) {
        /* Grid Unit constructor with default safes */
        
        m_grid_unid = Math::generateReferenceId<XioudownGridUnit*>(this); // generate id for unit

        this->m_grid_unit_base = new SDL_Rect;
        this->x(rect.x);
        this->y(rect.y);
        this->w(rect.w);
        this->h(rect.h);

        hidden = false;
        m_last_move = {this->x(), this->y()};
    }

    XioudownGridUnit::XioudownGridUnit(SDL_Rect rect, Essentials::rgb _rgb, unitType _unit_type) :
    m_unit_type(_unit_type),
    m_unit_rgba({_rgb.r, _rgb.g, _rgb.b, 0xff}) {
        /* Grid Unit constructor with default safes */

        m_grid_unid = Math::generateReferenceId<XioudownGridUnit>(*this); // generate id for unit

        this->m_grid_unit_base = new SDL_Rect;
        this->x(rect.x);
        this->y(rect.y);
        this->w(rect.w);
        this->h(rect.h);

        m_last_move = {this->x(), this->y()};
    }

    bool XioudownGridUnit::inside(int vert_x , int vert_y) { //To find if a certain point is inside
        int vertex_1, vertex_2;
        bool flag = false; //boolean flag
        
        //convert x and y coordinates to short values
        short vx = short(vert_x);
        short vy = short(vert_y);

        short vertices = 4;

        SDL_Rect *base = m_grid_unit_base;
        int poly_y[] = {
            base->y,
            base->y + base->h,
            base->y + base->h,
            base->y
        };
        int poly_x[] = {
            base->x,
            base->x,
            base->x + base->w,
            base->x + base->w
        };

        //our index and jump variable handles the first and last vertices as first case
        for (vertex_1 = 0, vertex_2 = vertices - 1; vertex_1 < vertices ; vertex_2 = vertex_1++) {

            //So, if our point has crossed line segments (including the fact that our point may be on the line)...
            if ( ((poly_y[vertex_1] > vy) != (poly_y[vertex_2] > vy)) &&
                 ( vx < ( poly_x[vertex_2] - poly_x[vertex_1] ) * ( vy - poly_y[vertex_1] ) / ( poly_y[vertex_2] - poly_y[vertex_1] ) + poly_x[vertex_1])
               ){
                   //then we flip the flag's boolean value
                    flag = !( flag ); //Even # of times flipped - should revert the flag to false.
                                          //Odd # of times flipped  - should revert the flag to true.
            }
        }
        return flag;
    }

    void XioudownGridUnit::collision(XioudownGridUnit *_grid_unit = nullptr) {
        /* Default void function for collision events between grid objects. */
        if (this->id() == _grid_unit->id()){
            return;
        }

        if (collisionSet() && collidesWith(_grid_unit)) {
            SDL_Rect u = *(_grid_unit->base());
            int unit_left_right[2] = {u.x, u.x + u.w};
            int unit_up_down[2] = {u.y, u.y + u.h};

            Essentials::direction d = getDirection();
            
            // Reset coordinates and direction based on position
            int collision_dist = 0;
            switch (d)
            {
            case Essentials::direction::LEFT:
                collision_dist = abs(unit_left_right[1] - m_last_move.x) - 1;
                x(m_last_move.x);
                m_last_move.x = x();
                break;
            case Essentials::direction::RIGHT:
                collision_dist = abs(unit_left_right[0] - m_last_move.x) + 1;
                x(m_last_move.x);
                m_last_move.x = x();
                break;
            case Essentials::direction::UP:
                collision_dist = abs(unit_up_down[1] - m_last_move.y) - 1;
                y(m_last_move.y);
                m_last_move.y = y();
                break;
            case Essentials::direction::DOWN:
                collision_dist = abs(unit_up_down[0] - m_last_move.y - 1) - 1;
                y(m_last_move.y);
                m_last_move.y = y();
                break;
            
            default:
                break;
            }
            this->setDirection(d);
        }
    }
    
    bool XioudownGridUnit::collidesWith(XioudownGridUnit *_grid_unit) {
        short vertices = 4;

        SDL_Rect *base = _grid_unit->base();
        int unit_y[4] = {
            base->y,
            base->y + base->h,
            base->y + base->h,
            base->y
        };
        int unit_x[4] = {
            base->x,
            base->x,
            base->x + base->w,
            base->x + base->w
        };
        bool hit = false;
        for (int i = 0; i < vertices; i++) {
            hit = inside(unit_x[i], unit_y[i]);
            if (hit) {
                break;
            }
        } return hit;
    }   

    XioudownGridUnit::~XioudownGridUnit() {
        delete m_grid_unit_base;
    }

    Essentials::coordinates XioudownGridUnit::getTransitionDistance() {
        return {
            m_grid_unit_base->x - m_last_move.x,
            m_grid_unit_base->y - m_last_move.y
        };
    }

    Essentials::direction XioudownGridUnit::generateUnitDirection() {
        Essentials::coordinates diff = getTransitionDistance();

        if (diff.x < 0) {
            m_direction = Essentials::direction::LEFT;
        }
        else if (diff.x > 0) {
            m_direction = Essentials::direction::RIGHT;
        }
        else if (diff.y < 0) {
            m_direction = Essentials::direction::UP;
        }
        else if (diff.y > 0) {
            m_direction = Essentials::direction::DOWN;
        }
        
        return m_direction;
    }

    void XioudownGridUnit::x(int _x) {
        m_last_move.x = m_grid_unit_base->x;
        m_grid_unit_base->x = _x;
        int transition = this->getTransitionDistance().x;
        if (transition < 0) {
            m_direction = Essentials::direction::LEFT;
        }
        else if (transition > 0) {
            m_direction = Essentials::direction::RIGHT;
        }
    }

    void XioudownGridUnit::y(int _y) {
        m_last_move.y = m_grid_unit_base->y;
        m_grid_unit_base->y = _y;

        int transition = this->getTransitionDistance().y;
        if (transition < 0) {
            m_direction = Essentials::direction::UP;
        }
        else if (transition > 0) {
            m_direction = Essentials::direction::DOWN;
        }
    }

    // void XioudownGridUnit::operator=(SDL_Rect rect) {
    //     m_grid_unit_base->x = rect.x;
    //     m_grid_unit_base->y = rect.y;
    //     m_grid_unit_base->w = rect.w;
    //     m_grid_unit_base->h = rect.h;
    // }
    
    // XioudownGridUnit* XioudownGridUnit::operator()(XioudownGridUnit _unit) {
    //     // Return complete deep copy of passed param XioudownGridUnit.
    //     return new XioudownGridUnit(_unit);
    // }

    // void XioudownGridUnit::operator=(XioudownGridUnit _unit) {
    //     SDL_Rect rect = {_unit.x(), _unit.y(), _unit.w(), _unit.h()};
    //     m_unit_type = _unit.getType();
    // }

    XioudownGridUnit* XioudownGridUnit::operator()(SDL_Rect rect) {
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->m_grid_unit_base->x = rect.x;
        unit->m_grid_unit_base->y = rect.y;
        unit->m_grid_unit_base->w = rect.w;
        unit->m_grid_unit_base->h = rect.h;

        return unit;
    }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::coordinates c) {
        // return deep copy of XioudownGridUnit with new Essentials::coordinates.
        XioudownGridUnit* unit = new XioudownGridUnit(this);
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);

        return unit;
    }

    // void XioudownGridUnit::operator=(Essentials::coordinates c) {
    //     this->x(c.x);
    //     this->y(c.y);
    // }

    XioudownGridUnit operator+(XioudownGridUnit &_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with added coordinates.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);

        return *(unit);
    }
    
    void operator+=(XioudownGridUnit &_unit, const Essentials::coordinates c) {
        XioudownGridUnit *unit = &_unit;
        unit->x(unit->x() + c.x);
        unit->y(unit->y() + c.y);
    }

    XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with subtracted coordinates.
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() - c.x);
        unit->y(unit->y() - c.y);

        return *(unit);
    }

    /* General operations between said class and others */

    void operator-=(XioudownGridUnit &_unit, const Essentials::coordinates c) {
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() - c.x);
        unit->y(unit->y() - c.y);
    }
    
    // void XioudownGridUnit::operator=(Essentials::rgba _rgba) {
    //     this->r(_rgba.r);
    //     this->g(_rgba.g);
    //     this->b(_rgba.b);
    //     this->a(_rgba.a);
    // }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with parameter rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->r(_rgba.r);
        unit->g(_rgba.g);
        unit->b(_rgba.b);
        unit->a(_rgba.a);
        
        return unit;
    }

    // void XioudownGridUnit::operator=(Essentials::rgb _rgb) {
    //     this->r(_rgb.r);
    //     this->g(_rgb.g);
    //     this->b(_rgb.b);
    // }

    XioudownGridUnit* XioudownGridUnit::operator()(Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with parameter rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(this);
        unit->r(_rgb.r);
        unit->g(_rgb.g);
        unit->b(_rgb.b);
        
        return unit;
    }

    XioudownGridUnit operator+(XioudownGridUnit& _unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());

        return *(unit);
    }

    void operator+=(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());
    }

    XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r - unit->r());
        unit->g(_rgba.g - unit->g());
        unit->b(_rgba.b - unit->b());
        unit->a(_rgba.a - unit->a());

        return *(unit);
    }

    void operator-=(XioudownGridUnit &_unit, const Essentials::rgba _rgba) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r - unit->r());
        unit->g(_rgba.g - unit->g());
        unit->b(_rgba.b - unit->b());
        unit->a(_rgba.a - unit->a());
    }

    XioudownGridUnit operator+(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with added rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r + unit->r());
        unit->g(_rgb.g + unit->g());
        unit->b(_rgb.b + unit->b());

        return *(unit);
    }

    void operator+=(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r + unit->r());
        unit->g(_rgb.g + unit->g());
        unit->b(_rgb.b + unit->b());
    }

    XioudownGridUnit operator-(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with subtracted rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r - unit->r());
        unit->g(_rgb.g - unit->g());
        unit->b(_rgb.b - unit->b());

        return *(unit);
    }

    void operator-=(XioudownGridUnit &_unit, const Essentials::rgb _rgb) {
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r - unit->r());
        unit->g(_rgb.g - unit->g());
        unit->b(_rgb.b - unit->b());
    }

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~ pointer implementations ~~~~~~~~~~~~~~~~~~~~~~~~~ */

    XioudownGridUnit* operator+(XioudownGridUnit *_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with added coordinates.
        _unit->x(_unit->x() + c.x);
        _unit->y(_unit->y() + c.y);

        return _unit;
    }
    
    void operator+=(XioudownGridUnit *_unit, const Essentials::coordinates c) {
        _unit->x(_unit->x() + c.x);
        _unit->y(_unit->y() + c.y);
    }

    XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::coordinates c) {
        // retrieve new deep copy with subtracted coordinates.
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->x(unit->x() - c.x);
        unit->y(unit->y() - c.y);

        return unit;
    }

    /* General operations between said class and others */

    void operator-=(XioudownGridUnit *_unit, const Essentials::coordinates c) {
        _unit->x(_unit->x() - c.x);
        _unit->y(_unit->y() - c.y);
    }

    XioudownGridUnit* operator+(XioudownGridUnit* _unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());

        return _unit;
    }

    void operator+=(XioudownGridUnit *_unit, const Essentials::rgba _rgba) {
        XioudownGridUnit* unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r + unit->r());
        unit->g(_rgba.g + unit->g());
        unit->b(_rgba.b + unit->b());
        unit->a(_rgba.a + unit->a());
    }

    XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::rgba _rgba) {
        // retrieve a completely new copy of XioudownGridUnit with added rgba.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgba.r - unit->r());
        unit->g(_rgba.g - unit->g());
        unit->b(_rgba.b - unit->b());
        unit->a(_rgba.a - unit->a());

        return unit;
    }

    void operator-=(XioudownGridUnit *_unit, const Essentials::rgba _rgba) {
        _unit->r(_rgba.r - _unit->r());
        _unit->g(_rgba.g - _unit->g());
        _unit->b(_rgba.b - _unit->b());
        _unit->a(_rgba.a - _unit->a());
    }

    XioudownGridUnit* operator+(XioudownGridUnit *_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with added rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(_rgb.r + unit->r());
        unit->g(_rgb.g + unit->g());
        unit->b(_rgb.b + unit->b());

        return unit;
    }

    void operator+=(XioudownGridUnit *_unit, const Essentials::rgb _rgb) {
        _unit->r(_rgb.r - _unit->r());
        _unit->g(_rgb.g - _unit->g());
        _unit->b(_rgb.b - _unit->b());
    }

    XioudownGridUnit* operator-(XioudownGridUnit *_unit, const Essentials::rgb _rgb) {
        // retrieve a completely new copy of XioudownGridUnit with subtracted rgb.
        XioudownGridUnit *unit = new XioudownGridUnit(_unit);
        unit->r(unit->r() - _rgb.r);
        unit->g(unit->g() - _rgb.g);
        unit->b(unit->b() - _rgb.b);

        return unit;
    }

    void operator-=(XioudownGridUnit *_unit, const Essentials::rgb _rgb) {
        _unit->r(_unit->r() - _rgb.r);
        _unit->g(_unit->g() - _rgb.g);
        _unit->b(_unit->b() - _rgb.b);
    }
};
