#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "adc.h"
void setUp(void) {}
void tearDown(void) {}
void test_mul_div(void) {
    TEST_ASSERT_EQUAL_UINT32(3300, mul_div(4095, 3300, 4095));
    TEST_ASSERT_EQUAL_UINT32(1650, mul_div(2048, 3300, 4095));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0xFFFFFFFFu, mul_div(0xFFFFFFFFu, 100, 100), "the product must be computed in 64 bits");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, mul_div(1, 1, 2), "multiply first, divide last: 1*1/2 = 0");
}
void test_smpr_field(void) {
    TEST_ASSERT_EQUAL_UINT32(0, smpr_field(0));
    TEST_ASSERT_EQUAL_UINT32(6, smpr_field(2));
    TEST_ASSERT_EQUAL_UINT32(27, smpr_field(9));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, smpr_field(10), "channels 10..17 live in SMPR1: restart at shift 0");
    TEST_ASSERT_EQUAL_UINT32(18, smpr_field(16));
    TEST_ASSERT_EQUAL_UINT32(21, smpr_field(17));
}
void test_adc_to_mv(void) {
    TEST_ASSERT_EQUAL_UINT32(0, adc_to_mv(0));
    TEST_ASSERT_EQUAL_UINT32(3300, adc_to_mv(4095));
    TEST_ASSERT_EQUAL_UINT32(1650, adc_to_mv(2048));
    TEST_ASSERT_EQUAL_UINT32(824, adc_to_mv(1023));
}
void test_adc_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(ADC_TypeDef, SR));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(ADC_TypeDef, CR1));
    TEST_ASSERT_EQUAL_HEX32(0x08, offsetof(ADC_TypeDef, CR2));
    TEST_ASSERT_EQUAL_HEX32(0x0C, offsetof(ADC_TypeDef, SMPR1));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(ADC_TypeDef, SMPR2));
    TEST_ASSERT_EQUAL_HEX32(0x24, offsetof(ADC_TypeDef, HTR));
    TEST_ASSERT_EQUAL_HEX32(0x2C, offsetof(ADC_TypeDef, SQR1));
    TEST_ASSERT_EQUAL_HEX32(0x34, offsetof(ADC_TypeDef, SQR3));
    TEST_ASSERT_EQUAL_HEX32(0x4C, offsetof(ADC_TypeDef, DR));
}
void test_adc_code(void) {
    const char *f = "src/adc.c";
    ASSERT_HAS(f, "gpio_cfg(GPIOA, 2u, 0x0u);", "adc_init: PA2 = analog, nibble 0x0 (M2-01 step 3)");
    ASSERT_HAS(f, "smpr_field(2u)", "adc_init: use smpr_field(2) for the sample-time shift (M2-01 step 3)");
    ASSERT_HAS(f, "ADC1->SQR3", "adc_init: set SQR3 to channel 2 (M2-01 step 3)");
    ASSERT_HAS(f, "ADC_CR2_CAL", "adc_init: run the calibration (M2-01 step 3)");
    ASSERT_HAS(f, "ADC_CR2_SWSTART", "adc_read: start with SWSTART (M2-01 step 4)");
    ASSERT_HAS(f, "wait_flag(&ADC1->SR, ADC_SR_EOC, 1,", "adc_read: wait for EOC (M2-01 step 4)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_mul_div); RUN_TEST(test_smpr_field); RUN_TEST(test_adc_to_mv); RUN_TEST(test_adc_layout); RUN_TEST(test_adc_code);
    return UNITY_END();
}
