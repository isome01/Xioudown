#pragma once
#include "xioudown_predefs.h"
#include "polygon.h"

#include <SDL.h>
#include <SDL_video.h>
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"
#include "SDL2_gfxPrimitives_font.h"


const Xioudown::Essentials::rgba DEFAULT_BACKGROUND_COLOR_RGBA = { 0xff, 0xff, 0xff, 0xff };
const Xioudown::Essentials::rgb DEFAULT_BACKGROUND_COLOR_RGB = { 0xff, 0xff, 0xff };

namespace Xioudown {
    
    class GameWindow : public XioudownClass {
        private:
            SDL_Window *g_window; //Our window that will contain our renderer, thus the frame of our little world.
            SDL_Renderer *g_renderer; //The renderer that we will use to render things to the window.
            SDL_Surface *g_surface;

            Uint32 start_time, time_count;
            int frame_Count;

            // XioudownGrid *game_window_grid;

        public:
            GameWindow(char* title = (char*)("Xioudown Engine Window"), int SCREEN_WIDTH = 960, int SCREEN_HEIGHT = 640);
            ~GameWindow();

            void presentRenderedItems();
            void clearScreenToRenderedColor();
            void setScreenBackgroundColor(Essentials::rgba rgba);
            void renderGridUnit(XioudownGridUnit* _grid_unit);
            void runFPS(); //renders fps on screen

            void drawRect(SDL_Rect* rect, Essentials::rgb rgb);  // uses renderer to draw a rect
            void drawRect(SDL_Rect* rect, Essentials::rgba rgba);
            void drawFilledRect(SDL_Rect* rect, Essentials::rgb rgb);  // uses renderer to draw a rect
            void drawFilledRect(SDL_Rect* rect, Essentials::rgba rgba);
            void drawFilledPolygon(Math::Polygon *polygon, Essentials::rgb rgb);  //uses renderer to draw a polygon
            void drawFilledPolygon(Math::Polygon *polygon, Essentials::rgba rgba);
            void destroy() { this->~GameWindow(); };  // explicit destroy function

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
            bool aptToRender(int *p);
            bool aptToRender(Math::Polygon *p);
    };
};

#define XIOUDOWN_GAME_WINDOW
