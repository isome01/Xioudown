#include "level_room_sub.h"
#include "../room.h"

#include <iostream>
#include <SDL.h>

#ifndef log
#define log(x) std::cout << "Lvl Room Sub: from "<< __func__ << " " << x << std::endl;
#endif

namespace Xioudown { namespace Room{

    Lvl_Room_Sub::Lvl_Room_Sub(){

        log("Entering Sub room creation.")
    }

    Lvl_Room_Sub::~Lvl_Room_Sub(){

        log("Sub room deleted.")
    }

}; };