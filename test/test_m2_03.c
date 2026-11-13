#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "pwm.h"
void setUp(void) {}
void tearDown(void) {}
void test_pwm_ccr(void) {
    TEST_ASSERT_EQUAL_UINT32(250, pwm_ccr(999, 25));
    TEST_ASSERT_EQUAL_UINT32(500, pwm_ccr(999, 50));
    TEST_ASSERT_EQUAL_UINT32(0, pwm_ccr(999, 0));
    TEST_ASSERT_EQUAL_UINT32(1000, pwm_ccr(999, 100));
    TEST_ASSERT_EQUAL_UINT32(45, pwm_ccr(449, 10));
}
void test_count_above(void) {
    uint16_t a[5] = { 0, 4095, 0, 4095, 100 };
    TEST_ASSERT_EQUAL_UINT32(2, count_above(a, 5, 2048));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, count_above(a, 0, 2048), "n = 0: nothing to count");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, count_above(a, 5, 4095), "strictly greater than thr: 4095 is not > 4095");
}
void test_count_above_pwm(void) {
    uint16_t s[256];
    for (int i = 0; i < 256; i++) s[i] = ((i + 5) % 32 < 8) ? 4090 : 3;   /* 1 kHz, 25 % duty, 32 kSPS */
    unsigned c = count_above(s, 256, 2048);
    TEST_ASSERT_TRUE_MESSAGE(c >= 56 && c <= 72, "25% of 256 samples is about 64 (allowed 56..72)");
}
void test_tim_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x18, offsetof(TIM_TypeDef, CCMR1));
    TEST_ASSERT_EQUAL_HEX32(0x20, offsetof(TIM_TypeDef, CCER));
    TEST_ASSERT_EQUAL_HEX32(0x34, offsetof(TIM_TypeDef, CCR1));
    TEST_ASSERT_EQUAL_HEX32(0x38, offsetof(TIM_TypeDef, CCR2));
}
void test_pwm_code(void) {
    const char *f = "src/pwm.c";
    ASSERT_HAS(f, "gpio_cfg(GPIOA, 1u, 0xAu);", "pwm_init: PA1 = nibble 0xA (M2-03 step 2)");
    ASSERT_HAS(f, "TIM2->PSC = 71u;", "pwm_init: PSC 71 (M2-03 step 2)");
    ASSERT_HAS(f, "TIM2->ARR = 999u;", "pwm_init: ARR 999 (M2-03 step 2)");
    ASSERT_HAS(f, "TIM_CCMR1_OC2M_Pos, 3u, 6u", "pwm_init: OC2M = 6 via field_set (M2-03 step 2)");
    ASSERT_HAS(f, "TIM_CCER_CC2E", "pwm_init: enable CC2E (M2-03 step 2)");
    ASSERT_HAS(f, "TIM2->CCR2 = pwm_ccr(TIM2->ARR, percent);", "pwm_set_duty: CCR2 (M2-03 step 3)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_pwm_ccr); RUN_TEST(test_count_above); RUN_TEST(test_count_above_pwm); RUN_TEST(test_tim_layout); RUN_TEST(test_pwm_code);
    return UNITY_END();
}
