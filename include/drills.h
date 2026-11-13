#ifndef DRILLS_H
#define DRILLS_H
#include <stdint.h>
#include "stm32f103.h"
uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value); /* C1 */
unsigned pin_shift(unsigned pin);                                                  /* C2 */
void     gpio_cfg(GPIO_TypeDef *g, unsigned pin, unsigned nibble);                 /* C2 */
int      wait_flag(volatile uint32_t *reg, uint32_t mask, int want_set, uint32_t tries); /* C3 */
uint32_t elapsed(uint32_t now, uint32_t start);                                    /* C6 */
int      reached(uint32_t now, uint32_t deadline);                                 /* C6 */
uint32_t brr_for(uint32_t pclk_hz, uint32_t baud);
uint32_t mul_div(uint32_t a, uint32_t b, uint32_t c);                              /* C5 */
unsigned smpr_field(unsigned channel);
uint32_t i2c_ccr(uint32_t pclk_hz, uint32_t scl_hz, int fast);
unsigned fb_index(unsigned x, unsigned y);
unsigned fb_bit(unsigned y);
int      clamp_i(int v, int lo, int hi);
#endif
