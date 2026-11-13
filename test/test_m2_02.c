#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "sampler.h"
void setUp(void) {}
void tearDown(void) {}
static void chk(uint32_t fs, uint32_t psc, uint32_t arr) {
    uint32_t p = 99, a = 99; sampler_timing(fs, &p, &a);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(psc, p, "PSC wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(arr, a, "ARR wrong");
}
void test_timing_table(void) {
    chk(160000, 0, 449); chk(80000, 0, 899); chk(32000, 0, 2249); chk(16000, 0, 4499);
    chk(8000, 0, 8999); chk(3200, 0, 22499); chk(1600, 0, 44999);
    chk(800, 1, 44999);   /* 90000 ticks do not fit in 16 bits: prescaler 1 */
}
void test_timing_exact(void) {
    static const uint32_t fs[8] = { 160000, 80000, 32000, 16000, 8000, 3200, 1600, 800 };
    for (int i = 0; i < 8; i++) {
        uint32_t p, a; sampler_timing(fs[i], &p, &a);
        TEST_ASSERT_TRUE_MESSAGE(a <= 65535u, "ARR must fit in 16 bits");
        TEST_ASSERT_EQUAL_UINT32((uint32_t)72000000u, (p + 1u) * (a + 1u) * fs[i]);
    }
}
void test_layouts(void) {
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(TIM_TypeDef, CR2));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(TIM_TypeDef, SR));
    TEST_ASSERT_EQUAL_HEX32(0x14, offsetof(TIM_TypeDef, EGR));
    TEST_ASSERT_EQUAL_HEX32(0x28, offsetof(TIM_TypeDef, PSC));
    TEST_ASSERT_EQUAL_HEX32(0x2C, offsetof(TIM_TypeDef, ARR));
    TEST_ASSERT_EQUAL_HEX32(0x38, offsetof(TIM_TypeDef, CCR2));
    TEST_ASSERT_EQUAL_HEX32(0x08, offsetof(DMA_TypeDef, ch[0].CCR));
    TEST_ASSERT_EQUAL_HEX32(0x0C, offsetof(DMA_TypeDef, ch[0].CNDTR));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(DMA_TypeDef, ch[0].CPAR));
    TEST_ASSERT_EQUAL_HEX32(0x14, offsetof(DMA_TypeDef, ch[0].CMAR));
    TEST_ASSERT_EQUAL_HEX32(0x1C, offsetof(DMA_TypeDef, ch[1].CCR));
}
void test_sampler_code(void) {
    const char *f = "src/sampler.c";
    ASSERT_HAS(f, "RCC->AHBENR |= RCC_AHBENR_DMA1EN;", "sampler_init: DMA1 clock (M2-02 step 2)");
    ASSERT_HAS(f, "RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;", "sampler_init: TIM3 clock (M2-02 step 2)");
    ASSERT_HAS(f, "TIM_CR2_MMS_Pos, 3u, 2u", "sampler_start: MMS = 2 via field_set (M2-02 step 3)");
    ASSERT_HAS(f, "TIM3->EGR = TIM_EGR_UG;", "sampler_start: EGR = UG (M2-02 step 3)");
    ASSERT_HAS(f, "DMA1->ch[0].CNDTR = SAMPLE_COUNT;", "sampler_start: CNDTR (M2-02 step 4)");
    ASSERT_HAS(f, "DMA_CCR_MINC", "sampler_start: MINC (M2-02 step 4)");
    ASSERT_HAS(f, "ADC_CR2_EXTSEL_Pos, 3u, 4u", "sampler_start: EXTSEL = 4 via field_set (M2-02 step 5)");
    ASSERT_HAS(f, "TIM_CR1_CEN", "sampler_start: start the timer last (M2-02 step 5)");
    ASSERT_HAS(f, "wait_flag(&DMA1->ISR, DMA_ISR_TCIF1, 1,", "sampler_wait: wait for TCIF1 (M2-02 step 6)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_timing_table); RUN_TEST(test_timing_exact); RUN_TEST(test_layouts); RUN_TEST(test_sampler_code);
    return UNITY_END();
}
