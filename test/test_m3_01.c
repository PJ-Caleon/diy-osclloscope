#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "i2c.h"
void setUp(void) {}
void tearDown(void) {}
void test_i2c_ccr(void) {
    TEST_ASSERT_EQUAL_UINT32(30, i2c_ccr(36000000u, 400000u, 1));
    TEST_ASSERT_EQUAL_UINT32(180, i2c_ccr(36000000u, 100000u, 0));
    TEST_ASSERT_EQUAL_UINT32(120, i2c_ccr(36000000u, 100000u, 1));
}
void test_i2c_layout(void) {
    TEST_ASSERT_EQUAL_HEX32(0x00, offsetof(I2C_TypeDef, CR1));
    TEST_ASSERT_EQUAL_HEX32(0x04, offsetof(I2C_TypeDef, CR2));
    TEST_ASSERT_EQUAL_HEX32(0x10, offsetof(I2C_TypeDef, DR));
    TEST_ASSERT_EQUAL_HEX32(0x14, offsetof(I2C_TypeDef, SR1));
    TEST_ASSERT_EQUAL_HEX32(0x18, offsetof(I2C_TypeDef, SR2));
    TEST_ASSERT_EQUAL_HEX32(0x1C, offsetof(I2C_TypeDef, CCR));
    TEST_ASSERT_EQUAL_HEX32(0x20, offsetof(I2C_TypeDef, TRISE));
}
void test_error_codes(void) {
    TEST_ASSERT_EQUAL_INT(0, I2C_OK); TEST_ASSERT_EQUAL_INT(-1, I2C_ERR_TIMEOUT);
    TEST_ASSERT_EQUAL_INT(-2, I2C_ERR_NACK); TEST_ASSERT_EQUAL_INT(-3, I2C_ERR_BUSY);
}
void test_i2c_code(void) {
    const char *f = "src/i2c.c";
    ASSERT_HAS(f, "gpio_cfg(GPIOB, 6u, 0xFu);", "i2c_init: PB6 nibble 0xF (M3-01 step 2)");
    ASSERT_HAS(f, "gpio_cfg(GPIOB, 7u, 0xFu);", "i2c_init: PB7 nibble 0xF (M3-01 step 2)");
    ASSERT_HAS(f, "i2c_ccr(36000000u, 400000u, 1)", "i2c_init: CCR from i2c_ccr (M3-01 step 2)");
    ASSERT_HAS(f, "I2C1->TRISE = 12u;", "i2c_init: TRISE = 12 (M3-01 step 2)");
    ASSERT_HAS(f, "wait_flag(&I2C1->SR2, I2C_SR2_BUSY, 0,", "i2c_write: wait BUSY clear (M3-01 step 3)");
    ASSERT_HAS(f, "wait_flag(&I2C1->SR1, I2C_SR1_SB, 1,", "i2c_write: wait SB (M3-01 step 3)");
    ASSERT_HAS(f, "I2C_SR1_ADDR | I2C_SR1_AF", "i2c_write: wait ADDR or AF (M3-01 step 3)");
    ASSERT_HAS(f, "I2C_ERR_NACK", "i2c_write: return I2C_ERR_NACK on AF (M3-01 step 3)");
    ASSERT_HAS(f, "I2C_CR1_STOP", "i2c_write: send STOP (M3-01 step 3)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_i2c_ccr); RUN_TEST(test_i2c_layout); RUN_TEST(test_error_codes); RUN_TEST(test_i2c_code);
    return UNITY_END();
}
