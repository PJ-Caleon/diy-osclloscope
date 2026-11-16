/* Minimal Unity-compatible test header (stand-in).
 * It implements the subset of the Unity API that this repo uses, with the same macro names.
 * The upstream framework is ThrowTheSwitch/Unity (MIT licence). To use the real one, copy its
 * src/unity.c, src/unity.h and src/unity_internals.h over these files. No test needs changing. */
#ifndef UNITY_STANDIN_H
#define UNITY_STANDIN_H
#include <stdint.h>
#include <stddef.h>
void UnityBegin(const char *file);
int  UnityEnd(void);
void UnityRun(void (*fn)(void), const char *name);
void UnityFailAt(const char *msg, int line);
void UnityNum(long long exp, long long act, int hex, const char *msg, int line);
void UnityStr(const char *exp, const char *act, const char *msg, int line);
void UnityArr(const void *exp, const void *act, size_t n, size_t elem, const char *msg, int line);
void UnityWithin(long long delta, long long exp, long long act, const char *msg, int line);
void UnityTrue(int cond, const char *msg, int line);
void setUp(void);
void tearDown(void);
#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()
#define RUN_TEST(f) UnityRun(f, #f)
#define TEST_FAIL_MESSAGE(m) UnityFailAt((m), __LINE__)
#define TEST_FAIL() UnityFailAt(NULL, __LINE__)
#define TEST_MESSAGE(m) ((void)0)
#define TEST_ASSERT(c) UnityTrue(!!(c), "Expression Evaluated To FALSE", __LINE__)
#define TEST_ASSERT_TRUE(c) UnityTrue(!!(c), "Expected TRUE Was FALSE", __LINE__)
#define TEST_ASSERT_FALSE(c) UnityTrue(!(c), "Expected FALSE Was TRUE", __LINE__)
#define TEST_ASSERT_TRUE_MESSAGE(c, m) UnityTrue(!!(c), (m), __LINE__)
#define TEST_ASSERT_FALSE_MESSAGE(c, m) UnityTrue(!(c), (m), __LINE__)
#define TEST_ASSERT_NOT_NULL(p) UnityTrue((p) != NULL, "Expected Non-NULL", __LINE__)
#define TEST_ASSERT_EQUAL_INT(e, a) UnityNum((long long)(e), (long long)(a), 0, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_INT_MESSAGE(e, a, m) UnityNum((long long)(e), (long long)(a), 0, (m), __LINE__)
#define TEST_ASSERT_EQUAL_UINT8(e, a) UnityNum((long long)(uint8_t)(e), (long long)(uint8_t)(a), 0, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_UINT16(e, a) UnityNum((long long)(uint16_t)(e), (long long)(uint16_t)(a), 0, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_UINT8_MESSAGE(e, a, m) UnityNum((long long)(uint8_t)(e), (long long)(uint8_t)(a), 0, (m), __LINE__)
#define TEST_ASSERT_EQUAL_UINT16_MESSAGE(e, a, m) UnityNum((long long)(uint16_t)(e), (long long)(uint16_t)(a), 0, (m), __LINE__)
#define TEST_ASSERT_EQUAL_HEX8_MESSAGE(e, a, m) UnityNum((long long)(uint8_t)(e), (long long)(uint8_t)(a), 1, (m), __LINE__)
#define TEST_ASSERT_EQUAL_UINT32(e, a) UnityNum((long long)(uint32_t)(e), (long long)(uint32_t)(a), 0, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_UINT32_MESSAGE(e, a, m) UnityNum((long long)(uint32_t)(e), (long long)(uint32_t)(a), 0, (m), __LINE__)
#define TEST_ASSERT_EQUAL_HEX8(e, a) UnityNum((long long)(uint8_t)(e), (long long)(uint8_t)(a), 1, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_HEX32(e, a) UnityNum((long long)(uint32_t)(e), (long long)(uint32_t)(a), 1, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_HEX32_MESSAGE(e, a, m) UnityNum((long long)(uint32_t)(e), (long long)(uint32_t)(a), 1, (m), __LINE__)
#define TEST_ASSERT_EQUAL_STRING(e, a) UnityStr((e), (a), NULL, __LINE__)
#define TEST_ASSERT_EQUAL_STRING_MESSAGE(e, a, m) UnityStr((e), (a), (m), __LINE__)
#define TEST_ASSERT_EQUAL_UINT8_ARRAY(e, a, n) UnityArr((e), (a), (n), 1, NULL, __LINE__)
#define TEST_ASSERT_EQUAL_UINT16_ARRAY(e, a, n) UnityArr((e), (a), (n), 2, NULL, __LINE__)
#define TEST_ASSERT_INT_WITHIN(d, e, a) UnityWithin((long long)(d), (long long)(e), (long long)(a), NULL, __LINE__)
#define TEST_ASSERT_INT_WITHIN_MESSAGE(d, e, a, m) UnityWithin((long long)(d), (long long)(e), (long long)(a), (m), __LINE__)
#define TEST_ASSERT_UINT32_WITHIN(d, e, a) UnityWithin((long long)(d), (long long)(uint32_t)(e), (long long)(uint32_t)(a), NULL, __LINE__)
#endif
