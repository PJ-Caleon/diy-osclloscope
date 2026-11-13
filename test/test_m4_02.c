#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "ui.h"
#include "sampler.h"
void setUp(void) {}
void tearDown(void) {}
void test_tables(void) {
    static const uint32_t t[8] = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
    static const uint32_t v[3] = { 1000, 500, 250 };
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)t, (const uint8_t *)tdiv_us, sizeof t);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)v, (const uint8_t *)vdiv_mv, sizeof v);
}
void test_tdiv_to_fs(void) {
    static const uint32_t fs[8] = { 160000, 80000, 32000, 16000, 8000, 3200, 1600, 800 };
    for (int i = 0; i < 8; i++) TEST_ASSERT_EQUAL_UINT32(fs[i], tdiv_to_fs(i));
}
void test_rates_fit_timer(void) {
    for (int i = 0; i < 8; i++) {
        uint32_t fs = tdiv_to_fs(i), p, a;
        sampler_timing(fs, &p, &a);
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(72000000u, (p + 1u) * (a + 1u) * fs, "every rate must divide 72 MHz exactly");
    }
}
void test_ui_init(void) {
    ui_t u; u.mode = M_EDGE; u.tdiv_idx = 7; u.vdiv_idx = 2; u.level_mv = 0; u.rising = 0;
    ui_init(&u);
    TEST_ASSERT_EQUAL_INT(M_TDIV, u.mode); TEST_ASSERT_EQUAL_INT(3, u.tdiv_idx); TEST_ASSERT_EQUAL_INT(0, u.vdiv_idx);
    TEST_ASSERT_EQUAL_INT(1650, u.level_mv); TEST_ASSERT_EQUAL_INT(1, u.rising);
}
void test_step_index(void) {
    TEST_ASSERT_EQUAL_INT(4, step_index(3, 1, 7));
    TEST_ASSERT_EQUAL_INT_MESSAGE(7, step_index(7, 1, 7), "stop at max");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, step_index(0, -1, 7), "stop at 0");
    TEST_ASSERT_EQUAL_INT(2, step_index(2, 1, 2));
}
void test_labels(void) {
    TEST_ASSERT_EQUAL_STRING("1ms", tdiv_label[3]);
    TEST_ASSERT_EQUAL_STRING("250mV", vdiv_label[2]);
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tables); RUN_TEST(test_tdiv_to_fs); RUN_TEST(test_rates_fit_timer); RUN_TEST(test_ui_init); RUN_TEST(test_step_index); RUN_TEST(test_labels);
    return UNITY_END();
}
