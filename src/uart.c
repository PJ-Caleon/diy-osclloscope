/* uart.c - USART1 transmit on PA9 (RX on PA10), 115200 baud. */
#include "uart.h"
#include "drills.h"
#include "stm32f103.h"

/* @TODO(M1-05/4) uart_init, card C4: clock (GPIOA + USART1 in APB2ENR), pin (PA9 nibble 0xB, PA10 nibble 0x4), configure (BRR = brr_for(72 MHz, 115200)), start (CR1 = UE | TE | RE). [src: M1-05 step 4, cards C2+C4] */
void uart_init(void)
{
}

/* @TODO(M1-05/5) uart_putc, card C3: wait_flag on TXE (want set), then write the byte to DR. [src: M1-05 step 5, card C3] */
void uart_putc(char c)
{
    (void)c;
}

/* @TODO(M1-05/6) uart_puts: call uart_putc for each character until the NUL at the end. [src: M1-05 step 6] */
void uart_puts(const char *s)
{
    (void)s;
}
