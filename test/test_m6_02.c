#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "cal.h"
#include "scope.h"
void setUp(void) { g_cal.offset = 0; g_cal.span = 4095; }
void tearDown(void) { g_cal.offset = 0; g_cal.span = 4095; }
void test_fit(void) {
    cal_t c = { 0, 0 };
    TEST_ASSERT_EQUAL_INT(0, cal_fit(&c, 5, 4090));
    TEST_ASSERT_EQUAL_UINT16(5, c.offset); TEST_ASSERT_EQUAL_UINT16(4085, c.span);
    cal_t d = { 1, 2 };
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, cal_fit(&d, 4000, 4090), "span under 2000 counts: reject");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(1, d.offset, "a rejected fit must not change c");
}
void test_apply(void) {
    cal_t c = { 5, 4085 };
    TEST_ASSERT_EQUAL_UINT16(0, cal_apply(&c, 5));
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, cal_apply(&c, 2), "below the offset: 0, no wrap-around");
    TEST_ASSERT_EQUAL_UINT16(4095, cal_apply(&c, 4090));
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(4095, cal_apply(&c, 4095), "above the span: clamp to 4095");
    TEST_ASSERT_EQUAL_UINT16(2046, cal_apply(&c, 2047));
}
void test_identity(void) {
    TEST_ASSERT_EQUAL_UINT16(1234, cal_apply(&g_cal, 1234));
    TEST_ASSERT_EQUAL_UINT16(4095, cal_apply(&g_cal, 4095));
}
void test_scope_uses_cal(void) {
    uint16_t cap[256]; scope_t sc;
    scope_init(&sc); sc.cap = cap;
    g_cal.offset = 100; g_cal.span = 4000;
    for (int i = 0; i < 256; i++) cap[i] = 100;
    scope_process(&sc);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, cap[0], "scope_process must run samples through cal_apply (M6-02 step 3)");
    ASSERT_HAS("src/scope.c", "cal_apply(&g_cal,", "scope.c: call cal_apply(&g_cal, ...) in scope_process (M6-02 step 3)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fit); RUN_TEST(test_apply); RUN_TEST(test_identity); RUN_TEST(test_scope_uses_cal);
    return UNITY_END();
}
