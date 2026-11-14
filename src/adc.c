/* adc.c - ADC1 channel 2 on PA2. */
#include "adc.h"
#include "drills.h"
#include "stm32f103.h"

/* @TODO(M2-01/3) adc_init, card C4: clock (GPIOA + ADC1 in APB2ENR; ADCPRE = 2 with C1), pin (PA2 nibble 0x0 = analog), configure (SMPR2 field of channel 2 = 4 for 41.5 cycles; SQR3 field 0..4 = 2), start (CR2 = ADON, wait a moment, set CAL and wait_flag CAL clear, then EXTSEL = 7 and EXTTRIG). [src: M2-01 step 3, cards C1+C2+C3+C4] */
void adc_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;
  RCC->CFGR = field_set(RCC->CFGR, RCC_CFGR_ADCPRE_Pos, 2u, 2u);
  gpio_cfg(GPIOA, 2u, 0x0u);
  ADC1->SMPR2 = field_set(ADC1->SMPR2, smpr_field(2u),3u,4u);
  ADC1->SQR3 = field_set(ADC1->SQR3, 0u, 5u, 2u);
  ADC1->CR2 = ADC_CR2_ADON;

  for (volatile int i = 0; i < 1000; i++){ } // wait  
  
  ADC1->CR2 |= ADC_CR2_CAL;
  wait_flag(&ADC1->CR2, ADC_CR2_CAL, 0, 100000u);

  ADC1->CR2 = field_set(ADC1->CR2, ADC_CR2_EXTSEL_Pos, 3u, 7u) | ADC_CR2_EXTTRIG;
}

/* @TODO(M2-01/4) adc_read, card C3: set SWSTART in CR2, wait_flag on EOC (want set) with a timeout, then return ADC1->DR through *out. Reading DR clears EOC. Return 0 or -1. [src: M2-01 step 4, card C3] */
int adc_read(uint16_t *out)
{
  ADC1->CR2 |= ADC_CR2_SWSTART;

  if ((wait_flag(&ADC1->SR, ADC_SR_EOC, 1, 100000u)) != 0){
    return -1;
  }
  *out = (uint16_t)ADC1->DR;
  return 0;
}

/* @TODO(M2-01/5) adc_to_mv: raw counts (0..4095) to millivolts (0..3300) with mul_div, card C5. [src: M2-01 step 5, card C5] */
uint32_t adc_to_mv(uint32_t raw)
{
    (void)raw;
    return 0;
}
