#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string> //std::string
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Texture manager version 2: singleton class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Astral { namespace Essentials {
    class TextureManager{
    private:
        std::vector<SDL_Texture*> textures;
        TTF_Font* game_font;

    public:
        TextureManager(){  printf("Texture manager instantiated.\n"); }
        ~TextureManager();
        
        int getCount() const{ return (int)textures.size(); }
        
        //just for now; we plan to load plenty of other fonts later.
        bool fontLoaded() const{ return (bool)(game_font != NULL); }

        SDL_Texture *getTexture(int index){

            SDL_Texture *t = NULL;
            if (textures.empty()){
                printf("From %s there are no textures loaded into the manager.\n", __func__ );
                printf("\nError: ", IMG_GetError());
            } else {

                t = textures[index];

                if (!t) {
                    printf("From %s: the texture requested is of NULL value.\n");
                    printf("\nError: ", IMG_GetError());
                    t = NULL;
                }
            }

            return t;
        }

        //Loading textures into the manager
        void LoadTexture( SDL_Renderer* r, char *path ){
            //reference the renderer

            //create a surface for the image.
            SDL_Surface* surface = IMG_Load(path);
            if (!surface){
                printf("From %s Error: %s\n", __func__, IMG_GetError());
            } else {

                SDL_Texture* t = SDL_CreateTextureFromSurface(r, surface);

                if (!t){
                    printf("From %s Error: %s\n", __func__, SDL_GetError() );
                } else {
                    textures.push_back( t);
                    printf("Loaded texture successfully.\n");
                } SDL_FreeSurface(surface);
            }
        }

        SDL_Texture* getFontedTexture( SDL_Renderer *r, char* text, SDL_Color color ){
            //create a surface for the image.

            SDL_Texture* t = NULL;

            if ( game_font != NULL ){

                SDL_Surface* surface = TTF_RenderText_Solid( game_font, text, color);
                if (!surface){
                    printf("From %s Error: %s\n", __func__, IMG_GetError());
                } else {
                    t = SDL_CreateTextureFromSurface(r, surface);
                    
                    if (!t){
                        printf("From %s Error: %s\n", __func__, SDL_GetError() );
                        t = NULL;
                    } else {
                        return t;
                    } SDL_FreeSurface(surface);
                }
            } 
            return t;
        }

        void LoadFont( char *path, int font_sz = 12){
            
            game_font = TTF_OpenFont(path, font_sz);

            if (!game_font){
                printf("From %s The problem is:", __func__, TTF_GetError() );
            } else {
                printf("Font loaded successfully\n");
            } 
        }

        SDL_Surface* loadSurface( SDL_Renderer* r, SDL_Surface *s, std::string path )
        {
            //The final optimized image
            SDL_Surface* optimizedSurface = NULL;

            //Load image at specified path
            SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
            if( loadedSurface == NULL ){
                printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            } else {
                //Convert surface to screen format
                optimizedSurface = SDL_ConvertSurface( loadedSurface, s->format, 0 );
                if( optimizedSurface == NULL )
                    printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );


                //Get rid of old loaded surface
                SDL_FreeSurface( loadedSurface );
            }

            return optimizedSurface;
        }
    };
};};
