#pragma once

#include <SDL.h>
#include "xioudown.h"

namespace Xioudown { namespace Math{

    //Helpful methods
    char *appendToString(char *a, char *b);
    SDL_Rect *p_centerSDLRect(const SDL_Rect src, const SDL_Rect dst);
    SDL_Rect centerSDLRect(const SDL_Rect src, const SDL_Rect dst);
    Essentials::coordinates g_rand_coords_from_Rect(const SDL_Rect src);
    int cl_SQRT(int n);

};};