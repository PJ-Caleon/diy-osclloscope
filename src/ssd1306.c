/* ssd1306.c - 128x64 OLED: framebuffer in RAM, sent over any bus_write_fn (real I2C or a test fake). */
#include "ssd1306.h"
#include "drills.h"

uint8_t g_fb[1024];
static bus_write_fn s_bus;

/* @TODO(M3-02/3) fb_clear: set all 1024 bytes of g_fb to 0 (a for loop). [src: M3-02 step 3] */
void fb_clear(void)
{
  for (unsigned i = 0; i < sizeof g_fb; i++){
    g_fb[i] = 0;
  }
}

/* @TODO(M3-02/4) fb_pixel: ignore x >= 128 or y >= 64. Otherwise find the byte with fb_index(x, y) and the bit with fb_bit(y); OR the bit in when on, AND its inverse when off. [src: M3-02 step 4, card C1] */
void fb_pixel(unsigned x, unsigned y, int on)
{
  if (x >= 128u || y >= 64u){
    return;
  }

  unsigned i = fb_index(x,y);
  uint8_t m = (uint8_t)(1u << fb_bit(y));
  if (on){
    g_fb[i] |= m;
  } else{
    g_fb[i] &= (uint8_t)~m;
  }
}

/* @TODO(M3-02/5) init_seq: type the 25 init bytes from the cheat sheet, in order. ssd1306_init: remember the bus, build a 26-byte buffer = 0x00 control byte + init_seq, send it to SSD1306_ADDR with the bus function, return its result. [src: M3-02 step 5] */
static const uint8_t init_seq[] = {
  0xAE, 0xD5, 0x80, 0xA8, 0x3F, 
  0xD3, 0x00, 0x40, 0x8D, 0x14, 
  0x20, 0x00, 0xA1, 0xC8, 0xDA,
  0x12, 0x81, 0xCF, 0xD9, 0xF1,
  0xDB, 0x40, 0xA4, 0xA6, 0xAF
};

int ssd1306_init(bus_write_fn bus)
{
  uint8_t buf[1 + sizeof init_seq];
  s_bus = bus;
  buf[0] = 0x00;
  for (unsigned i = 0; i < sizeof init_seq; i++){
    buf[1+i] = init_seq[i];
  }
  return s_bus(SSD1306_ADDR, buf, (uint16_t)sizeof buf);
}

/* @TODO(M3-02/6) ssd1306_flush: first send the 7-byte command write 00 21 00 7F 22 00 07. Then for page 0..7: build a 129-byte buffer (0x40, then the 128 bytes of g_fb starting at page*128) and send it. Return the first non-zero result, else 0. [src: M3-02 step 6] */
int ssd1306_flush(void)
{
  static const uint8_t cmd[7] = {
    0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07
  };

  uint8_t buf[129];

  int r = s_bus(SSD1306_ADDR, cmd, sizeof cmd);
  if (r != 0){
    return r;
  }

  for (unsigned page = 0; page < 8u; page++){
    buf[0] = 0x40;
    for (unsigned i = 0; i < 128u; i++){
      buf[i+1] = g_fb[page * 128u + i];
    }  
    r = s_bus(SSD1306_ADDR, buf, sizeof buf);
    if (r != 0){
      return r;
    }
  }

  return 0;
}
