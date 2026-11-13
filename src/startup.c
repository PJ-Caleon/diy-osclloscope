/* startup.c - vector table and Reset_Handler for the Cortex-M3 in the STM32F103. */
#include <stdint.h>
#include "memutil.h"

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

typedef void (*isr_t)(void);

__attribute__((section(".isr_vector"), used))
const isr_t vectors[16] = {
    /* @TODO(M1-02/4) Slot 0 = initial stack pointer: the ADDRESS of _estack, cast to isr_t. Slot 1 = Reset_Handler. Slot 15 = SysTick_Handler. [src: M1-02 step 4] */
    0,                     /* 0  initial stack pointer */
    0,                     /* 1  reset                 */
    NMI_Handler,           /* 2  NMI           */
    HardFault_Handler,     /* 3  HardFault     */
    MemManage_Handler,     /* 4  MemManage     */
    BusFault_Handler,      /* 5  BusFault      */
    UsageFault_Handler,    /* 6  UsageFault    */
    0, 0, 0, 0,            /* 7-10 reserved    */
    SVC_Handler,           /* 11 SVCall        */
    DebugMon_Handler,      /* 12 DebugMon      */
    0,                     /* 13 reserved      */
    PendSV_Handler,        /* 14 PendSV        */
    0,                     /* 15 SysTick       */
};

/* @TODO(M1-02/3) Reset_Handler: copy .data from Flash to RAM, zero .bss, call main(). Use copy_words and zero_words with the linker symbols. [src: M1-02 step 3] */
void Reset_Handler(void)
{
  copt_words(&_sdata, &_edata, &_sidata);
  zero_words(&_sbss, &_ebss);
  main();
  for (;;){}
}

void Default_Handler(void)
{
    for (;;) { }
}
