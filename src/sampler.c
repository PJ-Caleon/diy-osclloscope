/* sampler.c - TIM3 update event triggers the ADC; DMA1 channel 1 stores 256 samples. */
#include "sampler.h"
#include "drills.h"
#include "stm32f103.h"

uint16_t g_cap[SAMPLE_COUNT];

/* @TODO(M2-02/1) sampler_timing: ticks = SYSCLK_HZ / fs_hz. The ARR register is only 16 bits, so psc = (ticks - 1) / 65536 and arr = ticks / (psc + 1) - 1. Write both results through the pointers. [src: M2-02 step 1] */
void sampler_timing(uint32_t fs_hz, uint32_t *psc, uint32_t *arr)
{
    (void)fs_hz;
    *psc = 0; *arr = 0;
}

/* @TODO(M2-02/2) sampler_init: enable the DMA1 clock (AHBENR bit 0) and the TIM3 clock (APB1ENR bit 1). [src: M2-02 step 2] */
void sampler_init(void)
{
}

void sampler_stop(void)
{
}

void sampler_start(uint32_t fs_hz)
{
    /* @TODO(M2-02/3) Timer first: sampler_timing(fs_hz, ...) then PSC and ARR; CR2 MMS field (shift 4, width 3) = 2 with C1; EGR = UG to load the values; clear SR. [src: M2-02 step 3, card C1] */
    /* @TODO(M2-02/4) DMA next: clear flags (IFCR = 0xF); CPAR = address of ADC1->DR; CMAR = address of g_cap; CNDTR = SAMPLE_COUNT; CCR = PSIZE 1 and MSIZE 1 (C1 twice) | MINC | EN. [src: M2-02 step 4, card C1] */
    /* @TODO(M2-02/5) Last: route the ADC to TIM3: CR2 EXTSEL field (shift 17, width 3) = 4 with C1, then set DMA and EXTTRIG; finally start the timer (CR1 CEN). [src: M2-02 step 5, card C1] */
}

/* @TODO(M2-02/6) sampler_wait, card C3: wait_flag on DMA1->ISR bit TCIF1 (want set) with `tries`; on success clear it with IFCR = TCIF1 and return 0. [src: M2-02 step 6, card C3] */
int sampler_wait(uint32_t tries)
{
    (void)tries;
    return -1;
}
