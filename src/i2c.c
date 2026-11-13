/* i2c.c - I2C1 master on PB6 (SCL) and PB7 (SDA), 400 kHz, every wait has a timeout. */
#include "i2c.h"
#include "drills.h"
#include "stm32f103.h"

#define I2C_TRIES 20000u

/* @TODO(M3-01/2) i2c_init, card C4: clock (GPIOB + I2C1), pin (PB6 and PB7 nibble 0xF), configure (CR2 FREQ field shift 0 width 6 = 36; CCR = I2C_CCR_FS | i2c_ccr(36 MHz, 400 kHz, fast); TRISE = 12), start (CR1 PE). [src: M3-01 step 2, cards C1+C2+C4] */
void i2c_init(void)
{
}

/* @TODO(M3-01/3) i2c_write, card C3 five times: wait BUSY clear; START and wait SB; send addr<<1 and wait ADDR or AF; on AF clear it, STOP, return NACK; read SR1 then SR2; send bytes (wait TXE each); wait BTF; STOP. Every wait that times out: STOP and return TIMEOUT. [src: M3-01 step 3, card C3] */
int i2c_write(uint8_t addr, const uint8_t *buf, uint16_t len)
{
    (void)addr; (void)buf; (void)len;
    return I2C_ERR_TIMEOUT;
}

/* @TODO(M3-01/4) i2c_scan: try every address 0x08..0x77 with a zero-length i2c_write; store each address that returns I2C_OK in found[] (up to max); return how many. [src: M3-01 step 4] */
int i2c_scan(uint8_t *found, unsigned max)
{
    (void)found; (void)max;
    return 0;
}
