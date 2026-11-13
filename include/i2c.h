#ifndef I2C_H
#define I2C_H
#include <stdint.h>
#define I2C_OK           0
#define I2C_ERR_TIMEOUT -1
#define I2C_ERR_NACK    -2
#define I2C_ERR_BUSY    -3
void i2c_init(void);
int  i2c_write(uint8_t addr, const uint8_t *buf, uint16_t len);
int  i2c_scan(uint8_t *found, unsigned max);
#endif
