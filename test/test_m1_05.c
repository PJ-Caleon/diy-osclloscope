#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "fmt.h"
void setUp(void) {}
void tearDown(void) {}
void test_brr_for(void) {
    TEST_ASSERT_EQUAL_UINT32(625, brr_for(72000000u, 115200u));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(313, brr_for(36000000u, 115200u), "36 MHz / 115200 = 312.5: round to nearest");
    TEST_ASSERT_EQUAL_UINT32(7500, brr_for(72000000u, 9600u));
}
void test_fmt_u32(void) {
    char b[16];
    TEST_ASSERT_EQUAL_UINT32(1, fmt_u32(b, 0));          TEST_ASSERT_EQUAL_STRING("0", b);
    TEST_ASSERT_EQUAL_UINT32(1, fmt_u32(b, 7));          TEST_ASSERT_EQUAL_STRING("7", b);
    TEST_ASSERT_EQUAL_UINT32(3, fmt_u32(b, 123));        TEST_ASSERT_EQUAL_STRING("123", b);
    TEST_ASSERT_EQUAL_UINT32(8, fmt_u32(b, 72000000u));  TEST_ASSERT_EQUAL_STRING("72000000", b);
    TEST_ASSERT_EQUAL_UINT32(10, fmt_u32(b, 4294967295u)); TEST_ASSERT_EQUAL_STRING("4294967295", b);
}
void test_banner(void) {
    char b[48]; banner_build(b);
    TEST_ASSERT_EQUAL_STRING("SCOPE v0.1 clk=72000000\r\n", b);
}
void test_usart_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(USART_TypeDef, SR));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(USART_TypeDef, DR));
    TEST_ASSERT_EQUAL_HEX32(0x08, offsetof(USART_TypeDef, BRR));
    TEST_ASSERT_EQUAL_HEX32(0x0C, offsetof(USART_TypeDef, CR1));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(USART_TypeDef, CR2));
    TEST_ASSERT_EQUAL_HEX32(0x14, offsetof(USART_TypeDef, CR3));
}
void test_uart_code(void) {
    const char *f = "src/uart.c";
    ASSERT_HAS(f, "gpio_cfg(GPIOA, 9u, 0xBu);", "uart_init: PA9 = nibble 0xB (M1-05 step 4)");
    ASSERT_HAS(f, "gpio_cfg(GPIOA, 10u, 0x4u);", "uart_init: PA10 = nibble 0x4 (M1-05 step 4)");
    ASSERT_HAS(f, "USART1->BRR = brr_for(SYSCLK_HZ, 115200u);", "uart_init: BRR from brr_for (M1-05 step 4)");
    ASSERT_HAS(f, "USART_CR1_UE | USART_CR1_TE | USART_CR1_RE", "uart_init: CR1 = UE | TE | RE (M1-05 step 4)");
    ASSERT_HAS(f, "wait_flag(&USART1->SR, USART_SR_TXE, 1,", "uart_putc: wait for TXE (M1-05 step 5)");
    ASSERT_HAS(f, "USART1->DR =", "uart_putc: write DR (M1-05 step 5)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_brr_for); RUN_TEST(test_fmt_u32); RUN_TEST(test_banner); RUN_TEST(test_usart_layout); RUN_TEST(test_uart_code);
    return UNITY_END();
}
