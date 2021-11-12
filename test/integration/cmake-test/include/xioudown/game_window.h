#pragma once
#include <SDL.h>
#include <SDL_video.h>
#include "polygon.h"


namespace Xioudown{ namespace Catalyst{
    

    class GameWindow{
        private:
            SDL_Window *g_window; //Our window that will contain our renderer, thus the frame of our little world.
            SDL_Renderer *g_renderer; //The renderer that we will use to render things to the window.
            SDL_Surface *g_surface;

            Uint32 start_time, time_count;
            int frame_Count;

        public:
            GameWindow();
            GameWindow(char *title, int SCREEN_WIDTH, int SCREEN_HEIGHT);
            ~GameWindow();

            void renderFPSText(); //renders fps on screen
            
        private:
            int g_SCREEN_WIDTH, g_SCREEN_HEIGHT;
            char* window_title;

            void init(int width, int height, char* title){
                g_SCREEN_WIDTH = width;
                g_SCREEN_HEIGHT = height;
                window_title = title;
            }

            bool initSDLWindowDependencies();

        public:
            SDL_Renderer* getRenderer(){ return g_renderer; }

            int getWidth()const{ return g_SCREEN_WIDTH; }
            int getHeight()const{ return g_SCREEN_HEIGHT; }

            bool getStatus() { return (bool)(g_window != nullptr); }
            bool aptToRender(SDL_Rect r);  // checks if the elements or items on a screen are within the bounds of the window
            bool aptToRender(int *p);  // same as above
            bool aptToRender(Math::Polygon *p); // ... same as above
    };
};};