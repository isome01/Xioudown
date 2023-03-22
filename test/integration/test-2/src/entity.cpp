#include "xioudown.h"
#include "entity.h"


Entity::Entity () : XioudownGridUnit::XioudownGridUnit() {
    w(75);
    h(75);
}

Entity::Entity (Essentials::coordinates c) : XioudownGridUnit::XioudownGridUnit() {
    w(75);
    h(75);
    x(c.x);
    y(c.y);
}

Entity::Entity (short _width, short _height, Essentials::coordinates c) : XioudownGridUnit::XioudownGridUnit(){
    w(_width);
    h(_height);
    x(c.x);
    y(c.y);
}

Entity::~Entity () {
    XioudownGridUnit::~XioudownGridUnit();
}

void Entity::processExternalEvents(std::function<void(Entity *e)> *_callback) {
    // Do other entity things before processing callback:
    //  - stat buffs
    //  - special abilities
    //  - damage mitigation

    (*_callback)(this);
}
