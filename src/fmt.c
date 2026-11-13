/* fmt.c - tiny text formatting (no printf: no libc). All functions write a NUL-terminated string and return its length. */
#include "fmt.h"
#include "stm32f103.h"

/* @TODO(M1-05/2) fmt_u32: write v as decimal. Collect digits with v % 10 into a small array (they come out backwards), then copy them out in reverse. Zero prints "0". [src: M1-05 step 2] */
unsigned fmt_u32(char *out, uint32_t v)
{
    (void)v;
    out[0] = 0;
    return 0;
}

static unsigned put_str(char *out, const char *s)
{
    unsigned n = 0;
    while (s[n]) { out[n] = s[n]; n++; }
    out[n] = 0;
    return n;
}

/* @TODO(M1-05/3) banner_build: out = "SCOPE v0.1 clk=" + decimal SYSCLK_HZ + "\r\n". Use put_str and fmt_u32; each returns a length, so advance the write position. [src: M1-05 step 3] */
void banner_build(char *out)
{
    out[0] = 0;
}

/* two decimals: 100 -> "1.00" ; writes whole + '.' + 2 digits, returns length. v is in hundredths. */
static unsigned put_hundredths(char *out, uint32_t v)
{
    unsigned n = fmt_u32(out, v / 100u);
    out[n++] = '.';
    out[n++] = (char)('0' + (v % 100u) / 10u);
    out[n++] = (char)('0' + (v % 10u));
    out[n] = 0;
    return n;
}

/* @TODO(M5-02/2) fmt_mv: millivolts as volts with 2 decimals, rounded to the nearest 10 mV: v = (mv + 5) / 10; then put_hundredths(out, v) and append 'V'. 3300 -> "3.30V". [src: M5-02 step 2] */
unsigned fmt_mv(char *out, uint32_t mv)
{
    (void)mv;
    out[0] = 0;
    return 0;
}

/* @TODO(M5-02/3) fmt_hz: below 1000 Hz print the number then "Hz" (800 -> "800Hz"). From 1000 Hz up: v = (hz + 5) / 10, put_hundredths(out, v), then "kHz" (1000 -> "1.00kHz"). [src: M5-02 step 3] */
unsigned fmt_hz(char *out, uint32_t hz)
{
    (void)hz;
    out[0] = 0;
    return 0;
}
