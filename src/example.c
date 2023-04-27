#include "epd_emu.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

void paint_pixel_1bpp(uint32_t x, uint32_t y, uint32_t wbyte, uint8_t* buf) {
  uint32_t addr = x / 8 + y * wbyte;
  uint32_t shift = x % 8;
  buf[addr] &= ~(0b1 << shift);
}

typedef struct {
  size_t buf_size;
  uint8_t* buf;
} tom_img;

void paint_rect_1bpp(int32_t xs,int32_t ys,int32_t xe,int32_t ye, uint32_t wbyte, uint8_t* buf){
  for (int32_t x=xs; x<xe; x++) {
      for (int32_t y=ys; y<ye; y++) {
          paint_pixel_1bpp(x, y, wbyte, buf);
      }
    }
}

tom_img alloc_and_paint_test_img_1bpp(int32_t size, int32_t rect_size) {
  tom_img ret;
  uint32_t w = size;
  uint32_t h = size;
  uint32_t wbyte = (w % 8 == 0)? w / 8 : w / 8 + 1;

  ret.buf_size = h * wbyte;
  ret.buf = malloc(ret.buf_size);
  memset(ret.buf, 0xFF, ret.buf_size);
  if (ret.buf) {
    // top left corner rect, 8 px from left
    paint_rect_1bpp(8, 0, 8+rect_size, rect_size, wbyte, ret.buf);
    // top right corner rect, 8 px from right
    paint_rect_1bpp(size-rect_size-8, 0, size-8, rect_size, wbyte, ret.buf);
    // centered rect
    paint_rect_1bpp(size/2-rect_size/2, size/2-rect_size/2, size/2+rect_size/2, size/2+rect_size/2, wbyte, ret.buf);
    // botom left and bottom right rects
    paint_rect_1bpp(8, size-rect_size, 8+rect_size, size, wbyte, ret.buf);
    paint_rect_1bpp(size-rect_size-8, size-rect_size, size-8, size, wbyte, ret.buf);
  }
  return ret;
}

int main(int argc, char *args[])
{
  EpdEmu_Create(SCREEN_WIDTH, SCREEN_HEIGHT, 0xF0);

  if (EpdEmu_StartWindow())
  {
    printf("Error starting window\n");
    return 1;
  }

  tom_img mybuf = alloc_and_paint_test_img_1bpp(704, 16);

  EpdEmu_Write1bpp(104, 40, 704, 704, mybuf.buf, mybuf.buf_size);
  EpdEmu_DisplayArea1bpp(104,40, 704, 704);
  //EpdEmu_DisplayArea1bpp(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);

  sleep(30);
  EpdEmu_Destroy();
  free(mybuf.buf);
  return 0;
}