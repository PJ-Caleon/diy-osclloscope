/* measure.c - min, max, Vpp, average, frequency. */
#include "measure.h"
#include "drills.h"

/* @TODO(M5-01/1) measure_stats: one pass over s[0..n-1]: track min, track max, add up a uint32_t sum; avg = sum / n. [src: M5-01 step 1] */
void measure_stats(const uint16_t *s, unsigned n, stats_t *st)
{
    (void)s; (void)n;
    st->min = 0; st->max = 0; st->avg = 0;
}

/* @TODO(M5-01/2) stats_vpp_mv = (max - min) counts to mV with mul_div(.., 3300, 4095); stats_avg_mv = avg counts to mV the same way. [src: M5-01 step 2, card C5] */
uint32_t stats_vpp_mv(const stats_t *st)
{
    (void)st;
    return 0;
}

uint32_t stats_avg_mv(const stats_t *st)
{
    (void)st;
    return 0;
}

/* @TODO(M5-02/1) measure_freq: scan i = 1..n-1 for rising crossings (s[i-1] < level and s[i] >= level). Remember the first index, the last index and the count. With fewer than 2 crossings return 0. Else hz = mul_div(fs_hz, count - 1, last - first) (card C5). [src: M5-02 step 1, card C5] */
uint32_t measure_freq(const uint16_t *s, unsigned n, uint16_t level, uint32_t fs_hz)
{
    (void)s; (void)n; (void)level; (void)fs_hz;
    return 0;
}
