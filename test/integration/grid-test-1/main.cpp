#include "../../../include/Xioudown/Xioudown.h"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <SDL.h>

#define EXIT_SUCCESS 0
#define STANDBY system("pause")

Inst_Catalyst(NULL);
using namespace Essentials;
using std::cout;
using std::endl;

int main (int argc, char *argv[]) {

  if (Xioudown_Cat->initDependence()) {
    if (Xioudown_Cat->finalize()) {
      std::map<char, std::string> test;
      test.insert(std::pair<char, std::string>('K', "Hello :D"));
      log(test['K']);
      test['C'] = "HIIII :D";
      log(test['C']);
      /*SDL_Renderer *r = Xioudown_Cat->getWindow()->getRenderer();
      Xioudown_Cat->getManager()->LoadTexture(r, (char*)"basic_stone_brick.png");
      SDL_Texture *texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 500, 500);
      SDL_Rect rect;
      while (Xioudown_Cat->g_gameState()) {
        SDL_RenderClear(r);
        rect.x = rand()%1280;
        rect.y = rand()%960;
        rect.w = 500;
        rect.h = 500;
        int flag = SDL_RENDERER_TARGETTEXTURE;
        SDL_QueryTexture(texture, NULL, &flag, &(rect.w), &(rect.h));
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        log(SDL_SetRenderTarget(r, texture));
        SDL_SetRenderDrawColor(r, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderDrawRect(r, &rect);
        SDL_SetRenderTarget(r, NULL);
        SDL_RenderClear(r);
        SDL_Rect test = {300, 300, 500, 500};
        SDL_RenderCopy(r, texture, NULL, &test);
        SDL_RenderPresent(r);
      }*/
    }
  }
  STANDBY;
  return EXIT_SUCCESS;
}