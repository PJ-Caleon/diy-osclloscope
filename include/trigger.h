#ifndef TRIGGER_H
#define TRIGGER_H
#include <stdint.h>
uint16_t mv_to_counts(uint32_t mv);
int      trig_find(const uint16_t *s, unsigned n_scan, uint16_t level, int rising);
unsigned trig_start(int idx);
#endif
