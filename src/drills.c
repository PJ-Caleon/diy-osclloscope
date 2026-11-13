/* drills.c - the small formulas that come back all project long (pattern cards C1..C6). */
#include "drills.h"

uint32_t field_set(uint32_t reg, unsigned shift, unsigned width, uint32_t value)
{
  volatile uint32_t mask = (1u << width) - 1u; // mask = 0xF
  reg &= ~(mask << shift); // Go to digit position at left by 2^n = shift and change the value to F and compliment is 0 
  reg |= (value & mask) << shift; // change the value of that bit with the value 
  return reg;
}

unsigned pin_shift(unsigned pin)
{
  return (pin % 8u) * 4u;
}

void gpio_cfg(GPIO_TypeDef *g, unsigned pin, unsigned nibble)
{
  volatile uint32_t *reg = (pin < 8u) ? &g->CRL : &g->CRH;
  *reg = field_set(*reg, pin_shift(pin), 4u, nibble);
}

/* @TODO(M1-04/7) wait_flag: card C3. Poll until the bit is in the wanted state; give up after `tries` reads. [src: M1-04 step 7, card C3] */
int wait_flag(volatile uint32_t *reg, uint32_t mask, int want_set, uint32_t tries)
{
  while (tries--){
    uint32_t is_set = ((*reg & mask) != 0u);
    if (is_set == (want_set != 0)){
      return 0;
    }
  }
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
