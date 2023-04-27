#include "epd_emu.h"

#ifndef TEST
#include "epd_emu_window.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CEIL_INT_DIV(x, y) ((x % y) ? x / y + 1 : x / y)
#define ROUND_UP_TO_4_MUL(x) ((x + 3) & ~0x03)

enum BPP
{
  BPP_1 = 1,
  BPP_2 = 2,
  BPP_4 = 4,
};

typedef struct EpdEmu_s
{
  uint32_t width_px;
  uint32_t width_bytes;
  uint32_t height_px;
  enum BPP bpp;
  EpdEmu_image_t img;
} EpdEmu_t;

static EpdEmu_t g_emu;

int EpdEmu_Create(uint32_t width_px, uint32_t height_px, uint8_t bg_val)
{
  EpdEmu_Destroy();
  // for now bpp hardcoded to 1 as it is anyway the only one supporting fast refreshes in 10.3"
  g_emu.bpp = BPP_1;

  uint32_t width_bytes = CEIL_INT_DIV(width_px * g_emu.bpp, 8);
  g_emu.width_bytes = ROUND_UP_TO_4_MUL(width_bytes);
  g_emu.width_px = width_px;
  g_emu.height_px = height_px;

  g_emu.img.buf = malloc(g_emu.width_bytes * g_emu.height_px);
  if (g_emu.img.buf == NULL)
  {
    g_emu.img.len = 0;
    return -1;
  };
  g_emu.img.len = g_emu.width_bytes * g_emu.height_px;
  memset(g_emu.img.buf, bg_val, g_emu.img.len);

  return g_emu.img.len;
}

#ifndef TEST
int EpdEmu_StartWindow()
{
  if (g_emu.img.buf == NULL)
  {
    return -1;
  }

  return EpdEmuWindow_Start(g_emu.width_px, g_emu.height_px);
}

void EpdEmu_DisplayArea1bpp(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
  for (uint32_t x_p = x; x_p < x + w; x_p++)
  {
    for (uint32_t y_p = y; y_p < y + h; y_p++)
    {
      uint32_t pos = x_p / 8 + y_p * g_emu.width_bytes;
      uint8_t color = (g_emu.img.buf[pos] & (0b1 << (x_p % 8))) ? 255 : 0;
      EpdEmuWindow_DrawPx(x_p, y_p, color);
    }
  }
  EpdEmuWindow_Render();
}

#endif

uint32_t EpdEmu_Write1bpp(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint8_t *buf, uint32_t len)
{
  if (g_emu.img.buf == NULL)
  {
    return 0;
  }

  uint32_t xpos_unaligned_bytes = CEIL_INT_DIV(x, 8);
  uint32_t xpos_algined_bytes = ROUND_UP_TO_4_MUL(xpos_unaligned_bytes);

  uint32_t w_bytes = CEIL_INT_DIV(w, 8) + xpos_unaligned_bytes%2;

  uint32_t bytes_written_to_row = 0;
  uint32_t row = y;
  for(int i=0; i<len;i+=2) {
    uint32_t x_bytes = xpos_algined_bytes+bytes_written_to_row;
    uint32_t pos = x_bytes  + row * g_emu.width_bytes;

    g_emu.img.buf[pos] = buf[i];
    g_emu.img.buf[pos+1] = buf[i+1];
    bytes_written_to_row+=2;
    if (bytes_written_to_row>=w_bytes) {
      row++;
      bytes_written_to_row=0;
    }
  }
  return 0;
}

EpdEmu_image_t EpdEmu_GetImg()
{
  return g_emu.img;
}

void EpdEmu_Destroy()
{
#ifndef TEST
  EpdEmuWindow_Destroy();
#endif

  if (g_emu.img.buf != NULL)
  {
    free(g_emu.img.buf);
    g_emu.img.buf = 0;
    g_emu.img.len = 0;
  }
}