#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "led.h"
void setUp(void) { fake_chip_reset(); }
void tearDown(void) {}

void test_base_addresses(void) {
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x40011000u, GPIOC_BASE, "GPIOC_BASE must be 0x40011000 (M1-01 step 1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x40021000u, RCC_BASE, "RCC_BASE must be 0x40021000 (M1-01 step 1)");
}
void test_gpio_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(GPIO_TypeDef, CRL));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(GPIO_TypeDef, CRH));
    TEST_ASSERT_EQUAL_HEX32(0x08, offsetof(GPIO_TypeDef, IDR));
    TEST_ASSERT_EQUAL_HEX32(0x0C, offsetof(GPIO_TypeDef, ODR));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(GPIO_TypeDef, BSRR));
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x1C, sizeof(GPIO_TypeDef), "GPIO struct must be 0x1C bytes: add BRR (0x14) and LCKR (0x18) after BSRR (M1-01 step 2)");
}
void test_rcc_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(RCC_TypeDef, CR));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(RCC_TypeDef, CFGR));
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x14, offsetof(RCC_TypeDef, AHBENR), "AHBENR must sit at 0x14: add CIR, APB2RSTR, APB1RSTR between CFGR and AHBENR (M1-01 step 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x18, offsetof(RCC_TypeDef, APB2ENR), "APB2ENR must sit at 0x18 (M1-01 step 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x1C, offsetof(RCC_TypeDef, APB1ENR), "APB1ENR must sit at 0x1C (M1-01 step 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x24, offsetof(RCC_TypeDef, CSR), "CSR must sit at 0x24: add BDCR (0x20) before it (M1-01 step 4)");
    TEST_ASSERT_EQUAL_HEX32(0x28, sizeof(RCC_TypeDef));
}
void test_bit_names(void) {
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x10, RCC_APB2ENR_IOPCEN, "IOPCEN is bit 4 (M1-01 step 5)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(13, LED_PIN, "LED is on PC13 (M1-01 step 5)");
}
void test_field_set(void) {
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFF0Fu, field_set(0xFFFFFFFFu, 4, 4, 0));
    TEST_ASSERT_EQUAL_HEX32(0x00000500u, field_set(0u, 8, 3, 5));
    TEST_ASSERT_EQUAL_HEX32(0x44244444u, field_set(0x44444444u, 20, 4, 2));
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x3u, field_set(0u, 0, 2, 7), "value must be masked to the field width");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x000000D0u, field_set(0x000000F0u, 4, 2, 1), "clear the field first, then set; bits outside the field stay as they were");
}
void test_pin_shift(void) {
    TEST_ASSERT_EQUAL_UINT32(0, pin_shift(0));
    TEST_ASSERT_EQUAL_UINT32(28, pin_shift(7));
    TEST_ASSERT_EQUAL_UINT32(0, pin_shift(8));
    TEST_ASSERT_EQUAL_UINT32(20, pin_shift(13));
    TEST_ASSERT_EQUAL_UINT32(28, pin_shift(15));
}
void test_gpio_cfg(void) {
    GPIO_TypeDef g; g.CRL = 0x44444444u; g.CRH = 0x44444444u;
    gpio_cfg(&g, 13, 2);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x44244444u, g.CRH, "pin 13 lives in CRH, nibble at shift 20");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x44444444u, g.CRL, "CRL must not change for pin 13");
    gpio_cfg(&g, 0, 3);
    TEST_ASSERT_EQUAL_HEX32(0x44444443u, g.CRL);
    gpio_cfg(&g, 9, 0xB);
    TEST_ASSERT_EQUAL_HEX32(0x442444B4u, g.CRH);
}
void test_led_init(void) {
    led_init();
    TEST_ASSERT_TRUE_MESSAGE(fake_rcc.APB2ENR & BIT(4), "led_init must enable the GPIOC clock (APB2ENR bit 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x44244444u, fake_gpioc.CRH, "PC13 must be output 2 MHz push-pull (nibble 0x2)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(BIT(13), fake_gpioc.BSRR, "after init the LED must be OFF (pin high: BSRR bit 13)");
}
void test_led_on_off(void) {
    led_on();  TEST_ASSERT_EQUAL_HEX32_MESSAGE(BIT(29), fake_gpioc.BSRR, "led_on drives the pin LOW: BSRR bit 13+16");
    led_off(); TEST_ASSERT_EQUAL_HEX32_MESSAGE(BIT(13), fake_gpioc.BSRR, "led_off drives the pin HIGH: BSRR bit 13");
}
void test_led_toggle(void) {
    fake_gpioc.ODR = BIT(13);  led_toggle();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(BIT(29), fake_gpioc.BSRR, "pin was high (LED off): toggle must turn it on");
    fake_gpioc.ODR = 0;  led_toggle();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(BIT(13), fake_gpioc.BSRR, "pin was low (LED on): toggle must turn it off");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_base_addresses); RUN_TEST(test_gpio_layout); RUN_TEST(test_rcc_layout); RUN_TEST(test_bit_names);
    RUN_TEST(test_field_set); RUN_TEST(test_pin_shift); RUN_TEST(test_gpio_cfg);
    RUN_TEST(test_led_init); RUN_TEST(test_led_on_off); RUN_TEST(test_led_toggle);
    return UNITY_END();
}
