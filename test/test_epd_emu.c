#include "unity.h"
#include "epd_emu.h"

#include <stdio.h>

#define TEST_SCREEN_W_PX 64
#define TEST_SCREEN_H_PX 5
#define TEST_SCREEN_BUF_SIZE 64 / 8 * 5

static const void print_buf(const uint8_t *buf, uint32_t len, uint32_t perrow)
{
  for (int i = 0; i < len; i++)
  {
    if (i % perrow == 0)
    {
      printf("\n");
    }
    printf("%d,", buf[i]);
  }

  printf("\n");
}

static const uint8_t SAMPLE_BUF_PADDED[] = {
    0x00, 0x01, 0x02, 0x03,
    0x10, 0x11, 0x12, 0x13,
    0x20, 0x21, 0x22, 0x23,
};

static const uint8_t SAMPLE_BUF_NOT_PADDED[] = {
    0x00, 0x01, 0x02,
    0x10, 0x11, 0x12,
    0x20, 0x21, 0x22,
};

void setUp(void)
{
  int ret = EpdEmu_Create(TEST_SCREEN_W_PX, TEST_SCREEN_H_PX, 0x00);
  TEST_ASSERT_EQUAL(TEST_SCREEN_BUF_SIZE, ret);
};

void tearDown(void)
{
  EpdEmu_Destroy();
};

void test_write_aligned_and_padded(void)
{
  uint32_t len = sizeof(SAMPLE_BUF_PADDED);
  int ret = EpdEmu_Write1bpp(0, 0, 32, SAMPLE_BUF_PADDED, len);
  TEST_ASSERT_EQUAL(0, ret);
  EpdEmu_image_t result = EpdEmu_GetImg();

  static const uint8_t EXPECTED[] = {
      0x00, 0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x11, 0x12, 0x13, 0x00, 0x00, 0x00, 0x00,
      0x20, 0x21, 0x22, 0x23, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  TEST_ASSERT_EQUAL(sizeof(EXPECTED), result.len);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, result.buf, result.len);
}

void test_write_unaligned_and_padded(void)
{
  uint32_t len = sizeof(SAMPLE_BUF_PADDED);

  int ret = EpdEmu_Write1bpp(20, 0, 32, SAMPLE_BUF_PADDED, len);
  TEST_ASSERT_EQUAL(0, ret);

  EpdEmu_image_t result = EpdEmu_GetImg();
  static const uint8_t EXPECTED[] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
      0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13,
      0x00, 0x00, 0x00, 0x00, 0x20, 0x21, 0x22, 0x23,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  print_buf(result.buf, result.len, 8);
  TEST_ASSERT_EQUAL(sizeof(EXPECTED), result.len);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, result.buf, result.len);
}

void test_write_aligned_unpadded(void)
{
  uint32_t len = sizeof(SAMPLE_BUF_NOT_PADDED);

  int ret = EpdEmu_Write1bpp(0, 0, 24, SAMPLE_BUF_NOT_PADDED, len);
  TEST_ASSERT_EQUAL(0, ret);

  EpdEmu_image_t result = EpdEmu_GetImg();
  static const uint8_t EXPECTED[] = {
      0x00, 0x01, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00,
      0x11, 0x12, 0X20, 0x21, 0x00, 0x00, 0x00, 0x00,
      0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  print_buf(result.buf, result.len, 8);
  TEST_ASSERT_EQUAL(sizeof(EXPECTED), result.len);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED, result.buf, result.len);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_write_aligned_and_padded);
  RUN_TEST(test_write_unaligned_and_padded);
  RUN_TEST(test_write_aligned_unpadded);
  return UNITY_END();
}