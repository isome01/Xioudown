#pragma once
#include <SDL.h>
#include <SDL_video.h>
#include "../Math/polygon.h"

namespace Astral{ namespace Catalyst{
    
    class gameWindow{
        private:
            SDL_Window *g_window; //Our window that will contain our renderer, thus the frame of our little world.
            SDL_Renderer *g_renderer; //The renderer that we will use to render things to the window.
            SDL_Surface *g_surface;

            Uint32 start_time, time_count;
            int frame_Count;

        public:
            gameWindow(char* title = (char*)("Astral Engine Window"), int SCREEN_WIDTH = 960, int SCREEN_HEIGHT = 640);
            ~gameWindow(){
                SDL_DestroyRenderer(g_renderer);
                g_renderer = NULL;
            }

            void runFPS(); //renders fps on screen
            
        private:
            int g_SCREEN_WIDTH, g_SCREEN_HEIGHT;
            char* window_title;

            void init(int width, int height, char* title){
                g_SCREEN_WIDTH = width;
                g_SCREEN_HEIGHT = height;
                window_title = title;
            }
        public:
            SDL_Renderer* getRenderer(){ return g_renderer; }

            int getWidth()const{ return g_SCREEN_WIDTH; }
            int getHeight()const{ return g_SCREEN_HEIGHT; }

            bool getStatus() { return (bool)(g_window != NULL); }
            bool aptToRender(SDL_Rect r);
            bool aptToRender(Math::Polygon *p);
    };
};};