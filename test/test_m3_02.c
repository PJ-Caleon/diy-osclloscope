#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "ssd1306.h"
static int bits(void) { int n = 0; for (int i = 0; i < 1024; i++) for (int b = 0; b < 8; b++) n += (g_fb[i] >> b) & 1; return n; }
void setUp(void) { fake_i2c_reset(); fb_clear(); }
void tearDown(void) {}
void test_fb_index_bit(void) {
    TEST_ASSERT_EQUAL_UINT32(0, fb_index(0, 0));
    TEST_ASSERT_EQUAL_UINT32(133, fb_index(5, 9));
    TEST_ASSERT_EQUAL_UINT32(128, fb_index(0, 8));
    TEST_ASSERT_EQUAL_UINT32(1023, fb_index(127, 63));
    TEST_ASSERT_EQUAL_UINT32(0, fb_bit(0));
    TEST_ASSERT_EQUAL_UINT32(1, fb_bit(9));
    TEST_ASSERT_EQUAL_UINT32(7, fb_bit(63));
}
void test_fb_clear(void) {
    g_fb[0] = 1; g_fb[1023] = 0x80; fb_clear();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, bits(), "fb_clear must zero all 1024 bytes");
}
void test_fb_pixel(void) {
    fb_pixel(0, 0, 1);     TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, g_fb[0], "pixel (0,0) = byte 0, bit 0");
    fb_pixel(5, 9, 1);     TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x02, g_fb[133], "pixel (5,9) = byte 133, bit 1");
    fb_pixel(127, 63, 1);  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x80, g_fb[1023], "pixel (127,63) = byte 1023, bit 7");
    fb_pixel(0, 0, 0);     TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, g_fb[0], "on = 0 must clear the bit");
    TEST_ASSERT_EQUAL_INT(2, bits());
    fb_pixel(128, 0, 1); fb_pixel(0, 64, 1);
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, bits(), "pixels outside 128x64 must be ignored");
}
void test_init(void) {
    static const uint8_t want[26] = { 0x00, 0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0x20, 0x00,
                                      0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF };
    TEST_ASSERT_EQUAL_INT(0, ssd1306_init(fake_i2c_write));
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, fake_i2c_count(), "init = ONE write: control byte 0x00 + 25 init bytes");
    TEST_ASSERT_EQUAL_HEX8(0x3C, fake_i2c_addr(0));
    TEST_ASSERT_EQUAL_INT_MESSAGE(26, fake_i2c_len(0), "26 bytes: 0x00 then the 25 init bytes");
    TEST_ASSERT_EQUAL_UINT8_ARRAY(want, fake_i2c_data(0), 26);
}
void test_flush(void) {
    static const uint8_t cmd[7] = { 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07 };
    ssd1306_init(fake_i2c_write); fake_i2c_reset();
    fb_pixel(1, 0, 1); fb_pixel(0, 8, 1);
    TEST_ASSERT_EQUAL_INT(0, ssd1306_flush());
    TEST_ASSERT_EQUAL_INT_MESSAGE(9, fake_i2c_count(), "flush = 1 command write + 8 data writes");
    TEST_ASSERT_EQUAL_INT(7, fake_i2c_len(0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(cmd, fake_i2c_data(0), 7);
    for (int p = 0; p < 8; p++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(129, fake_i2c_len(1 + p), "each page write is 1 + 128 bytes");
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x40, fake_i2c_data(1 + p)[0], "data writes start with control byte 0x40");
        TEST_ASSERT_EQUAL_HEX8(0x3C, fake_i2c_addr(1 + p));
    }
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, fake_i2c_data(1)[2], "pixel (1,0) is page 0, column 1 = data byte index 2");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, fake_i2c_data(2)[1], "pixel (0,8) is page 1, column 0 = data byte index 1");
}
void test_flush_nack(void) {
    ssd1306_init(fake_i2c_write); fake_i2c_reset(); fake_i2c_set_nack(1);
    TEST_ASSERT_EQUAL_INT(-2, ssd1306_flush());
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, fake_i2c_count(), "stop at the first error");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fb_index_bit); RUN_TEST(test_fb_clear); RUN_TEST(test_fb_pixel); RUN_TEST(test_init); RUN_TEST(test_flush); RUN_TEST(test_flush_nack);
    return UNITY_END();
}
