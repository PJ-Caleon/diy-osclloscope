#ifndef FMT_H
#define FMT_H
#include <stdint.h>
unsigned fmt_u32(char *out, uint32_t v);
void     banner_build(char *out);
unsigned fmt_mv(char *out, uint32_t mv);
unsigned fmt_hz(char *out, uint32_t hz);
#endif
