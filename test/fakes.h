#ifndef FAKES_H
#define FAKES_H
#include <stdint.h>
void fake_chip_reset(void);   /* GPIOC/RCC back to reset values */
void fake_i2c_reset(void);
void fake_i2c_set_nack(int on);
int  fake_i2c_write(uint8_t addr, const uint8_t *buf, uint16_t len);
int  fake_i2c_count(void);
uint8_t fake_i2c_addr(int i);
uint16_t fake_i2c_len(int i);
const uint8_t *fake_i2c_data(int i);
#endif
