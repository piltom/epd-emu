#include "epd_emu.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{
  EpdEmu_Create(SCREEN_WIDTH, SCREEN_HEIGHT, 0xF0);

  if (EpdEmu_StartWindow())
  {
    printf("Error starting window\n");
    return 1;
  }

  uint8_t *mybuf = malloc(800);
  memset(mybuf, 0x00, 80);

  // the load parameters will affect how the buffer is interpreted
  // in this case, 800 bytes at 1 bit per pixel means 3200 pixels
  // With a width of 32px, that makes it 100px in height
  EpdEmu_Write1bpp(32, 10, 32, mybuf, 800);
  sleep(3);
  // after loading in the buffer, we display part of it
  EpdEmu_DisplayArea1bpp(32, 10, 32, 100);

  sleep(3);
  EpdEmu_Destroy();
  free(mybuf);
  return 0;
}