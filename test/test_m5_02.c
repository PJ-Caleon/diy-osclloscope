#include "unity.h"
#include "stm32f103.h"
#include "drills.h"
#include "fakes.h"
#include "support.h"
#include "measure.h"
#include "fmt.h"
void setUp(void) {}
void tearDown(void) {}
void test_freq_phase0(void) {
    uint16_t s[256];
    for (int i = 0; i < 256; i++) s[i] = (i % 32 < 8) ? 4095 : 0;
    TEST_ASSERT_EQUAL_UINT32(1000, measure_freq(s, 256, 2048, 32000));
}
void test_freq_phase5(void) {
    uint16_t s[256];
    for (int i = 0; i < 256; i++) s[i] = ((i + 5) % 32 < 8) ? 4090 : 3;
    TEST_ASSERT_EQUAL_UINT32(1000, measure_freq(s, 256, 2046, 32000));
}
void test_freq_other_rate(void) {
    uint16_t s[256];
    for (int i = 0; i < 256; i++) s[i] = (i % 16 < 4) ? 4095 : 0;   /* 16 samples per period at 16 kSPS = 1 kHz */
    TEST_ASSERT_EQUAL_UINT32(1000, measure_freq(s, 256, 2048, 16000));
    for (int i = 0; i < 256; i++) s[i] = (i % 64 < 16) ? 4095 : 0;  /* 64 samples per period at 800 Hz sample rate = 12.5 Hz */
    TEST_ASSERT_EQUAL_UINT32(12, measure_freq(s, 256, 2048, 800));
}
void test_freq_none(void) {
    uint16_t flat[64]; for (int i = 0; i < 64; i++) flat[i] = 2000;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, measure_freq(flat, 64, 2048, 32000), "no crossing: 0 Hz");
    uint16_t one[64]; for (int i = 0; i < 64; i++) one[i] = (i < 30) ? 0 : 4095;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, measure_freq(one, 64, 2048, 32000), "ONE crossing is not enough to know a period");
}
void test_fmt_mv(void) {
    char b[16];
    TEST_ASSERT_EQUAL_UINT32(5, fmt_mv(b, 3300)); TEST_ASSERT_EQUAL_STRING("3.30V", b);
    fmt_mv(b, 824);  TEST_ASSERT_EQUAL_STRING("0.82V", b);
    fmt_mv(b, 0);    TEST_ASSERT_EQUAL_STRING("0.00V", b);
    fmt_mv(b, 1650); TEST_ASSERT_EQUAL_STRING("1.65V", b);
    fmt_mv(b, 5);    TEST_ASSERT_EQUAL_STRING("0.01V", b);
    fmt_mv(b, 4);    TEST_ASSERT_EQUAL_STRING("0.00V", b);
}
void test_fmt_hz(void) {
    char b[16];
    TEST_ASSERT_EQUAL_UINT32(7, fmt_hz(b, 1000)); TEST_ASSERT_EQUAL_STRING("1.00kHz", b);
    fmt_hz(b, 999);   TEST_ASSERT_EQUAL_STRING("999Hz", b);
    fmt_hz(b, 800);   TEST_ASSERT_EQUAL_STRING("800Hz", b);
    fmt_hz(b, 0);     TEST_ASSERT_EQUAL_STRING("0Hz", b);
    fmt_hz(b, 1006);  TEST_ASSERT_EQUAL_STRING("1.01kHz", b);
    fmt_hz(b, 40000); TEST_ASSERT_EQUAL_STRING("40.00kHz", b);
    fmt_hz(b, 12345); TEST_ASSERT_EQUAL_STRING("12.35kHz", b);
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_freq_phase0); RUN_TEST(test_freq_phase5); RUN_TEST(test_freq_other_rate); RUN_TEST(test_freq_none);
    RUN_TEST(test_fmt_mv); RUN_TEST(test_fmt_hz);
    return UNITY_END();
}
