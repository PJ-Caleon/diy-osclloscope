#ifndef SUPPORT_H
#define SUPPORT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* file_has(path, needle): 1 if the file contains `needle` as CODE.
 * Comments are removed first (C comments, and # comments in Makefile / .yml files), then ALL
 * whitespace is removed from both the file text and the needle. So spacing never matters. */
static inline int file_has(const char *path, const char *needle)
{
    FILE *f = fopen(path, "r"); if (!f) return 0;
    static char raw[200000], buf[200000]; size_t r = fread(raw, 1, sizeof raw - 1, f); raw[r] = 0; fclose(f);
    int hash = strstr(path, "Makefile") || strstr(path, ".yml") || strstr(path, ".active");
    size_t n = 0;
    for (size_t i = 0; i < r; i++) {
        if (!hash && raw[i] == '/' && raw[i+1] == '*') { i += 2; while (i < r && !(raw[i] == '*' && raw[i+1] == '/')) i++; i++; continue; }
        if (!hash && raw[i] == '/' && raw[i+1] == '/') { while (i < r && raw[i] != '\n') i++; continue; }
        if (hash && raw[i] == '#') { while (i < r && raw[i] != '\n') i++; continue; }
        if (!isspace((unsigned char)raw[i])) buf[n++] = raw[i];
    }
    buf[n] = 0;
    char nd[256]; size_t m = 0;
    for (const char *p = needle; *p && m < sizeof nd - 1; p++) if (!isspace((unsigned char)*p)) nd[m++] = *p;
    nd[m] = 0;
    return strstr(buf, nd) != NULL;
}
#define ASSERT_HAS(path, needle, hint) TEST_ASSERT_TRUE_MESSAGE(file_has((path), (needle)), (hint))
#endif
