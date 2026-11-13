#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "scope.h"
#include "ssd1306.h"
#include "gfx.h"
#include "cal.h"
static uint16_t cap[256];
static scope_t sc;
static int bits_rows(int y0, int y1) { int n = 0; for (int y = y0; y < y1; y++) for (int x = 0; x < 128; x++) n += (g_fb[(y/8)*128 + x] >> (y%8)) & 1; return n; }
void setUp(void) { scope_init(&sc); sc.cap = cap; g_cal.offset = 0; g_cal.span = 4095; fake_i2c_reset(); }
void tearDown(void) {}
static void pwm16(void) { for (int i = 0; i < 256; i++) cap[i] = (i % 16 < 4) ? 4090 : 3; }   /* 1 kHz, 25 %, at 16 kSPS (1 ms/div) */
void test_process_default(void) {
    pwm16(); scope_process(&sc);
    TEST_ASSERT_EQUAL_INT_MESSAGE(16, sc.trig, "first rising crossing of 1650 mV is at index 16");
    TEST_ASSERT_EQUAL_UINT32(16, sc.start);
    TEST_ASSERT_EQUAL_UINT16(3, sc.st.min); TEST_ASSERT_EQUAL_UINT16(4090, sc.st.max);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1000, sc.freq_hz, "1 kHz measured at 16 kSPS");
}
void test_process_falling_and_auto(void) {
    pwm16(); sc.ui.rising = 0; scope_process(&sc);
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, sc.trig, "first falling crossing is at index 4");
    for (int i = 0; i < 256; i++) cap[i] = 1000;
    scope_process(&sc);
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, sc.trig, "flat line: no trigger");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, sc.start, "no trigger: auto, start at 0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, sc.freq_hz, "tiny Vpp: no frequency");
}
void test_header(void) {
    char a[32], b[32];
    pwm16(); scope_process(&sc); scope_header(&sc, a, b);
    TEST_ASSERT_EQUAL_STRING("1ms 1V 1.00kHz R", a);
    TEST_ASSERT_EQUAL_STRING("T p3.29V a0.83V", b);
    sc.ui.mode = M_LEVEL; sc.ui.rising = 0; sc.ui.tdiv_idx = 0; sc.ui.vdiv_idx = 2;
    scope_header(&sc, a, b);
    TEST_ASSERT_EQUAL_UINT8('1', a[0]);
    TEST_ASSERT_EQUAL_UINT8('L', b[0]);
    TEST_ASSERT_EQUAL_UINT8('F', a[(int)strlen(a) - 1]);
    TEST_ASSERT_EQUAL_UINT8_ARRAY("100us 250mV", a, 11);
}
void test_draw(void) {
    pwm16(); scope_process(&sc); scope_draw(&sc);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x42, g_fb[1], "first header glyph '1' (columns 00 42 7F 40 00)");
    TEST_ASSERT_EQUAL_HEX8(0x7F, g_fb[2]);
    TEST_ASSERT_TRUE_MESSAGE(bits_rows(0, 16) > 20, "header text must sit in rows 0..15");
    TEST_ASSERT_TRUE_MESSAGE(bits_rows(16, 64) > 128, "grid + waveform in rows 16..63");
    ssd1306_init(fake_i2c_write); fake_i2c_reset();
    TEST_ASSERT_EQUAL_INT(0, ssd1306_flush());
    TEST_ASSERT_EQUAL_INT(9, fake_i2c_count());
}
void test_draw_does_not_flush(void) {
    ssd1306_init(fake_i2c_write); fake_i2c_reset();
    pwm16(); scope_process(&sc); scope_draw(&sc);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, fake_i2c_count(), "scope_draw must only draw; flushing is main's job");
}
void test_main_loop_code(void) {
    const char *f = "src/main.c";
    ASSERT_HAS(f, "clock_init();", "main: call clock_init() (M6-01 step 4)");
    ASSERT_HAS(f, "ssd1306_init(i2c_write);", "main: ssd1306_init(i2c_write) (M6-01 step 4)");
    ASSERT_HAS(f, "scope_process(", "main: call scope_process (M6-01 step 4)");
    ASSERT_HAS(f, "scope_draw(", "main: call scope_draw (M6-01 step 4)");
    ASSERT_HAS(f, "ssd1306_flush();", "main: call ssd1306_flush (M6-01 step 4)");
    ASSERT_HAS(f, "ui_handle(", "main: call ui_handle for each button event (M6-01 step 4)");
    ASSERT_HAS(f, "for (;;)", "main: super loop (M6-01 step 4)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_process_default); RUN_TEST(test_process_falling_and_auto); RUN_TEST(test_header);
    RUN_TEST(test_draw); RUN_TEST(test_draw_does_not_flush); RUN_TEST(test_main_loop_code);
    return UNITY_END();
}
