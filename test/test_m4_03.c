#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "ui.h"
void setUp(void) {}
void tearDown(void) {}
void test_debounce_clean_press(void) {
    debounce_t d = { 0, 0, 0 };
    TEST_ASSERT_EQUAL_INT(0, debounce_update(&d, 1, 0));
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, debounce_update(&d, 1, 10), "only 10 ms stable: not yet");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, debounce_update(&d, 1, 20), "20 ms stable: report ONE press");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, debounce_update(&d, 1, 30), "still held: no second press");
    TEST_ASSERT_EQUAL_INT(0, debounce_update(&d, 0, 100));
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, debounce_update(&d, 0, 130), "a release is not a press");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, debounce_update(&d, 1, 200), "press starts again");
    TEST_ASSERT_EQUAL_INT(1, debounce_update(&d, 1, 221));
}
void test_debounce_bounce(void) {
    debounce_t d = { 0, 0, 0 };
    int fired = 0;
    static const int raw[] = { 1, 0, 1, 0, 1, 1, 0, 1 };
    for (int i = 0; i < 8; i++) fired += debounce_update(&d, raw[i], (uint32_t)(i * 3));
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, fired, "bouncing every 3 ms must never fire");
    TEST_ASSERT_EQUAL_INT(1, debounce_update(&d, 1, 21 + 20));
}
void test_debounce_wrap(void) {
    debounce_t d = { 0, 0, 0 };
    debounce_update(&d, 1, 0xFFFFFFF0u);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, debounce_update(&d, 1, 0xFFFFFFFFu), "15 ms across the wrap: not yet");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, debounce_update(&d, 1, 0x00000004u), "20 ms across the wrap: fire");
}
static ui_t fresh(void) { ui_t u; ui_init(&u); return u; }
void test_ui_tdiv(void) {
    ui_t u = fresh();
    ui_handle(&u, EV_UP);   TEST_ASSERT_EQUAL_INT(4, u.tdiv_idx);
    ui_handle(&u, EV_DOWN); ui_handle(&u, EV_DOWN); TEST_ASSERT_EQUAL_INT(2, u.tdiv_idx);
    for (int i = 0; i < 10; i++) ui_handle(&u, EV_UP);
    TEST_ASSERT_EQUAL_INT_MESSAGE(7, u.tdiv_idx, "tdiv_idx stops at 7");
    for (int i = 0; i < 10; i++) ui_handle(&u, EV_DOWN);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, u.tdiv_idx, "tdiv_idx stops at 0");
    TEST_ASSERT_EQUAL_INT(M_TDIV, u.mode);
}
void test_ui_mode_cycle(void) {
    ui_t u = fresh();
    ui_handle(&u, EV_MODE); TEST_ASSERT_EQUAL_INT(M_VDIV, u.mode);
    ui_handle(&u, EV_MODE); TEST_ASSERT_EQUAL_INT(M_LEVEL, u.mode);
    ui_handle(&u, EV_MODE); TEST_ASSERT_EQUAL_INT(M_EDGE, u.mode);
    ui_handle(&u, EV_MODE); TEST_ASSERT_EQUAL_INT(M_TDIV, u.mode);
}
void test_ui_vdiv_level_edge(void) {
    ui_t u = fresh();
    ui_handle(&u, EV_MODE);
    ui_handle(&u, EV_DOWN); TEST_ASSERT_EQUAL_INT_MESSAGE(0, u.vdiv_idx, "vdiv stops at 0");
    ui_handle(&u, EV_UP); ui_handle(&u, EV_UP); ui_handle(&u, EV_UP);
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, u.vdiv_idx, "vdiv stops at 2");
    ui_handle(&u, EV_MODE);
    ui_handle(&u, EV_UP);   TEST_ASSERT_EQUAL_INT(1750, u.level_mv);
    ui_handle(&u, EV_DOWN); ui_handle(&u, EV_DOWN); TEST_ASSERT_EQUAL_INT(1550, u.level_mv);
    for (int i = 0; i < 40; i++) ui_handle(&u, EV_UP);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3300, u.level_mv, "level stops at 3300 mV");
    for (int i = 0; i < 40; i++) ui_handle(&u, EV_DOWN);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, u.level_mv, "level stops at 0 mV");
    ui_handle(&u, EV_MODE);
    ui_handle(&u, EV_UP);   TEST_ASSERT_EQUAL_INT(0, u.rising);
    ui_handle(&u, EV_DOWN); TEST_ASSERT_EQUAL_INT(1, u.rising);
}
void test_ui_none_event(void) {
    ui_t u = fresh(); ui_handle(&u, EV_NONE);
    TEST_ASSERT_EQUAL_INT(M_TDIV, u.mode); TEST_ASSERT_EQUAL_INT(3, u.tdiv_idx);
}
void test_buttons_code(void) {
    const char *f = "src/ui.c";
    ASSERT_HAS(f, "gpio_cfg(GPIOB, 12u, 0x8u);", "buttons_init: PB12 nibble 0x8 (M4-03 step 4)");
    ASSERT_HAS(f, "gpio_cfg(GPIOB, 13u, 0x8u);", "buttons_init: PB13 nibble 0x8 (M4-03 step 4)");
    ASSERT_HAS(f, "gpio_cfg(GPIOB, 14u, 0x8u);", "buttons_init: PB14 nibble 0x8 (M4-03 step 4)");
    ASSERT_HAS(f, "GPIOB->BSRR = BIT(12) | BIT(13) | BIT(14);", "buttons_init: BSRR bits 12..14 select pull-UP (M4-03 step 4)");
    ASSERT_HAS(f, "GPIOB->IDR", "buttons_read: read IDR (M4-03 step 5)");
    ASSERT_HAS(f, ">> 12) & 7u", "buttons_read: shift by 12 and mask with 7 (M4-03 step 5)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_debounce_clean_press); RUN_TEST(test_debounce_bounce); RUN_TEST(test_debounce_wrap);
    RUN_TEST(test_ui_tdiv); RUN_TEST(test_ui_mode_cycle); RUN_TEST(test_ui_vdiv_level_edge); RUN_TEST(test_ui_none_event); RUN_TEST(test_buttons_code);
    return UNITY_END();
}
