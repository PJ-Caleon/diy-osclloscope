/* clock.c - 72 MHz from the 8 MHz crystal, and a 1 ms SysTick heartbeat. */
#include "clock.h"
#include "drills.h"
#include "stm32f103.h"

static volatile uint32_t g_ms;
#define WAIT_TRIES 100000u

/* @TODO(M1-04/1) clock_cfgr_word: build the CFGR value with card C1 three times: PLLMUL (shift 18, width 4, value 7), PPRE1 (shift 8, width 3, value 4), ADCPRE (shift 14, width 2, value 2); also OR in RCC_CFGR_PLLSRC_HSE. Do NOT set SW here. [src: M1-04 step 1, card C1] */
uint32_t clock_cfgr_word(void)
{
  volatile uint32_t w = RCC_CFGR_PLLSRC_HSE; // Assign w to 16 bits 
  w = field_set(w, RCC_CFGR_PLLMUL_Pos, 4u, 7u); // 8 MHz x9 = 72 MHz 
  w = field_set(w, RCC_CFGR_PPRE1_Pos, 3u, 4u); // 72/2 = 36 MHz for bus drivers since max is only 48 MHz 
  w = field_set(w, RCC_CFGR_ADCPRE_Pos, 2u, 2u); // 72/6 = 12 MHz since ADC max is 14 MHz
  return w;
}

/* @TODO(M1-04/5) tick_reload: SysTick counts clk/tick_hz ticks, and LOAD holds that minus one. [src: M1-04 step 5] */
uint32_t tick_reload(uint32_t clk_hz, uint32_t tick_hz)
{
  return (clk_hz/tick_hz) - 1u;
}

int clock_init(void)
{
  /* @TODO(M1-04/2) Card C4 step 1: set HSEON in RCC->CR, then wait_flag on HSERDY (want set). Return -1 on timeout. [src: M1-04 step 2, card C3] */
  RCC->CR |= RCC_CR_HSEON;
  if (wait_flag(&RCC->CR, RCC_CR_HSERDY, 1, WAIT_TRIES) != 0){
    return -1;
  }

  /* @TODO(M1-04/3) Flash needs 2 wait states above 48 MHz: write FLASH->ACR with LATENCY field (shift 0, width 3) = 2 using card C1, plus the PRFTBE bit. Do this BEFORE the clock goes up. [src: M1-04 step 3, card C1] */
    /* @TODO(M1-04/4) Write CFGR = clock_cfgr_word(); set PLLON and wait for PLLRDY; then switch SW (shift 0, width 2) to 2 with card C1 and wait for SWS_PLL (bit 3). [src: M1-04 step 4, cards C1+C3] */
  /* @TODO(M1-04/5) SysTick: LOAD = tick_reload(SYSCLK_HZ, 1000), VAL = 0, CTRL = ENABLE | TICKINT | CLKSRC. [src: M1-04 step 5] */
  SYSTICK->LOAD = tick_reload(SYSCLK_HZ, 1000u);
  SYSTICK->VAL = 0u;
  SYSTICK->CTRL = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSRC;
  return 0;
}

/* @TODO(M1-04/6) SysTick_Handler adds 1 to g_ms. millis() returns g_ms. [src: M1-04 step 6] */
void SysTick_Handler(void)
{
}

uint32_t millis(void)
{
    return 0;
}
