/* drills.c - the small formulas that come back all project long (pattern cards C1..C6). */
#include "drills.h"

/* @TODO(M1-01/6) field_set: card C1. Clear the field with a mask, then set the new value. [src: M1-01 step 6, card C1] */
uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value)
{
    uint32_t mask = (1u << width) - 1u;
    reg &= ~(mask << shift);
    reg |= (value & mask) << shift;
    return reg;
}

/* @TODO(M1-01/7) pin_shift: card C2. Position of a pin's 4-bit nibble inside CRL or CRH. [src: M1-01 step 7, card C2] */
unsigned pin_shift(unsigned pin)
{
    (void)pin;
    return 0;
}

/* @TODO(M1-01/8) gpio_cfg: card C2. Pick CRL (pin < 8) or CRH, then write the nibble with field_set. [src: M1-01 step 8, cards C1+C2] */
void gpio_cfg(GPIO_TypeDef *g, unsigned pin, unsigned nibble)
{
    (void)g; (void)pin; (void)nibble;
}

/* @TODO(M1-04/7) wait_flag: card C3. Poll until the bit is in the wanted state; give up after `tries` reads. [src: M1-04 step 7, card C3] */
int wait_flag(volatile uint32_t *reg, uint32_t mask, int want_set, uint32_t tries)
{
    (void)reg; (void)mask; (void)want_set; (void)tries;
    return -1;
}

/* @TODO(M1-04/8) elapsed: card C6. How many ticks since start? One subtraction, unsigned. [src: M1-04 step 8, card C6] */
uint32_t elapsed(uint32_t now, uint32_t start)
{
    (void)now; (void)start;
    return 0;
}

/* @TODO(M1-04/9) reached: card C6. Has the deadline passed? Subtract first, then look at the sign. [src: M1-04 step 9, card C6] */
int reached(uint32_t now, uint32_t deadline)
{
    (void)now; (void)deadline;
    return 0;
}

/* @TODO(M1-05/1) brr_for: USART divisor = pclk / baud, rounded to nearest. [src: M1-05 step 1] */
uint32_t brr_for(uint32_t pclk_hz, uint32_t baud)
{
    (void)pclk_hz; (void)baud;
    return 0;
}

/* @TODO(M2-01/1) mul_div: card C5. Multiply first in 64 bits, divide last. [src: M2-01 step 1, card C5] */
uint32_t mul_div(uint32_t a, uint32_t b, uint32_t c)
{
    (void)a; (void)b; (void)c;
    return 0;
}

/* @TODO(M2-01/2) smpr_field: card C2 in disguise. Shift of channel N's 3-bit sample-time field. [src: M2-01 step 2, card C2] */
unsigned smpr_field(unsigned channel)
{
    (void)channel;
    return 0;
}

/* @TODO(M3-01/1) i2c_ccr: CCR divisor for I2C. Standard mode: pclk/(2*scl). Fast mode (DUTY 0): pclk/(3*scl). [src: M3-01 step 1] */
uint32_t i2c_ccr(uint32_t pclk_hz, uint32_t scl_hz, int fast)
{
    (void)pclk_hz; (void)scl_hz; (void)fast;
    return 0;
}

/* @TODO(M3-02/1) fb_index: byte index of pixel (x,y) in the SSD1306 framebuffer: page*128 + x. [src: M3-02 step 1] */
unsigned fb_index(unsigned x, unsigned y)
{
    (void)x; (void)y;
    return 0;
}

/* @TODO(M3-02/2) fb_bit: which bit inside that byte? Bit 0 is the top row of the page. [src: M3-02 step 2] */
unsigned fb_bit(unsigned y)
{
    (void)y;
    return 0;
}

/* @TODO(M3-03/1) clamp_i: force v into the range lo..hi. [src: M3-03 step 1] */
int clamp_i(int v, int lo, int hi)
{
    (void)lo; (void)hi;
    return v;
}
