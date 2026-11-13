/* cal.c - two-point calibration: what the ADC reads at 0 V and at 3.3 V. */
#include "cal.h"
#include "drills.h"

/* @TODO(M6-02/5) After you measure (issue M6-02 steps 5 and 6) replace the two numbers below with yours: offset = counts with PA2 on GND, span = (counts on 3V3) minus (counts on GND). [src: M6-02 step 5] */
cal_t g_cal = { 0, 4095 };

/* @TODO(M6-02/1) cal_fit: if raw_3v3 < raw_gnd + 2000 return -1 (the readings make no sense). Otherwise offset = raw_gnd, span = raw_3v3 - raw_gnd, return 0. [src: M6-02 step 1] */
int cal_fit(cal_t *c, uint16_t raw_gnd, uint16_t raw_3v3)
{
    (void)c; (void)raw_gnd; (void)raw_3v3;
    return -1;
}

/* @TODO(M6-02/2) cal_apply: if raw <= offset return 0. Else corrected = mul_div(raw - offset, 4095, span), forced into 0..4095 with clamp_i. [src: M6-02 step 2, card C5] */
uint16_t cal_apply(const cal_t *c, uint16_t raw)
{
    (void)c;
    return raw;
}
