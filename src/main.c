/* main.c - firmware entry point. It grows issue by issue; M6-01 turns it into the scope super loop. */
#include "stm32f103.h"
#include "drills.h"
#include "led.h"
#include "clock.h"
#include "uart.h"
#include "fmt.h"
#include "adc.h"
#include "sampler.h"
#include "pwm.h"
#include "i2c.h"
#include "ssd1306.h"
#include "ui.h"
#include "scope.h"


int main(void)
{
  /* @TODO(M1-03/1) Blink: call led_init(); then loop forever: led_toggle(); then a crude delay (a volatile counter counting up to N). Tune N for about 1 blink per second. [src: M1-03 step 1] */
  led_init();  
  /* @TODO(M1-04/10) Replace the crude delay: call clock_init() first, then blink with millis() and reached() (card C6): next = millis() + 500; when reached(millis(), next) toggle and add 500. [src: M1-04 step 10] */
    /* @TODO(M1-05/7) After clock_init(): call uart_init(), build the banner with banner_build() into a char buffer and send it with uart_puts(). [src: M1-05 step 7] */
    /* @TODO(M2-01/6) Every 500 ms: adc_read(), convert with adc_to_mv(), print "raw=... mv=..." with fmt_u32 and uart_puts. [src: M2-01 step 6] */
    /* @TODO(M2-02/7) Call sampler_init(); sampler_start(32000); sampler_wait(); print the first 8 samples of g_cap over UART. [src: M2-02 step 7] */
    /* @TODO(M2-03/5) Call pwm_init() and pwm_set_duty(25); capture, count samples above 2048 with count_above() and print the count. [src: M2-03 step 5] */
    /* @TODO(M3-01/5) Call i2c_init(); run i2c_scan() and print every address found as a number (0x3C = 60). [src: M3-01 step 5] */
    /* @TODO(M3-02/7) Call ssd1306_init(i2c_write); set a few pixels with fb_pixel; ssd1306_flush(). [src: M3-02 step 7] */
    /* @TODO(M6-01/4) Final super loop: init everything, then forever: poll buttons -> ui_handle; if the capture is done: scope_process, scope_draw, ssd1306_flush, start the next capture. [src: M6-01 step 4] */
  for (;;) { 
    led_toggle();
    for (volatile uint32_t i = 0; i < 500000; i++) {}
  }
}
