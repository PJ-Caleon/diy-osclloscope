#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>
uint32_t clock_cfgr_word(void);
uint32_t tick_reload(uint32_t clk_hz, uint32_t tick_hz);
int      clock_init(void);          /* 0 = ok, -1 = a wait timed out */
uint32_t millis(void);
void     SysTick_Handler(void);
#endif
