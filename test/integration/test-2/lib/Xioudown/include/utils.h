#pragma once

#include <SDL.h>
#include "xioudown_predefs.h"

namespace Xioudown { namespace Math{

    //Helpful methods
    char *appendToString(char *a, char *b);
    SDL_Rect *p_centerSDLRect(const SDL_Rect src, const SDL_Rect dst);
    SDL_Rect centerSDLRect(const SDL_Rect src, const SDL_Rect dst);
    Essentials::coordinates g_rand_coords_from_Rect(const SDL_Rect src);
    int cl_SQRT(int n);

    template <class T>
    uint64_t generateReferenceId(T ref) { return reinterpret_cast<uint64_t>(&ref); }; // generates a unique id for any item

    Uint8 deviseMaxMinHex(Uint8 n);  // returns values inclusive such that 0 <= n <= 255.
};};

#define XIOUDOWN_UTILS
