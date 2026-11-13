/* pwm.c - 1 kHz test signal on PA1 (TIM2 channel 2). */
#include "pwm.h"
#include "drills.h"
#include "stm32f103.h"

/* @TODO(M2-03/1) pwm_ccr: compare value = (arr + 1) * percent / 100, with mul_div (card C5). [src: M2-03 step 1, card C5] */
uint32_t pwm_ccr(uint32_t arr, uint32_t percent)
{
    (void)arr; (void)percent;
    return 0;
}

/* @TODO(M2-03/2) pwm_init, card C4: clock (GPIOA + TIM2), pin (PA1 nibble 0xA), configure (PSC 71, ARR 999, CCMR1 OC2M field shift 12 width 3 = 6, OC2PE, CCER CC2E), start (EGR UG, CR1 CEN). [src: M2-03 step 2, cards C1+C2+C4] */
void pwm_init(void)
{
}

/* @TODO(M2-03/3) pwm_set_duty: write CCR2 = pwm_ccr(ARR, percent). [src: M2-03 step 3] */
void pwm_set_duty(uint32_t percent)
{
    (void)percent;
}

/* @TODO(M2-03/4) count_above: how many of the n samples are greater than thr? (loop, counter). [src: M2-03 step 4] */
unsigned count_above(const uint16_t *s, unsigned n, uint16_t thr)
{
    (void)s; (void)n; (void)thr;
    return 0;
}
