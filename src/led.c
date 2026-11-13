/* led.c - on-board LED on PC13 (active-low: pin LOW = LED on). */
#include "led.h"
#include "drills.h"

void led_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  led_off();
  gpio_cfg(GPIOC,LED_PIN,0x2u);
}

void led_on(void)
{
  // 0x20000000
  GPIOC->BSRR = BIT(LED_PIN + 16u);
}

void led_off(void)
{
  // 0x00002000
  GPIOC->BSRR = BIT(LED_PIN);
}

/* @TODO(M1-01/11) led_toggle: read the pin in ODR; if it is high call led_on(), else led_off(). [src: M1-01 step 11] */
void led_toggle(void)
{
  // ODR reads pin 
  // since PC13 is active high 
  // If Port C is 1 and Pin is 1 (off), set to on 
  // If Port C is 1 and Pin is 0 (on), set to off 
  if (GPIOC->ODR & BIT(LED_PIN)){
    led_on();
  } else{
    led_off();
  }
}
