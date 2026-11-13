/* uart.c - USART1 transmit on PA9 (RX on PA10), 115200 baud. */
#include "uart.h"
#include "drills.h"
#include "stm32f103.h"

/* @TODO(M1-05/4) uart_init, card C4: clock (GPIOA + USART1 in APB2ENR), pin (PA9 nibble 0xB, PA10 nibble 0x4), configure (BRR = brr_for(72 MHz, 115200)), start (CR1 = UE | TE | RE). [src: M1-05 step 4, cards C2+C4] */
void uart_init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
  gpio_cfg(GPIOA, 9u, 0xBu);
  gpio_cfg(GPIOA, 10u, 0x4u);
  USART1->BRR = brr_for(SYSCLK_HZ, 115200u);
  USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

/* @TODO(M1-05/5) uart_putc, card C3: wait_flag on TXE (want set), then write the byte to DR. [src: M1-05 step 5, card C3] */
void uart_putc(char c)
{
  if (wait_flag(&USART1->SR, USART_SR_TXE, 1, 10000u) != 0){
    return; // give up
  }
  USART1->DR = (uint32_t)(uint8_t)c;
}

/* @TODO(M1-05/6) uart_puts: call uart_putc for each character until the NUL at the end. [src: M1-05 step 6] */
void uart_puts(const char *s)
{
  while (*s){
    uart_putc(*s);
    s++;
  }
}
