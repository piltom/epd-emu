#ifndef EPD_EMU_H
#define EPD_EMU_H
#include <stdint.h>

typedef struct EpdEmu_image_s
{
  uint32_t len;
  uint8_t *buf;
} EpdEmu_image_t;

int EpdEmu_Create(uint32_t width_px, uint32_t height_px, uint8_t bg_val);

#ifndef TEST
// separated start window call to be able to test the alignment logic without sdl2 or windows popping up
int EpdEmu_StartWindow();
void EpdEmu_DisplayArea1bpp(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
#endif

uint32_t EpdEmu_Write1bpp(uint32_t x, uint32_t y, uint32_t w, const uint8_t *buf, uint32_t len);
EpdEmu_image_t EpdEmu_GetImg();
void EpdEmu_Destroy();

#endif