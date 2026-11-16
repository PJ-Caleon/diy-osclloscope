/* Minimal Unity-compatible implementation (stand-in). See unity.h. */
#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
static jmp_buf jb;
static int in_test, failed_now, n_run, n_fail;
static const char *cur_name = "", *cur_file = "";
void UnityBegin(const char *file) { cur_file = file; n_run = n_fail = 0; printf("\n"); }
int UnityEnd(void) {
    printf("\n-----------------------\n%d Tests %d Failures 0 Ignored\n%s\n", n_run, n_fail, n_fail ? "FAIL" : "OK");
    return n_fail;
}
void UnityRun(void (*fn)(void), const char *name) {
    cur_name = name; failed_now = 0; n_run++;
    if (setjmp(jb) == 0) { in_test = 1; setUp(); fn(); }
    in_test = 0; tearDown();
    if (failed_now) n_fail++; else printf("%s:%s:PASS\n", cur_file, name);
}
void UnityFailAt(const char *msg, int line) {
    printf("%s:%d:%s:FAIL: %s\n", cur_file, line, cur_name, msg ? msg : "(no message)");
    failed_now = 1;
    if (in_test) longjmp(jb, 1);
}
static void fail2(int line, const char *what, const char *msg) {
    char buf[512]; snprintf(buf, sizeof buf, "%s%s%s", what, msg ? " -- " : "", msg ? msg : ""); UnityFailAt(buf, line);
}
void UnityNum(long long e, long long a, int hex, const char *msg, int line) {
    char b[200];
    if (e == a) return;
    if (hex) snprintf(b, sizeof b, "Expected 0x%llX Was 0x%llX", e, a); else snprintf(b, sizeof b, "Expected %lld Was %lld", e, a);
    fail2(line, b, msg);
}
void UnityStr(const char *e, const char *a, const char *msg, int line) {
    char b[300];
    if (e && a && strcmp(e, a) == 0) return;
    snprintf(b, sizeof b, "Expected '%s' Was '%s'", e ? e : "NULL", a ? a : "NULL"); fail2(line, b, msg);
}
void UnityArr(const void *e, const void *a, size_t n, size_t el, const char *msg, int line) {
    char b[200]; size_t i;
    if (memcmp(e, a, n * el) == 0) return;
    for (i = 0; i < n; i++) if (memcmp((const char *)e + i * el, (const char *)a + i * el, el)) break;
    unsigned long long x = 0, y = 0; memcpy(&x, (const char *)e + i * el, el); memcpy(&y, (const char *)a + i * el, el);
    snprintf(b, sizeof b, "Element %zu Expected 0x%llX Was 0x%llX", i, x, y); fail2(line, b, msg);
}
void UnityWithin(long long d, long long e, long long a, const char *msg, int line) {
    char b[200];
    long long diff = e > a ? e - a : a - e;
    if (diff <= d) return;
    snprintf(b, sizeof b, "Values Not Within Delta %lld Expected %lld Was %lld", d, e, a); fail2(line, b, msg);
}
void UnityTrue(int c, const char *msg, int line) { if (!c) fail2(line, msg, NULL); }
