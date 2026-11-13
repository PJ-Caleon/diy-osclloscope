#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "memutil.h"
void setUp(void) {}
void tearDown(void) {}

void test_copy_words(void) {
    uint32_t src[5] = { 1, 2, 3, 4, 5 }, dst[6] = { 9, 9, 9, 9, 9, 9 };
    copy_words(dst, dst + 5, src);
    for (int i = 0; i < 5; i++) TEST_ASSERT_EQUAL_UINT32_MESSAGE(src[i], dst[i], "copy_words must copy every word up to dst_end");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(9, dst[5], "copy_words must stop AT dst_end (one past the last word)");
}
void test_copy_zero_length(void) {
    uint32_t src[1] = { 7 }, dst[1] = { 3 };
    copy_words(dst, dst, src);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3, dst[0], "empty range: nothing must be copied");
}
void test_zero_words(void) {
    uint32_t a[5] = { 1, 2, 3, 4, 5 };
    zero_words(a + 1, a + 4);
    TEST_ASSERT_EQUAL_UINT32(1, a[0]);
    TEST_ASSERT_EQUAL_UINT32(0, a[1]); TEST_ASSERT_EQUAL_UINT32(0, a[2]); TEST_ASSERT_EQUAL_UINT32(0, a[3]);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(5, a[4], "zero_words must stop AT end");
}
void test_linker_script(void) {
    const char *f = "ld/stm32f103c6.ld";
    ASSERT_HAS(f, "ENTRY(Reset_Handler)", "ld: first line ENTRY(Reset_Handler) (M1-02 step 5)");
    ASSERT_HAS(f, "FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K", "ld: FLASH line wrong or missing (M1-02 step 5)");
    ASSERT_HAS(f, "RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 10K", "ld: RAM line wrong or missing (M1-02 step 5)");
    ASSERT_HAS(f, "_estack = ORIGIN(RAM) + LENGTH(RAM);", "ld: _estack line (M1-02 step 6)");
    ASSERT_HAS(f, "*(.text*)", "ld: .text must collect *(.text*) (M1-02 step 7)");
    ASSERT_HAS(f, "} > FLASH", "ld: .text must end with > FLASH (M1-02 step 7)");
    ASSERT_HAS(f, "_sidata = LOADADDR(.data);", "ld: _sidata = LOADADDR(.data); (M1-02 step 8)");
    ASSERT_HAS(f, "_sdata = .;", "ld: _sdata marker (M1-02 step 8)");
    ASSERT_HAS(f, "_edata = .;", "ld: _edata marker (M1-02 step 8)");
    ASSERT_HAS(f, "} > RAM AT> FLASH", "ld: .data must be '> RAM AT> FLASH' (M1-02 step 8)");
    ASSERT_HAS(f, "_sbss = .;", "ld: _sbss marker (M1-02 step 9)");
    ASSERT_HAS(f, "_ebss = .;", "ld: _ebss marker (M1-02 step 9)");
}
void test_startup(void) {
    const char *f = "src/startup.c";
    ASSERT_HAS(f, "(isr_t)&_estack", "startup: vector slot 0 must be (isr_t)&_estack (M1-02 step 4)");
    ASSERT_HAS(f, "Reset_Handler,", "startup: vector slot 1 must be Reset_Handler (M1-02 step 4)");
    ASSERT_HAS(f, "SysTick_Handler,", "startup: vector slot 15 must be SysTick_Handler (M1-02 step 4)");
    ASSERT_HAS(f, "copy_words(&_sdata, &_edata, &_sidata);", "startup: Reset_Handler must call copy_words (M1-02 step 3)");
    ASSERT_HAS(f, "zero_words(&_sbss, &_ebss);", "startup: Reset_Handler must call zero_words (M1-02 step 3)");
    ASSERT_HAS(f, "main();", "startup: Reset_Handler must call main() (M1-02 step 3)");
}
void test_makefile(void) {
    const char *f = "Makefile";
    ASSERT_HAS(f, "CROSS = arm-none-eabi-", "Makefile: CROSS (M1-02 step 10)");
    ASSERT_HAS(f, "-mcpu=cortex-m3 -mthumb", "Makefile: CPU flags (M1-02 step 10)");
    ASSERT_HAS(f, "-fno-tree-loop-distribute-patterns", "Makefile: CFLAGS needs -fno-tree-loop-distribute-patterns (M1-02 step 10)");
    ASSERT_HAS(f, "-nostdlib -T ld/stm32f103c6.ld", "Makefile: LDFLAGS needs -nostdlib -T ld/stm32f103c6.ld (M1-02 step 10)");
    ASSERT_HAS(f, "-lgcc", "Makefile: the link line must end with -lgcc (M1-02 step 11)");
    ASSERT_HAS(f, "objcopy -O binary", "Makefile: firmware.bin rule (M1-02 step 11)");
    ASSERT_HAS(f, "firmware:", "Makefile: a `firmware` target (M1-02 step 11)");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_copy_words); RUN_TEST(test_copy_zero_length); RUN_TEST(test_zero_words);
    RUN_TEST(test_linker_script); RUN_TEST(test_startup); RUN_TEST(test_makefile);
    return UNITY_END();
}
