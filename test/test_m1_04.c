#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "clock.h"
void setUp(void) {}
void tearDown(void) {}
void test_wait_flag_set(void) {
    volatile uint32_t r = 0x4;
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, wait_flag(&r, 0x4, 1, 5), "flag already set: return 0");
    r = 0;
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, wait_flag(&r, 0x4, 1, 5), "flag never set: return -1 after the tries run out");
}
void test_wait_flag_clear(void) {
    volatile uint32_t r = 0;
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, wait_flag(&r, 0x4, 0, 5), "want clear and it is clear: return 0");
    r = 0x4;
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, wait_flag(&r, 0x4, 0, 5), "want clear but it stays set: return -1");
    r = 0x8;
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, wait_flag(&r, 0x4, 0, 5), "only the masked bit counts");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, wait_flag(&r, 0x8, 1, 0), "zero tries: give up at once");
}
void test_elapsed(void) {
    TEST_ASSERT_EQUAL_UINT32(60, elapsed(100, 40));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(21, elapsed(5, 0xFFFFFFF0u), "must survive the 32-bit wrap");
}
void test_reached(void) {
    TEST_ASSERT_TRUE(reached(10, 10));
    TEST_ASSERT_FALSE(reached(9, 10));
    TEST_ASSERT_TRUE_MESSAGE(reached(5, 0xFFFFFFF0u), "deadline just before the wrap, now just after: reached");
    TEST_ASSERT_FALSE_MESSAGE(reached(0xFFFFFFF0u, 5), "deadline just after the wrap, now just before: not reached");
}
void test_cfgr_word(void) {
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x001D8400u, clock_cfgr_word(), "CFGR = PLLSRC(bit16) | PLLMUL 7<<18 | ADCPRE 2<<14 | PPRE1 4<<8");
}
void test_tick_reload(void) {
    TEST_ASSERT_EQUAL_UINT32(71999, tick_reload(72000000u, 1000u));
    TEST_ASSERT_EQUAL_UINT32(31999, tick_reload(32000000u, 1000u));
}
void test_millis(void) {
    uint32_t m0 = millis();
    SysTick_Handler(); SysTick_Handler(); SysTick_Handler();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3, millis() - m0, "each SysTick_Handler call adds 1 to g_ms; millis() returns g_ms");
}
void test_layouts(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(FLASH_TypeDef, ACR));
    TEST_ASSERT_EQUAL_HEX32(0x0C, offsetof(FLASH_TypeDef, SR));
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(SysTick_TypeDef, CTRL));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(SysTick_TypeDef, LOAD));
    TEST_ASSERT_EQUAL_HEX32(0x08, offsetof(SysTick_TypeDef, VAL));
}
void test_clock_init_code(void) {
    const char *f = "src/clock.c";
    ASSERT_HAS(f, "RCC->CR |= RCC_CR_HSEON;", "clock_init: set HSEON (M1-04 step 2)");
    ASSERT_HAS(f, "wait_flag(&RCC->CR, RCC_CR_HSERDY, 1,", "clock_init: wait for HSERDY with wait_flag (M1-04 step 2)");
    ASSERT_HAS(f, "FLASH->ACR", "clock_init: set flash wait states (M1-04 step 3)");
    ASSERT_HAS(f, "RCC->CFGR = clock_cfgr_word();", "clock_init: CFGR = clock_cfgr_word() (M1-04 step 4)");
    ASSERT_HAS(f, "wait_flag(&RCC->CR, RCC_CR_PLLRDY, 1,", "clock_init: wait for PLLRDY (M1-04 step 4)");
    ASSERT_HAS(f, "wait_flag(&RCC->CFGR, RCC_CFGR_SWS_PLL, 1,", "clock_init: wait for SWS (M1-04 step 4)");
    ASSERT_HAS(f, "SYSTICK->LOAD = tick_reload(SYSCLK_HZ, 1000u);", "clock_init: SysTick LOAD (M1-04 step 5)");
    ASSERT_HAS(f, "SYSTICK->CTRL =", "clock_init: SysTick CTRL (M1-04 step 5)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_wait_flag_set); RUN_TEST(test_wait_flag_clear); RUN_TEST(test_elapsed); RUN_TEST(test_reached);
    RUN_TEST(test_cfgr_word); RUN_TEST(test_tick_reload); RUN_TEST(test_millis); RUN_TEST(test_layouts); RUN_TEST(test_clock_init_code);
    return UNITY_END();
}
