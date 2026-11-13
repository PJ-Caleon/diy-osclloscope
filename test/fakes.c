/* fakes.c - a fake GPIOC and RCC made of plain RAM, so register code can run on the PC. */
#include <string.h>
#include "stm32f103.h"
#include "fakes.h"
GPIO_TypeDef fake_gpioc;
RCC_TypeDef  fake_rcc;
void fake_chip_reset(void)
{
    memset(&fake_gpioc, 0, sizeof fake_gpioc);
    memset(&fake_rcc, 0, sizeof fake_rcc);
    fake_gpioc.CRL = 0x44444444u;   /* reset value: all pins floating input */
    fake_gpioc.CRH = 0x44444444u;
}
