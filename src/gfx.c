/* gfx.c - grid, text and waveform drawing into the framebuffer. */
#include "gfx.h"
#include "ssd1306.h"
#include "drills.h"

/* @TODO(M3-03/2) draw_grid: dots only. Horizontal lines at y = 16,24,32,40,48,56,63 with a dot every 4th x. Vertical lines at x = 0,16,...,112,127 with a dot on every even y from 16 to 63. [src: M3-03 step 2] */
void draw_grid(void)
{
}

/* @TODO(M3-03/3) text_draw: for each character c (stop at NUL): glyph = font5x7[c - 32]. For col 0..4 and row 0..6, if bit `row` of glyph[col] is set, fb_pixel(x + col, y + row, 1). Then x += 6. Skip characters outside 32..126. [src: M3-03 step 3] */
void text_draw(unsigned x, unsigned y, const char *s)
{
    (void)x; (void)y; (void)s;
}

/* @TODO(M3-03/4) plot_wave: for x = 0..n-1 (max 128): mv = mul_div(s[x], 3300, 4095); rows = mul_div(mv, 8, vdiv_mv); y = clamp_i(63 - rows, 16, 63). Plot (x, y); for x > 0 also fill the vertical gap between the previous y and this y. [src: M3-03 step 4, cards C5+drill clamp_i] */
void plot_wave(const uint16_t *s, unsigned n, uint32_t vdiv_mv)
{
    (void)s; (void)n; (void)vdiv_mv;
}
