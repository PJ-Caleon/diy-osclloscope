#ifndef SSD1306_H
#define SSD1306_H
#include <stdint.h>
typedef int (*bus_write_fn)(uint8_t addr, const uint8_t *buf, uint16_t len);
#define SSD1306_ADDR 0x3Cu
extern uint8_t g_fb[1024];
void fb_clear(void);
void fb_pixel(unsigned x, unsigned y, int on);
int  ssd1306_init(bus_write_fn bus);
int  ssd1306_flush(void);
#endif
