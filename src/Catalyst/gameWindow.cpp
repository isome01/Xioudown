#include "gameWindow.h"
#include "../Math/polygon.h"

#include <SDL.h>
#include <stdio.h>

namespace Xioudown{ namespace Catalyst{

    gameWindow::gameWindow(char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT){

        init(SCREEN_WIDTH, SCREEN_HEIGHT, title);

        //For that 60 fps 
        g_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //The screen surface is then rendered onto the window handle
        g_surface = SDL_GetWindowSurface(g_window);

        //We'll test this later... ~ resizable windows ~ 

        /* //make window resizable
        SDL_SetWindowResizable(g_window, SDL_TRUE ); */

        //Start FPS 
        start_time = SDL_GetTicks();
        frame_Count = 0;
    }

    void gameWindow::runFPS(){

        //Really should turn this into a timer from timer.h 12/22/2018

        time_count = SDL_GetTicks();

            if (time_count - start_time >= 1000){
                printf("Fps: %d", frame_Count);
                frame_Count = 0;
                start_time = SDL_GetTicks();
            }

            frame_Count++;
    }

    bool gameWindow::aptToRender(SDL_Rect r){

        int centerX = g_SCREEN_WIDTH / 2, centerY = g_SCREEN_HEIGHT / 2;
        if(r.x > centerX + (g_SCREEN_WIDTH * 2) || r.x < centerX - (g_SCREEN_WIDTH * 2))
            return false;

        if (r.y > (g_SCREEN_HEIGHT * 2) + centerY || r.y < centerY - (g_SCREEN_HEIGHT * 2))
            return false;

        return true;
    } //this function checks to see if an object is able to render given certain parameters.; //this function checks to see if an object is able to render given certain parameters.
    bool gameWindow::aptToRender(Math::Polygon *p){

        int sz = p->getVertices(), centerX = g_SCREEN_WIDTH / 2, centerY = g_SCREEN_HEIGHT / 2;
        for (int i = 0; i < sz; i++){
            //if at any time we have parameters that are in bounds, we'll render them.
            if (p->X()[i] > centerX + (g_SCREEN_WIDTH * 2) || p->X()[i] < centerX - (g_SCREEN_WIDTH * 2))
                return false;

            if (p->Y()[i] > (g_SCREEN_HEIGHT * 2) + centerY  || p->Y()[i] < centerY - (g_SCREEN_HEIGHT * 2))
                return false;
        } return true;
    }
};};