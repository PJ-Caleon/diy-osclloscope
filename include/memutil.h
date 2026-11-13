#ifndef MEMUTIL_H
#define MEMUTIL_H
#include <stdint.h>
void copy_words(uint32_t *dst, uint32_t *dst_end, const uint32_t *src);
void zero_words(uint32_t *start, uint32_t *end);
#endif
