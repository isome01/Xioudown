#include "level_room_sub.h"
#include "../room.h"

#include <iostream>
#include <SDL.h>

#define log(x) std::cout << "Lvl Room Sub: from "<< __func__ << " " << x << std::endl;

namespace Astral { namespace Room{ 

    Lvl_Room_Sub::Lvl_Room_Sub(){

        log("Entering Sub room creation.")
    }

    Lvl_Room_Sub::~Lvl_Room_Sub(){

        log("Sub room deleted.")
    }

}; };