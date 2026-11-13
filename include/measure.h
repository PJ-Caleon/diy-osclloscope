#ifndef MEASURE_H
#define MEASURE_H
#include <stdint.h>
typedef struct { uint16_t min; uint16_t max; uint32_t avg; } stats_t;   /* all in ADC counts */
void     measure_stats(const uint16_t *s, unsigned n, stats_t *st);
uint32_t stats_vpp_mv(const stats_t *st);
uint32_t stats_avg_mv(const stats_t *st);
uint32_t measure_freq(const uint16_t *s, unsigned n, uint16_t level, uint32_t fs_hz);
#endif
