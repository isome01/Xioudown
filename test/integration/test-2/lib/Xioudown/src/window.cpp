#include "window.h"
#include "xioudown.h"
#include "polygon.h"
#include <stdio.h>


namespace Xioudown{

    GameWindow::GameWindow(char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT){

        init(SCREEN_WIDTH, SCREEN_HEIGHT, title);

        if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
            SDL_GetError();
        } if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
            SDL_GetError();
        }

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

    GameWindow::~GameWindow() {

        SDL_DestroyRenderer(g_renderer);
        g_renderer = NULL;
        SDL_Quit();
    }

    void GameWindow::clearScreenToRenderedColor () {
        SDL_RenderClear(g_renderer);
    }

    void GameWindow::setScreenBackgroundColor (Essentials::rgba rgba = DEFAULT_BACKGROUND_COLOR_RGBA) {
        //Rendering the game background
        SDL_SetRenderDrawColor(
            g_renderer, rgba.r, rgba.g, rgba.b, rgba.a
        );
    }

    void GameWindow::presentRenderedItems () {
        //Then we show everything we attempted to render
        SDL_RenderPresent(g_renderer);
    }

    void GameWindow::renderGridUnit(XioudownGridUnit* _grid_unit) {

        // render grid unit base
        drawFilledRect(
            _grid_unit->base(),
            _grid_unit->rgba()
        );
    }

    void GameWindow::drawRect(SDL_Rect* rect, Essentials::rgb rgb = DEFAULT_BACKGROUND_COLOR_RGB) {
        
        // Shortcut for drawing a colored framed rectangle

        GameWindow::drawRect(rect, {rgb.r, rgb.g, rgb.b, 0xff});
    }

    void GameWindow::drawRect(SDL_Rect* rect, Essentials::rgba rgba = DEFAULT_BACKGROUND_COLOR_RGBA) {
        
        // Shortcut for drawing a colored framed rectangle

        GameWindow::setScreenBackgroundColor(rgba);
        SDL_RenderDrawRect(
            g_renderer,
            rect
        );
    }

    void GameWindow::drawFilledRect(SDL_Rect* rect, Essentials::rgb rgb) {
        GameWindow::drawFilledRect(
            rect, {rgb.r, rgb.g, rgb.b, 0xff});
    }

    void GameWindow::drawFilledRect(SDL_Rect* rect, Essentials::rgba rgba) {
        // Shortcut for drawing a colored framed rectangle

        GameWindow::setScreenBackgroundColor(rgba);
        SDL_RenderFillRect(
            g_renderer,
            rect
        );
    }

    void GameWindow::drawFilledPolygon(Math::Polygon *polygon, Essentials::rgb rgb = DEFAULT_BACKGROUND_COLOR_RGB) {
        drawFilledPolygon(polygon, { rgb.r, rgb.g, rgb.b, 0xff});
    }

    void GameWindow::drawFilledPolygon(Math::Polygon *polygon, Essentials::rgba rgba = DEFAULT_BACKGROUND_COLOR_RGBA) {

        short x[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};
        short y[12] = {10, 20, 20, 30, 30, 40, 40, 30, 30, 20, 20, 10};
        
        // shortcut for rendering polygon points
        // filledPolygonRGBA(
        //     g_renderer,
        //     x,
        //     y,
        //     polygon->getVertices(),
        //     rgba.r,
        //     rgba.g,
        //     rgba.b,
        //     rgba.a
        // );
    }

    void GameWindow::runFPS() {

        //Really should turn this into a timer from timer.h 12/22/2018

        time_count = SDL_GetTicks();

            if (time_count - start_time >= 1000){
                printf("Fps: %d", frame_Count);
                frame_Count = 0;
                start_time = SDL_GetTicks();
            }

            frame_Count++;
    }

    bool GameWindow::aptToRender(SDL_Rect r){

        int centerX = g_SCREEN_WIDTH / 2, centerY = g_SCREEN_HEIGHT / 2;
        if(r.x > centerX + (g_SCREEN_WIDTH * 2) || r.x < centerX - (g_SCREEN_WIDTH * 2))
            return false;

        if (r.y > (g_SCREEN_HEIGHT * 2) + centerY || r.y < centerY - (g_SCREEN_HEIGHT * 2))
            return false;

        return true;
    } //this function checks to see if an object is able to render given certain parameters.; //this function checks to see if an object is able to render given certain parameters.
    bool GameWindow::aptToRender(Math::Polygon *p){

        int sz = p->getVertices(), centerX = g_SCREEN_WIDTH / 2, centerY = g_SCREEN_HEIGHT / 2;
        for (int i = 0; i < sz; i++){
            //if at any time we have parameters that are in bounds, we'll render them.
            if (p->x()[i] > centerX + (g_SCREEN_WIDTH * 2) || p->x()[i] < centerX - (g_SCREEN_WIDTH * 2))
                return false;

            if (p->y()[i] > (g_SCREEN_HEIGHT * 2) + centerY  || p->y()[i] < centerY - (g_SCREEN_HEIGHT * 2))
                return false;
        } return true;
    }
};
