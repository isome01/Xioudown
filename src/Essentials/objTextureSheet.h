#pragma once
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Texture Pack Class: For sprite and tile rendering (etc..) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// A composite way of rendering tiles and textures (e.g.: render inventory boxes or floor texture)

#include <SDL.h>
#include <stdio.h>
#include "timer.h"
#include "paradigm.h"

namespace Astral { namespace Essentials{ 
    class gwTexturePack{
    private:
        SDL_Texture *texture;
        SDL_Rect *frames;
        SDL_Rect *texture_frame;
        int frame_count, m_rows, m_columns;
        bool isAnimated;
        Timer *frame_timer;

        int max_iters;
        int max_sprite_x, max_sprite_y;

        gwTexture_Sheet m_sprite_sheet;

        //Interface for this tool
        //gwTexturePack(
        //              SDL_Rect: the source rect that will be rendered,
        //              gwTexture_Sheet: A struct containing info about the sprite sheet to be rendered,
        //              int rows: how ever many rows that need to be rendered at a time,
        //              int columns: how ever many columns that need to be rendered at a time,
        //
        //              bool animated: if the sprite sheet represents an animated sprite; will be set to false by default,
        //              int iters: however many phases/iterations the sprite sheet would enter,
        //              int a_rows: however many rows the animated sprite file (either jpg or png) has to be rendered, and
        //              int a_columns: however many columns the animated sprite file must be rendered. frames could total ( 2 rows 5 columns )
        //      )


    public:

        //for rendering sprite sheets
        gwTexturePack(SDL_Rect *src);

        //for rendering tiling and other static images
        gwTexturePack(SDL_Rect *src, gwTexture_Sheet *src2, int rows = 1, int columns = 1, bool animated = false, int iters = 1, int a_rows = 1, int a_columns = 1){

            //Checkbox for sprite sheet
            isAnimated = animated;

            //Put everything into the texture sheet
            if (isAnimated){
                m_sprite_sheet.columns = a_columns;
                m_sprite_sheet.rows = a_rows;
                m_sprite_sheet.iterations = iters;
                printf("is animated. nothing here yet.\n");
            } else {
                // if not animated then we will circulate only one frame.
                m_sprite_sheet.columns = 1;
                m_sprite_sheet.rows = 1;
                m_sprite_sheet.iterations = 1;

                //make sure frames match as a constraint for non-animated sprites
                m_sprite_sheet.sprite_x = m_sprite_sheet.frame.w;
                m_sprite_sheet.sprite_y = m_sprite_sheet.frame.h;
                printf("isn't animated.\n");
            }

            frame_count = rows * columns;
            frames = new SDL_Rect[frame_count];

            //Here we start to make the rows and columns distributed proportionately. (i.e.: n rows * m columns)
            for (int r = 0, index = 0; r < rows; r++){
                for (int c = 0; c < columns; c++, index++){
                    frames[index].w = src->w / rows;
                    frames[index].h = src->h / columns;
                    frames[index].x = src->x + (frames[index].w * c);
                    frames[index].y = src->y + (frames[index].h * r);
                }
            }
            texture_frame = new SDL_Rect;
            texture_frame->x = src2->frame.x;
            texture_frame->y = src2->frame.y;
            texture_frame->w = src2->frame.w;
            texture_frame->h = src2->frame.h;

            texture = src2->texture;
            m_rows = rows;
            m_columns = columns;

        }
        ~gwTexturePack(){
            if (frame_count > 1)
                delete [] frames;
            else delete frames;

            delete texture_frame;
            SDL_DestroyTexture(this->texture);

            delete frame_timer;
            frame_timer = NULL;
        }

        //render dynamic coordinates of a texture
        void render(SDL_Renderer *renderer, SDL_Rect *src = NULL){

            //First we check to see if the texture pack is a sprite-sheet
            if (!isAnimated){

                if (src){
                    for (int r = 0, index = 0; r < m_rows; r++){
                        for (int c = 0; c < m_columns; c++, index++){
                            frames[index].w = src->w / m_rows;
                            frames[index].h = src->h / m_columns;
                            frames[index].x = src->x + (frames[index].w * c);
                            frames[index].y = src->y + (frames[index].h * r);
                        }
                    } //if no source is passed to render from, just render x and y as requested from the start
                }

                for (int i = 0; i < frame_count; i++){

                    if (texture)
                        SDL_RenderCopy(renderer, texture, texture_frame, &frames[i]);
                    else{
                        //if there is no texture to be rendered, then just do wireframe
                        SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
                        SDL_RenderDrawRect(renderer, &frames[i]);
                    }
                }
                    
            }
        }

        SDL_Texture *getTexture(){ return texture; }
        void setTexture(SDL_Texture *t){ texture = t; }
    };

};};