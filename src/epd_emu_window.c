#include "epd_emu_window.h"
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Renderer *renderer = NULL;

int EpdEmuWindow_Start(uint32_t w, uint32_t h)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return -1;

  SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

  if (window == NULL || renderer == NULL)
    return -2;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  return 0;
}

void EpdEmuWindow_DrawPx(uint32_t x, uint32_t y, uint8_t color)
{
  SDL_SetRenderDrawColor(renderer, color, color, color, 255);
  SDL_RenderDrawPoint(renderer, x, y);
}

void EpdEmuWindow_Render()
{
  SDL_RenderPresent(renderer);
}

void EpdEmuWindow_Destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}