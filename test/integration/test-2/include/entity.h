#pragma once
#include "xioudown.h"
#include <functional>

using namespace Xioudown;

class Entity : public XioudownGridUnit {
    public:
        Entity();
        Entity(Essentials::coordinates c);
        Entity(short _width, short _height, Essentials::coordinates c);
        ~Entity();
        
        void processExternalEvents(std::function<void(Entity *e)> *_callback);  // wrapper that accepts an external callback for an Entity.

    private:

};
