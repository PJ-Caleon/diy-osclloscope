#ifndef GFX_H
#define GFX_H
#include <stdint.h>
extern const uint8_t font5x7[95][5];
void draw_grid(void);
void text_draw(unsigned x, unsigned y, const char *s);
void plot_wave(const uint16_t *s, unsigned n, uint32_t vdiv_mv);
#endif
