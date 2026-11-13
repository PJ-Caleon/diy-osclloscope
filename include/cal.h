#ifndef CAL_H
#define CAL_H
#include <stdint.h>
typedef struct { uint16_t offset; uint16_t span; } cal_t;   /* counts at 0 V, and counts from 0 V to 3.3 V */
extern cal_t g_cal;
int      cal_fit(cal_t *c, uint16_t raw_gnd, uint16_t raw_3v3);
uint16_t cal_apply(const cal_t *c, uint16_t raw);
#endif
