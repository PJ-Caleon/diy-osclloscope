/* fake_i2c.c - a fake I2C bus that records every transaction (address + bytes). */
#include <string.h>
#include "fakes.h"
#define MAXTX 32
static struct { uint8_t addr; uint16_t len; uint8_t data[160]; } txlog[MAXTX];
static int ntx, nack_mode;
void fake_i2c_reset(void) { ntx = 0; nack_mode = 0; }
void fake_i2c_set_nack(int on) { nack_mode = on; }
int fake_i2c_write(uint8_t addr, const uint8_t *buf, uint16_t len)
{
    if (ntx < MAXTX) {
        txlog[ntx].addr = addr; txlog[ntx].len = len;
        memcpy(txlog[ntx].data, buf, len < 160 ? len : 160);
    }
    ntx++;
    return nack_mode ? -2 : 0;
}
int fake_i2c_count(void) { return ntx; }
uint8_t fake_i2c_addr(int i) { return txlog[i].addr; }
uint16_t fake_i2c_len(int i) { return txlog[i].len; }
const uint8_t *fake_i2c_data(int i) { return txlog[i].data; }
