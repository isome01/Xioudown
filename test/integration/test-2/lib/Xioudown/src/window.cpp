#include "window.h"
#include "xioudown.h"
#include "polygon.h"
#include "math.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string>

#include <SDL_image.h>

using namespace Xioudown::Essentials;
// using namespace Xioudown::Media;

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
        
        // SDL_SetWindowResizable(g_window, SDL_TRUE ); //make window resizable

        unsigned t = SDL_GetTicks();
        srand(t);
        //Start FPS
        start_time = t;
        frame_Count = 0;

        loadTexture("basic", "src/data/media/img/dungeon_floor.png");
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

    void GameWindow::renderGrid(XioudownGrid *_grid) {
        if (_grid->show_grid()) {
            Media::Texture *t = m_media_textures["basic"];
            
            int grid_size = _grid->wire_frame_grid_size();
            for (int i = 0; i < grid_size; i++) {
                if (t != NULL && t->body() != NULL) {
                    // printf("Gets here to the texture\n");
                    SDL_RenderCopy(g_renderer, t->body(), NULL, _grid->wire_frame(i));
                }
                // drawRect(
                //     _grid->wire_frame(i),
                //     {0, 0xff, 0xff, 1}
                // );
            }
        }
        /* 
            Then render the background of the grid, whatever it may be
        */
    }

    void GameWindow::renderGridUnit(XioudownGridUnit* _grid_unit) {
        // attempt texture filling
        Media::Texture *t = getTextureFromSpecs(_grid_unit);
        SDL_Rect *base = _grid_unit->base();
        if (t != NULL && t->body() != NULL) {
            // printf("Gets here to the texture\n");
            SDL_RenderCopy(g_renderer, t->body(), NULL, base);

        } else {
            // render grid unit base
            drawFilledRect(
                base,
                _grid_unit->rgba()
            );
        }
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

        // list of xs and ys that correspond to x1/y1, x2/y2
        
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
};

namespace Xioudown {
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

    coordinates GameWindow::setCameraTarget(XioudownGrid *_grid, XioudownGridUnit *_grid_unit)
    {
        coordinates screen_center = {
            this->g_SCREEN_WIDTH / 2,
            this->g_SCREEN_HEIGHT / 2
        };
        coordinates c = _grid_unit->getCenter();
        coordinates position_diff = {
            (c.x >= screen_center.x ? -1 : 1)*(abs(screen_center.x - c.x)),
            (c.y >= screen_center.y ? -1 : 1)*(abs(screen_center.y - c.y))
        };

        if (_grid->show_grid()) {
            for (int i = 0; i < _grid->wire_frame_grid_size(); i++) {
                _grid->wire_frame(i)->x += position_diff.x;
                _grid->wire_frame(i)->y += position_diff.y;
            }
        }

        int unit_count = _grid->gridUnitsCount();
        for (int i = 0; i < unit_count; i++) {
            // hold from processing same unit translation
            XioudownGridUnit *u = _grid->gridUnit(i);
            u += position_diff;
        }
        return c;// return current camera center
    }
};

namespace Xioudown {
    /* Media methods */
    void GameWindow::loadTexture(std::string _alias, std::string _path)
    {
        Media::Texture *t = new Media::Texture(_path);
        t->load(this->g_renderer);
        m_media_textures[_alias] = t != NULL ? t : NULL;
    }

    Media::Texture* GameWindow::getTextureFromSpecs(XioudownGridUnit *_unit)
    {
        Media::textureConfig config = _unit->textureConfig();
        std::string alias = !(config.alias.empty()) ? config.alias : " ";
        std::map<std::string, Media::Texture*>::const_iterator it = m_media_textures.find(alias);
        Media::Texture *m = nullptr;
        if (it == m_media_textures.end()) {
            // attempt to get new texture if one isn't assigned.
            loadTexture(alias, config.path);
        }
        
        return m_media_textures[alias];
    }
}