/* led.c - on-board LED on PC13 (active-low: pin LOW = LED on). */
#include "led.h"
#include "drills.h"

/* @TODO(M1-01/9) led_init: card C4 (clock, pin, configure). Enable the GPIOC clock, drive the pin high (LED off), set PC13 to nibble 0x2. [src: M1-01 step 9, cards C2+C4] */
void led_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  led_off();
  gpio_cfg(GPIOC,LED_PIN,0x2u);
}

/* @TODO(M1-01/10) led_on / led_off: one BSRR write each. Low half sets, high half clears: pin n low = bit (n+16). [src: M1-01 step 10] */
void led_on(void)
{
}

void led_off(void)
{
}

/* @TODO(M1-01/11) led_toggle: read the pin in ODR; if it is high call led_on(), else led_off(). [src: M1-01 step 11] */
void led_toggle(void)
{
}
