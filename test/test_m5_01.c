#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "measure.h"
void setUp(void) {}
void tearDown(void) {}
void test_stats_small(void) {
    uint16_t s[4] = { 10, 20, 30, 40 }; stats_t st;
    measure_stats(s, 4, &st);
    TEST_ASSERT_EQUAL_UINT16(10, st.min); TEST_ASSERT_EQUAL_UINT16(40, st.max); TEST_ASSERT_EQUAL_UINT32(25, st.avg);
}
void test_stats_min_not_first(void) {
    uint16_t s[5] = { 500, 700, 100, 900, 300 }; stats_t st;
    measure_stats(s, 5, &st);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(100, st.min, "min must be the smallest value, not s[0]");
    TEST_ASSERT_EQUAL_UINT16(900, st.max);
    TEST_ASSERT_EQUAL_UINT32(500, st.avg);
}
void test_stats_single(void) {
    uint16_t s[1] = { 1234 }; stats_t st;
    measure_stats(s, 1, &st);
    TEST_ASSERT_EQUAL_UINT16(1234, st.min); TEST_ASSERT_EQUAL_UINT16(1234, st.max); TEST_ASSERT_EQUAL_UINT32(1234, st.avg);
}
void test_sum_does_not_overflow(void) {
    uint16_t s[256]; stats_t st;
    for (int i = 0; i < 256; i++) s[i] = 4095;
    measure_stats(s, 256, &st);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4095, st.avg, "256 x 4095 = 1,048,320 needs a 32-bit sum");
}
void test_mv(void) {
    stats_t st = { 0, 4095, 1023 };
    TEST_ASSERT_EQUAL_UINT32(3300, stats_vpp_mv(&st));
    TEST_ASSERT_EQUAL_UINT32(824, stats_avg_mv(&st));
    stats_t h = { 1000, 3048, 2048 };
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1650, stats_vpp_mv(&h), "Vpp uses (max - min) = 2048 counts");
    TEST_ASSERT_EQUAL_UINT32(1650, stats_avg_mv(&h));
}
void test_pwm_capture_numbers(void) {
    uint16_t s[256]; stats_t st;
    for (int i = 0; i < 256; i++) s[i] = ((i + 5) % 32 < 8) ? 4090 : 3;
    measure_stats(s, 256, &st);
    TEST_ASSERT_EQUAL_UINT16(3, st.min); TEST_ASSERT_EQUAL_UINT16(4090, st.max);
    TEST_ASSERT_INT_WITHIN(20, 1024, st.avg);
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stats_small); RUN_TEST(test_stats_min_not_first); RUN_TEST(test_stats_single);
    RUN_TEST(test_sum_does_not_overflow); RUN_TEST(test_mv); RUN_TEST(test_pwm_capture_numbers);
    return UNITY_END();
}
