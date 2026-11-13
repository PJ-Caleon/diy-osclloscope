/* ssd1306.c - 128x64 OLED: framebuffer in RAM, sent over any bus_write_fn (real I2C or a test fake). */
#include "ssd1306.h"
#include "drills.h"

uint8_t g_fb[1024];
static bus_write_fn s_bus;

/* @TODO(M3-02/3) fb_clear: set all 1024 bytes of g_fb to 0 (a for loop). [src: M3-02 step 3] */
void fb_clear(void)
{
}

/* @TODO(M3-02/4) fb_pixel: ignore x >= 128 or y >= 64. Otherwise find the byte with fb_index(x, y) and the bit with fb_bit(y); OR the bit in when on, AND its inverse when off. [src: M3-02 step 4, card C1] */
void fb_pixel(unsigned x, unsigned y, int on)
{
    (void)x; (void)y; (void)on;
}

/* @TODO(M3-02/5) init_seq: type the 25 init bytes from the cheat sheet, in order. ssd1306_init: remember the bus, build a 26-byte buffer = 0x00 control byte + init_seq, send it to SSD1306_ADDR with the bus function, return its result. [src: M3-02 step 5] */
static const uint8_t init_seq[] = { 0x00 };
int ssd1306_init(bus_write_fn bus)
{
    (void)bus;
    return -1;
}

/* @TODO(M3-02/6) ssd1306_flush: first send the 7-byte command write 00 21 00 7F 22 00 07. Then for page 0..7: build a 129-byte buffer (0x40, then the 128 bytes of g_fb starting at page*128) and send it. Return the first non-zero result, else 0. [src: M3-02 step 6] */
int ssd1306_flush(void)
{
    return -1;
}
