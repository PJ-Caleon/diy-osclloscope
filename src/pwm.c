/* pwm.c - 1 kHz test signal on PA1 (TIM2 channel 2). */
#include "pwm.h"
#include "drills.h"
#include "stm32f103.h"

/* @TODO(M2-03/1) pwm_ccr: compare value = (arr + 1) * percent / 100, with mul_div (card C5). [src: M2-03 step 1, card C5] */
uint32_t pwm_ccr(uint32_t arr, uint32_t percent)
{
  return mul_div(arr+1, percent, 100u);
}

/* @TODO(M2-03/2) pwm_init, card C4: clock (GPIOA + TIM2), pin (PA1 nibble 0xA), configure (PSC 71, ARR 999, CCMR1 OC2M field shift 12 width 3 = 6, OC2PE, CCER CC2E), start (EGR UG, CR1 CEN). [src: M2-03 step 2, cards C1+C2+C4] */
void pwm_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  gpio_cfg(GPIOA, 1u, 0xAu);
  TIM2->PSC = 71u;
  TIM2->ARR = 999u;
  TIM2->CCMR1 = field_set(TIM2->CCMR1, TIM_CCMR1_OC2M_Pos,3u,6u) | TIM_CCMR1_OC2PE;
  TIM2->CCER |= TIM_CCER_CC2E;
  TIM2->CCR2 = pwm_ccr(999u, 50u);
  TIM2->EGR = TIM_EGR_UG;
  TIM2->CR1 |= TIM_CR1_CEN;
}

/* @TODO(M2-03/3) pwm_set_duty: write CCR2 = pwm_ccr(ARR, percent). [src: M2-03 step 3] */
void pwm_set_duty(uint32_t percent)
{
  TIM2->CCR2 = pwm_ccr(TIM2->ARR, percent);
}

/* @TODO(M2-03/4) count_above: how many of the n samples are greater than thr? (loop, counter). [src: M2-03 step 4] */
unsigned count_above(const uint16_t *s, unsigned n, uint16_t thr)
{
  unsigned count = 0;
  for (int i = 0; i < n; i++){
    if (s[i] > thr){
      count++;
    }
  }
  return count;
}
