#ifndef EPD_EMU_WINDOW_H
#define EPD_EMU_WINDOW_H
#include <stdint.h>

int EpdEmuWindow_Start(uint32_t w, uint32_t h);
void EpdEmuWindow_DrawPx(uint32_t x, uint32_t y, uint8_t color);
void EpdEmuWindow_Render();
void EpdEmuWindow_Destroy();

#endif