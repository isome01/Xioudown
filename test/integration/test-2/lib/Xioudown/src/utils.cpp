#include "utils.h"
#include "xioudown.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#define log(x) std::cout << x << std::endl;

namespace Xioudown{ namespace Math{

    using namespace Essentials;
    char *appendToString(char *a, char *b){

        log("Enters " << __func__)
        if (a && !b) return a;
        else if (!a && b) return b;
        else if (!a && !b) return a;
        else{
            int a_sz = (int)strlen(a);
            int b_sz = (int)strlen(b);

            char* c = new char[a_sz + b_sz];
            //start with both a and b
            for (int i = 0; a[i] || b[i]; i++){
                c[i] = a[i] ? a[i] : b[i];

            } log("passes here" << c)
            return c;
        }

        //just for the sake of return
        char *def;
        return def;
    }

    //Centering a target rectangle from another rectangle.
    SDL_Rect *p_centerSDLRect(const SDL_Rect src, const SDL_Rect dst){

        SDL_Rect *r = new SDL_Rect;

        r->x = src.x + ( (src.w - dst.w) / 2 );
        r->y = src.y + ( (src.h - dst.h) / 2 );
        r->w = dst.w;
        r->h = dst.h;

        return r;
    }

    SDL_Rect centerSDLRect(const SDL_Rect src, const SDL_Rect dst){

        SDL_Rect *r = new SDL_Rect;

        r->x = src.x + ( (src.w - dst.w) / 2 );
        r->y = src.y + ( (src.h - dst.h) / 2 );
        r->w = dst.w;
        r->h = dst.h;

        return *r;
    }

    int cl_SQRT(int n){

        double a = sqrt(n);

        if(a - int(a) > 0 )
            return int(a) + 1;

        return int(a);
    }

    coordinates g_rand_coords_from_Rect(const SDL_Rect src){
        srand((unsigned int)time(NULL));
        coordinates coords;

        coords.x = rand()%src.w + src.x;
        coords.y = rand()%src.h + src.y;

        return coords;
    }

    Uint8 deviseMaxMinHex(Uint8 n) {
        if ((int)n > 255) {
            return 0xff;
        } else if ((int)n < 0) {
            return 0x00;
        }
        return n;
    }
}};