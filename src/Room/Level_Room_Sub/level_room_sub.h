#pragma once

#include <SDL.h>
#include "../room.h"

namespace Astral { namespace Room{
    //Subroom of a "Lvl_Room"
    class Lvl_Room_Sub : public Room{

        public:
            Lvl_Room_Sub();
            ~Lvl_Room_Sub();
    };
};};