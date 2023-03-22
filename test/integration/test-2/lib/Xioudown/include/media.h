#pragma once
#include "xioudown_predefs.h"
#include <string>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define GET_IMG_ERROR IMG_GetError()
using namespace Xioudown::Essentials;

namespace Xioudown {
    namespace Media {
        // std::string DEFAULT_PNG = "../src/data/media/img/default.png";
        
        class Texture : public XioudownClass {
            private:
                SDL_Texture *m_texture;
                std::string m_path;
                bool loaded = false;
            public:
                Texture(std::string _path = "src/data/media/img/default.png") {
                    _path = _path.empty() ? "src/data/media/img/default.png" : _path;
                    m_path = _path;
                }
                
                ~Texture() {
                    SDL_DestroyTexture(m_texture);
                    m_texture = nullptr;
                }

                SDL_Texture* body() { return this->m_texture; }
                std::string path() { return m_path; }
            public:
                SDL_Texture* load(SDL_Renderer *_renderer, std::string _path = "") {
                    std::string file_path = !_path.empty() ? _path : m_path;
                    if (file_path.empty()) printf("Path provided is not present.\n");
                    else
                    {
                        printf("Path provided is present.\n");
                        //Load image at specified path
                        SDL_Surface* loaded_surface = IMG_Load(file_path.c_str());
                        if( loaded_surface == nullptr ) 
                        {
                            printf( "Unable to load image %s! SDL_image Error:\n", GET_IMG_ERROR);
                        }
                        else
                        {
                            m_texture = SDL_CreateTextureFromSurface(_renderer, loaded_surface);
                            if(m_texture == nullptr)
                            {
                                printf( "Unable to create texture from %s! SDL Error: %s\n", file_path.c_str(), SDL_GetError() );
                            }
                            SDL_FreeSurface(loaded_surface); //Get rid of old loaded surface
                            loaded = true;
                        }
                    }
                    return m_texture;
                }
    };
};};
