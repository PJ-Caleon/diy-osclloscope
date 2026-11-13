#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "ssd1306.h"
#include "gfx.h"
static int px(unsigned x, unsigned y) { return (g_fb[(y / 8) * 128 + x] >> (y % 8)) & 1; }
static int bits(void) { int n = 0; for (int i = 0; i < 1024; i++) for (int b = 0; b < 8; b++) n += (g_fb[i] >> b) & 1; return n; }
void setUp(void) { fb_clear(); }
void tearDown(void) {}
void test_clamp_i(void) {
    TEST_ASSERT_EQUAL_INT(5, clamp_i(5, 0, 10));
    TEST_ASSERT_EQUAL_INT(0, clamp_i(-3, 0, 10));
    TEST_ASSERT_EQUAL_INT(10, clamp_i(11, 0, 10));
    TEST_ASSERT_EQUAL_INT(16, clamp_i(16, 16, 63));
}
void test_grid(void) {
    draw_grid();
    TEST_ASSERT_TRUE_MESSAGE(px(0, 16), "corner (0,16) must be a grid dot");
    TEST_ASSERT_TRUE_MESSAGE(px(16, 16), "vertical line x=16 starts at y=16");
    TEST_ASSERT_TRUE_MESSAGE(px(4, 24), "horizontal line y=24, dot every 4th x");
    TEST_ASSERT_TRUE_MESSAGE(px(127, 40), "last vertical line is x=127");
    TEST_ASSERT_TRUE_MESSAGE(px(8, 63), "bottom line y=63");
    TEST_ASSERT_FALSE_MESSAGE(px(1, 17), "dots only: (1,17) must stay empty");
    TEST_ASSERT_FALSE_MESSAGE(px(16, 17), "vertical lines use every even y only");
    for (int y = 0; y < 16; y++) for (int x = 0; x < 128; x++) TEST_ASSERT_FALSE_MESSAGE(px(x, y), "grid must not touch the text rows 0..15");
}
void test_text(void) {
    static const uint8_t A[5] = { 0x7E, 0x11, 0x11, 0x11, 0x7E };
    text_draw(0, 0, "A");
    TEST_ASSERT_EQUAL_UINT8_ARRAY(A, g_fb, 5);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, g_fb[5], "one blank column after each glyph");
    fb_clear(); text_draw(0, 0, "AB");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x7F, g_fb[6], "second character starts 6 pixels right");
    fb_clear(); text_draw(0, 8, "A");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x7E, g_fb[128], "y = 8 is page 1");
    fb_clear(); text_draw(124, 0, "AA");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, g_fb[128], "text past the right edge must be clipped, not wrapped");
}
void test_plot_flat(void) {
    uint16_t s[128]; for (int i = 0; i < 128; i++) s[i] = 4095;
    plot_wave(s, 128, 1000);
    for (unsigned x = 0; x < 128; x++) {
        TEST_ASSERT_TRUE_MESSAGE(px(x, 37), "3.3 V at 1 V/div = 26 rows above row 63 = row 37");
        TEST_ASSERT_FALSE_MESSAGE(px(x, 36), "only one pixel per column for a flat line");
    }
}
void test_plot_clip(void) {
    uint16_t s[128]; for (int i = 0; i < 128; i++) s[i] = 4095;
    plot_wave(s, 128, 500);
    TEST_ASSERT_TRUE_MESSAGE(px(10, 16), "at 500 mV/div 3.3 V is off screen: clip to the top plot row 16");
    TEST_ASSERT_EQUAL_INT_MESSAGE(128, bits(), "nothing above row 16");
}
void test_plot_step(void) {
    uint16_t s[128]; for (int i = 0; i < 128; i++) s[i] = (i < 64) ? 0 : 4095;
    plot_wave(s, 128, 1000);
    TEST_ASSERT_TRUE(px(0, 63)); TEST_ASSERT_TRUE(px(63, 63));
    for (unsigned y = 37; y <= 63; y++) TEST_ASSERT_TRUE_MESSAGE(px(64, y), "an edge must be drawn as a vertical line");
    TEST_ASSERT_TRUE(px(127, 37));
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_clamp_i); RUN_TEST(test_grid); RUN_TEST(test_text); RUN_TEST(test_plot_flat); RUN_TEST(test_plot_clip); RUN_TEST(test_plot_step);
    return UNITY_END();
}
