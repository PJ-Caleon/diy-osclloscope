#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "trigger.h"
void setUp(void) {}
void tearDown(void) {}
void test_mv_to_counts(void) {
    TEST_ASSERT_EQUAL_UINT16(0, mv_to_counts(0));
    TEST_ASSERT_EQUAL_UINT16(2047, mv_to_counts(1650));
    TEST_ASSERT_EQUAL_UINT16(4095, mv_to_counts(3300));
    TEST_ASSERT_EQUAL_UINT16(1240, mv_to_counts(1000));
}
void test_rising(void) {
    uint16_t s[8] = { 0, 0, 100, 200, 300, 100, 0, 0 };
    TEST_ASSERT_EQUAL_INT(3, trig_find(s, 8, 150, 1));
}
void test_falling(void) {
    uint16_t s[6] = { 300, 200, 100, 0, 0, 0 };
    TEST_ASSERT_EQUAL_INT(2, trig_find(s, 6, 150, 0));
}
void test_none(void) {
    uint16_t flat[6] = { 200, 300, 400, 300, 200, 100 };
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, trig_find(flat, 6, 150, 1), "a signal that starts above the level has no rising crossing");
    uint16_t s[8] = { 0, 0, 0, 0, 0, 200, 0, 0 };
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, trig_find(s, 5, 150, 1), "only scan the first n_scan samples (indices 1..n_scan-1)");
    TEST_ASSERT_EQUAL_INT(5, trig_find(s, 8, 150, 1));
}
void test_equal_level(void) {
    uint16_t s[3] = { 0, 150, 0 };
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, trig_find(s, 3, 150, 1), "rising: s[i] >= level counts");
    uint16_t f[3] = { 150, 100, 0 };
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, trig_find(f, 3, 150, 0), "falling: s[i-1] >= level counts");
}
void test_trig_start(void) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, trig_start(-1), "no trigger = auto mode, start at 0");
    TEST_ASSERT_EQUAL_UINT32(7, trig_start(7));
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_mv_to_counts); RUN_TEST(test_rising); RUN_TEST(test_falling); RUN_TEST(test_none); RUN_TEST(test_equal_level); RUN_TEST(test_trig_start);
    return UNITY_END();
}
