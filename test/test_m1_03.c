#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
void setUp(void) {}
void tearDown(void) {}
void test_main_blinks(void) {
    ASSERT_HAS("src/main.c", "led_init();", "main.c: call led_init() (M1-03 step 1)");
    ASSERT_HAS("src/main.c", "led_toggle();", "main.c: call led_toggle() in the loop (M1-03 step 1)");
    ASSERT_HAS("src/main.c", "for (;;)", "main.c: needs an endless for (;;) loop (M1-03 step 1)");
}
void test_flash_target(void) {
    ASSERT_HAS("Makefile", "flash: build/firmware.elf", "Makefile: `flash:` target depending on build/firmware.elf (M1-03 step 2)");
    ASSERT_HAS("Makefile", "openocd -f interface/stlink.cfg -f target/stm32f1x.cfg", "Makefile: the openocd line (M1-03 step 2)");
    ASSERT_HAS("Makefile", "program build/firmware.elf verify reset exit", "Makefile: -c \"program build/firmware.elf verify reset exit\" (M1-03 step 2)");
}
void test_tests_active(void) {
    ASSERT_HAS("tests.active", "test_m1_01", "tests.active: add test_m1_01 (M1-03 step 3)");
    ASSERT_HAS("tests.active", "test_m1_02", "tests.active: add test_m1_02 (M1-03 step 3)");
    ASSERT_HAS("tests.active", "test_m1_03", "tests.active: add test_m1_03 (M1-03 step 3)");
}
void test_ci_firmware_job(void) {
    const char *f = ".github/workflows/ci.yml";
    ASSERT_HAS(f, "host-tests:", "ci.yml: the given host-tests job must stay");
    ASSERT_HAS(f, "firmware:", "ci.yml: add a job named `firmware:` (M1-03 step 4)");
    ASSERT_HAS(f, "gcc-arm-none-eabi", "ci.yml: install gcc-arm-none-eabi (M1-03 step 4)");
    ASSERT_HAS(f, "make firmware", "ci.yml: run `make firmware` (M1-03 step 4)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_main_blinks); RUN_TEST(test_flash_target); RUN_TEST(test_tests_active); RUN_TEST(test_ci_firmware_job);
    return UNITY_END();
}
