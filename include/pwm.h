#ifndef PWM_H
#define PWM_H
#include <stdint.h>
void     pwm_init(void);
void     pwm_set_duty(uint32_t percent);
uint32_t pwm_ccr(uint32_t arr, uint32_t percent);
unsigned count_above(const uint16_t *s, unsigned n, uint16_t thr);
#endif
